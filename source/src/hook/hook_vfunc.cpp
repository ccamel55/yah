#include <hook/hook_vfunc.hpp>
#include <system/system.hpp>

#include <cassert>

using namespace yah;

hook_vfunc::hook_vfunc(uintptr_t* vtable, const size_t index) {
    assert(vtable != nullptr);

    // Dereference to get the pointer that points to the actual vtable.
    _vtable_pointer = reinterpret_cast<uintptr_t*>(*vtable);
    _vfunc_index    = index;
}

hook_vfunc::~hook_vfunc() {
    assert(_vtable_pointer != nullptr);

    // Call restore function, will handle all the hoo-ha for us
    [[maybe_unused]] const auto _result_1 = restore_function();
}

std::expected<uintptr_t, std::string_view> hook_vfunc::replace_function(void* new_function) {
    if (new_function == nullptr) {
        return std::unexpected("new_function is null");
    }

    if (!_vfunc_original_address.has_value()) {
        // Save original address if we don't have it
        // we do this here rather than on construction in case something else hooks the function before us!
        _vfunc_original_address = _vtable_pointer[_vfunc_index];
    }

    const size_t page_size = getpagesize();

    // Change virtual memory protection since we are going to replace this in a bit
    [[maybe_unused]] const auto _result_1 = system::vm_protect(
        std::span(reinterpret_cast<std::byte*>(&_vtable_pointer[_vfunc_index]), page_size),
        VmProtectionType::EXECUTE_READ_WRITE
    );

    _vtable_pointer[_vfunc_index] = reinterpret_cast<uintptr_t>(new_function);

    // Restore virtual memory protection
    [[maybe_unused]] const auto _result_2 = system::vm_protect(
         std::span(reinterpret_cast<std::byte*>(&_vtable_pointer[_vfunc_index]), page_size),
         VmProtectionType::EXECUTE_READ
    );

    return *_vfunc_original_address;
}

std::expected<void, std::string_view> hook_vfunc::restore_function() const {
    if (!is_hooked()) {
        return std::unexpected("function has not been hooked");
    }

    const size_t page_size = getpagesize();

    // Change virtual memory protection since we are going to replace this in a bit
    [[maybe_unused]] const auto _result_1 = system::vm_protect(
        std::span(reinterpret_cast<std::byte*>(&_vtable_pointer[_vfunc_index]), page_size),
        VmProtectionType::EXECUTE_READ_WRITE
    );

    _vtable_pointer[_vfunc_index] = *_vfunc_original_address;

    // Restore virtual memory protection
    [[maybe_unused]] const auto _result_2 = system::vm_protect(
         std::span(reinterpret_cast<std::byte*>(&_vtable_pointer[_vfunc_index]), page_size),
         VmProtectionType::EXECUTE_READ
    );

    return {};
}

bool hook_vfunc::is_hooked() const {
    if (!_vfunc_original_address.has_value()) {
        return false;
    }

    const uintptr_t original_address = *_vfunc_original_address;
    const uintptr_t current_function_address = _vtable_pointer[_vfunc_index];

    return original_address != current_function_address;
}

uintptr_t hook_vfunc::original_function() const {
    // If we have already saved/hook then return function pointer at hook otherwise
    // just return hat ever is currently in the vtable.
    return _vfunc_original_address.value_or(_vtable_pointer[_vfunc_index]);
}
