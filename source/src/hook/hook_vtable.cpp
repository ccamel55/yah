#include <hook/hook_vtable.hpp>
#include <system/system.hpp>

#include <cassert>

using namespace yah;

namespace {
constexpr size_t get_vtable_size(const uintptr_t* vtable) {
    // Walk this bad boy until we start seeing stuff we shouldn't be seeing
    size_t length = 0;

    while (true) {
        // Thank you `winapi.h`!
        // - literally no clue what this does but it works
        if (vtable[length] >> 16 == 0) {
            break;
        }

        length += 1;
    }

    return length;
}
}

hook_vtable::hook_vtable(uintptr_t* vtable) {
    assert(vtable != nullptr);

    // Dereference to get the pointer that points to the actual vtable.
    _vtable_address = vtable;
    _vtable_pointer = reinterpret_cast<uintptr_t*>(*_vtable_address);

    // Create our replacement vtable, adjust size for RTTI accordingly
    _vtable_size        = get_vtable_size(_vtable_pointer);
    _vtable_replacement = std::make_unique<uintptr_t[]>(_vtable_size + YAH_VTABLE_RTTI);

    // Copy original/existing vtable into our new replacement table - adjust first depending on RTTI
    std::copy(_vtable_pointer - YAH_VTABLE_RTTI, _vtable_pointer + _vtable_size, _vtable_replacement.get());

    const size_t page_size = getpagesize();

    // Change virtual memory protection since we are going to replace this in a bit
    [[maybe_unused]] const auto _result_1 = system::vm_protect(
        std::span(reinterpret_cast<std::byte*>(_vtable_address), page_size),
        VmProtectionType::EXECUTE_READ_WRITE
    );

    // Replace vtable pointer with address of replacement vtable
    *reinterpret_cast<uintptr_t**>(_vtable_address) = &_vtable_replacement[YAH_VTABLE_RTTI];

    // Restore virtual memory protection
    [[maybe_unused]] const auto _result_2 = system::vm_protect(
         std::span(reinterpret_cast<std::byte*>(_vtable_address), page_size),
         VmProtectionType::EXECUTE_READ
    );
}

hook_vtable::~hook_vtable() {
    assert(_vtable_address != nullptr);
    assert(_vtable_pointer != nullptr);

    const size_t page_size = getpagesize();

    // Change virtual memory protection since we are going to replace this in a bit
    [[maybe_unused]] const auto _result_1 = system::vm_protect(
        std::span(reinterpret_cast<std::byte*>(_vtable_address), page_size),
        VmProtectionType::EXECUTE_READ_WRITE
    );

    // Replace vtable pointer with address of original vtable
    *reinterpret_cast<uintptr_t**>(_vtable_address) = _vtable_pointer;

    // Restore virtual memory protection
    [[maybe_unused]] const auto _result_2 = system::vm_protect(
         std::span(reinterpret_cast<std::byte*>(_vtable_address), page_size),
         VmProtectionType::EXECUTE_READ
    );
}

std::expected<uintptr_t, std::string_view> hook_vtable::replace_function(const size_t function_index, void* new_function) {
    if (function_index >= _vtable_size || !new_function) {
        return std::unexpected("function index out of range");
    }
    _vtable_replacement[function_index + YAH_VTABLE_RTTI] = reinterpret_cast<uintptr_t>(new_function);
    return _vtable_pointer[function_index];
}

std::expected<void, std::string_view> hook_vtable::restore_function(const size_t function_index) {
    if (function_index >= _vtable_size || !is_hooked(function_index)) {
        return std::unexpected("function index out of range");
    }
    _vtable_replacement[function_index + YAH_VTABLE_RTTI] = _vtable_pointer[function_index];
    return {};
}

bool hook_vtable::is_hooked(const size_t function_index) const {
    if (function_index >= _vtable_size) {
        return false;
    }

    const uintptr_t replacement_table_pointer   = _vtable_replacement[function_index + YAH_VTABLE_RTTI];
    const uintptr_t original_table_pointer      = _vtable_pointer[function_index];

    return replacement_table_pointer != original_table_pointer;
}

std::expected<uintptr_t, std::string_view> hook_vtable::original_function(const size_t function_index) const {
    if (function_index >= _vtable_size) {
        return std::unexpected("function index out of range");
    }
    return _vtable_pointer[function_index];
}
