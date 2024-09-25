#pragma once

#include <cstdint>
#include <optional>
#include <span>

namespace yah {
enum class VmProtectionType {
    READ,
    READ_WRITE,
    EXECUTE,
    EXECUTE_READ,
    EXECUTE_READ_WRITE
};

namespace impl {
namespace backend::system {
//! Default implementation
struct none {};
}

//! Base system type - will be specialised on for more functionality
template<typename Impl = backend::system::none>
struct system_base {};
}
}

