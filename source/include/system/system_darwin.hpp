#pragma once

#include <system/system_none.hpp>

namespace yah::impl {
namespace backend::system {
//! Darwin aka macOS implementation using MachO
struct darwin : none {};
}

//! Darwin specialization of system
template<>
struct system_base<backend::system::darwin> {
    [[nodiscard]] static uint64_t vm_page_size() {
        return 0;
    }

    [[nodiscard]] static std::optional<std::span<std::byte>> vm_allocate(
        size_t size_bytes,
        VmProtectionType protection_type
    ) {
        return std::nullopt;
    }

    [[nodiscard]] static bool vm_deallocate(const std::span<std::byte>& memory) {
        return false;
    }

    [[nodiscard]] static bool vm_commit(const std::span<std::byte>& memory) {
        return false;
    }

    [[nodiscard]] static bool vm_uncommit(const std::span<std::byte>& memory) {
        return false;
    }

    [[nodiscard]] static bool vm_protect(
        const std::span<std::byte>& memory,
        VmProtectionType protection_type
    ) {
        return false;
    }

    [[nodiscard]] static bool vm_lock(const std::span<std::byte>& memory) {
        return false;
    }

    [[nodiscard]] static bool vm_unlock(const std::span<std::byte>& memory) {
        return false;
    }
};
}