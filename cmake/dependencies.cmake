Include(FetchContent)

# todo: make these submodules actually -  this is stupid for this

# Import catch 2 for testing
if (YAH_ENABLE_TESTS)
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY	https://github.com/catchorg/Catch2.git
		GIT_TAG        	v3.7.1
	)

	FetchContent_MakeAvailable(Catch2)

	list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

	include(CTest)
	include(Catch)

	enable_testing()
endif ()

## Import Capstone for disassembly
##set(BUILD_SHARED_LIBS OFF)
#set(CAPSTONE_BUILD_STATIC_RUNTIME OFF)
#set(CAPSTONE_BUILD_DIET OFF)
#set(CAPSTONE_BUILD_TESTS OFF)
#set(CAPSTONE_BUILD_CSTOOL OFF)
#set(CAPSTONE_BUILD_CSTEST OFF)
#set(CAPSTONE_DEBUG OFF)
##set(CAPSTONE_INSTALL ${LIB_INSTALL_DEPENDENCIES})
##set(CAPSTONE_X86_ATT_DISABLE ON)
##set(CAPSTONE_OSXKERNEL_SUPPORT OFF) # todo: enable when building on macos
#
#FetchContent_Declare(
#	Capstone
#	GIT_REPOSITORY	https://github.com/capstone-engine/capstone.git
#	GIT_TAG    	 	5.0.1
#)
#
#FetchContent_MakeAvailable(Capstone)
#
## Import Keystone for assembly
#set(KEYSTONE_BUILD_STATIC_RUNTIME OFF)
#set(BUILD_LIBS_ONLY ON)
#
#FetchContent_Declare(
#	Keystone
#	GIT_REPOSITORY	https://github.com/keystone-engine/keystone.git
#	GIT_TAG    	 	0.9.2
#)
#
#FetchContent_MakeAvailable(Keystone)
