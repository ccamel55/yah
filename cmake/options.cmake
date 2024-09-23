set(
	YAH_ENABLE_TESTS ON CACHE
	STRING "Build tests"
)

set(
	YAH_ENABLE_INSTALL OFF CACHE
	STRING "Enable install"
)

set(
	YAH_PLATFORM OFF CACHE
	STRING "Platform, will auto resolve by default"
)

set(
	YAH_ARCH OFF CACHE
	STRING "Architecture, will auto resolve by default"
)

#
# Debug logs for options
#

message(STATUS "YAH OPTIONS:")
message(STATUS "\t YAH_ENABLE_TESTS:\t\t${YAH_ENABLE_TESTS}")
message(STATUS "\t YAH_ENABLE_INSTALL:\t${YAH_ENABLE_INSTALL}")
message(STATUS "\t YAH_PLATFORM:\t\t\t${YAH_PLATFORM}")
message(STATUS "\t YAH_ARCH:\t\t\t\t${YAH_ARCH}")

#
# Auto resolve platform and architecture if none is explicitly specified
#

# TODO: auto resolve if off

if (NOT YAH_PLATFORM)
	message(FATAL_ERROR "YAH_PLATFORM IS NOT SET")
endif ()

if (NOT YAH_ARCH)
	message(FATAL_ERROR "YAH_ARCH IS NOT SET")
endif ()

# Add the architecture to defines so we can use them in code later

add_compile_definitions(YAH_PLATFORM=${YAH_PLATFORM})
add_compile_definitions(YAH_ARCH=${YAH_ARCH})