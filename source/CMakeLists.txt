add_library(
	${PROJECT_NAME}
	STATIC
		src/yah.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		include
)