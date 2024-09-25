cmake_minimum_required(VERSION 3.26)

# Todo
# - allow shared or static library
# - add install stuff
# - documentation/diagrams

# Components
# - abstract away virtual memory functions/basic sys-calls (WinAPI for Windows, /proc/map for nix, march for macos)
# - abstract away assembly/disassembly using CRTP
# - abstract away hooking methods (inline/trampoline, vfunc replacement, vtable replacement)
# - deal with RTTI for VTable replacement

project(yah)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

# todo: let auto resolve do this

set(YAH_PLATFORM linux)
set(YAH_ARCH arm64)

# Add `cmake` folder to module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(options)
include(dependencies)

add_subdirectory(source)
add_subdirectory(test)
