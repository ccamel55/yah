cmake_minimum_required(VERSION 3.26)

# Todo
# - allow shared or static library
# - add install stuff

project(yah)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add `cmake` folder to module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(dependencies)
include(options)

include(host/architecture)
include(host/platform)

add_subdirectory(source)
