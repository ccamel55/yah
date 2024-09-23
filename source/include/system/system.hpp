#pragma once

#include <macro.hpp>
#include YAH_INCLUDE(system, YAH_PLATFORM)

namespace yah {
namespace concepts {
//! Concept that 'system' implementation MUST satisfy
template<typename T>
concept System = requires {
    { T::vm_page_size() }                                           -> std::same_as<uint64_t>;
    { T::vm_allocate(size_t{}, VmProtectionType{}) }                -> std::same_as<std::optional<std::span<std::byte>>>;
    { T::vm_deallocate(std::span<std::byte>{}) }                    -> std::same_as<bool>;
    { T::vm_commit(std::span<std::byte>{}) }                        -> std::same_as<bool>;
    { T::vm_uncommit(std::span<std::byte>{}) }                      -> std::same_as<bool>;
    { T::vm_protect(std::span<std::byte>{}, VmProtectionType{}) }   -> std::same_as<bool>;
    { T::vm_lock(std::span<std::byte>{}) }                          -> std::same_as<bool>;
    { T::vm_unlock(std::span<std::byte>{}) }                        -> std::same_as<bool>;
};
}

// Alias correct version and make sure concept is satisfied.
using system = impl::system_base<impl::backend::system::YAH_PLATFORM>;
static_assert(concepts::System<system>, "'system' does not satisfy concept");
}