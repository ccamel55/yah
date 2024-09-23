#pragma once

#include <unistd.h>
#include <sys/mman.h>

#include <system/system_none.hpp>

namespace yah::impl {
namespace backend::system {
//! linux implementation
struct linux : none {};
}

namespace detail::system {
//! Get mmap flags from 'VmProtectionType' enum
[[nodiscard]] constexpr int vm_protection_flags_linux(const VmProtectionType protection_type) {
    switch (protection_type) {
        case VmProtectionType::READ:
            return PROT_READ;
        case VmProtectionType::READ_WRITE:
            return PROT_READ | PROT_WRITE;
        case VmProtectionType::EXECUTE:
            return PROT_EXEC;
        case VmProtectionType::EXECUTE_READ:
            return PROT_EXEC | PROT_READ;
        case VmProtectionType::EXECUTE_READ_WRITE:
            return PROT_EXEC | PROT_READ | PROT_WRITE;
        default:
            break;
    }
    return PROT_READ;
}
}

//! linux specialization of system
template<>
struct system_base<backend::system::linux> {
    [[nodiscard]] static uint64_t vm_page_size() {
        return sysconf(_SC_PAGESIZE);
    }

    [[nodiscard]] static std::optional<std::span<std::byte>> vm_allocate(
        const size_t size_bytes,
        const VmProtectionType protection_type = VmProtectionType::READ_WRITE
    ) {
        if (size_bytes <= 0) {
            return std::nullopt;
        }

        const int flags = detail::system::vm_protection_flags_linux(protection_type);
        void* address = mmap(nullptr, size_bytes, flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

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

        return munmap(
            memory.data(),
            memory.size_bytes()
        ) == 0;
    }

    [[nodiscard]] static bool vm_commit(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        // Linux commits on first write :D
        return true;
    }

    [[nodiscard]] static bool vm_uncommit(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return madvise(
            memory.data(),
            memory.size_bytes(),
            MADV_DONTNEED
        ) == 0;
    }

    [[nodiscard]] static bool vm_protect(
        const std::span<std::byte>& memory,
        const VmProtectionType protection_type
    ) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return mprotect(
            memory.data(),
            memory.size_bytes(),
            detail::system::vm_protection_flags_linux(protection_type)
        ) == 0;
    }

    [[nodiscard]] static bool vm_lock(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return mlock(
            memory.data(),
            memory.size_bytes()
        ) == 0;
    }

    [[nodiscard]] static bool vm_unlock(const std::span<std::byte>& memory) {
        if (memory.data() == nullptr ||
            memory.size_bytes() <= 0) {
            return false;
        }

        return munlock(
            memory.data(),
            memory.size_bytes()
        ) == 0;
    }
};
}