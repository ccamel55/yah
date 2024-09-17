# Skip if tests aren't enabled
if (NOT YAH_ENABLE_TESTS)
	return()
endif ()

add_executable(
	${PROJECT_NAME}-test
		src/test_1.cpp
)

target_link_libraries(
	${PROJECT_NAME}-test
	PRIVATE
		${PROJECT_NAME}
		Catch2::Catch2WithMain
)

target_include_directories(
	${PROJECT_NAME}-test
	PRIVATE
		include
)

catch_discover_tests(${PROJECT_NAME}-test)