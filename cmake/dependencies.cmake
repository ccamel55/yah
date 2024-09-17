Include(FetchContent)

# Import catch 2 for testing
if (YAH_ENABLE_TESTS)
	FetchContent_Declare(
		Catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG        v3.7.1
	)

	FetchContent_MakeAvailable(Catch2)

	list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)

	include(CTest)
	include(Catch)

	enable_testing()
endif ()

# Import Capstone for disassembly
# TODO

# Import Keystone for assembly
# TODO

