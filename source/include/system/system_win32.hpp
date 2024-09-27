#pragma once

#include <Windows.h>
#include <system/system_none.hpp>

namespace yah::impl {
namespace backend::system {
//! windows implementation
struct win32 : none {};
}

namespace detail::system {
//! Get flags from 'VmProtectionType' enum
[[nodiscard]] constexpr DWORD vm_protection_flags_win32(const VmProtectionType protection_type) {
    switch (protection_type) {
        case VmProtectionType::READ:
            return PAGE_READONLY;
        case VmProtectionType::READ_WRITE:
            return PAGE_READWRITE;
        case VmProtectionType::EXECUTE:
            return PAGE_EXECUTE;
        case VmProtectionType::EXECUTE_READ:
            return PAGE_EXECUTE_READ;
        case VmProtectionType::EXECUTE_READ_WRITE:
            return PAGE_EXECUTE_READWRITE;
    }
    return PAGE_READONLY;
}
}

//! windows specialization of system
template<>
struct system_base<backend::system::win32> {
    [[nodiscard]] static uint64_t vm_page_size() {
        SYSTEM_INFO system_info = {};
        GetSystemInfo(&system_info);

        // todo: cache this shit
        return system_info.dwPageSize;
    }

    [[nodiscard]] static std::optional<std::span<std::byte>> vm_allocate(
        const size_t size_bytes,
        const VmProtectionType protection_type = VmProtectionType::READ_WRITE
    ) {
        if (size_bytes <= 0) {
            return std::nullopt;
        }

        const DWORD flags = detail::system::vm_protection_flags_win32(protection_type);
        LPVOID address = VirtualAlloc(nullptr, size_bytes, MEM_RESERVE, flags);

        return std::span {
            static_cast<std::byte*>(address),
            size_bytes
        };
    }

    [[nodiscard]] static bool vm_deallocate(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return VirtualFree(memory.data(), 0, MEM_RELEASE);
    }

    [[nodiscard]] static bool vm_commit(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        constexpr DWORD flags = detail::system::vm_protection_flags_win32(VmProtectionType::READ_WRITE);

        return VirtualAlloc(
            memory.data(),
            memory.size_bytes(),
            MEM_COMMIT,
            flags
        ) != nullptr;
    }

    [[nodiscard]] static bool vm_uncommit(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return VirtualFree(
            memory.data(),
            memory.size_bytes(),
            MEM_DECOMMIT
        );
    }

    [[nodiscard]] static bool vm_protect(
        const std::span<std::byte>& memory,
        const VmProtectionType protection_type
    ) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        DWORD old_protect = 0;
        const DWORD flags = detail::system::vm_protection_flags_win32(protection_type);

        return VirtualProtect(
            memory.data(),
            memory.size_bytes(),
            flags,
            &old_protect
        );
    }

    [[nodiscard]] static bool vm_lock(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return VirtualLock(
            memory.data(),
            memory.size_bytes()
        );
    }

    [[nodiscard]] static bool vm_unlock(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return VirtualUnlock(
            memory.data(),
            memory.size_bytes()
        );
    }
};
}