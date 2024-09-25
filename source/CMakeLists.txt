add_library(
	${PROJECT_NAME}
	STATIC
		src/hook/hook_vfunc.cpp
		src/hook/hook_vtable.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
		include
)