option(
	YAH_ENABLE_TESTS "Build tests"
	ON
)

option(
	YAH_ENABLE_INSTALL "Enable install"
	OFF
)

message(STATUS "YAH OPTIONS:")
message(STATUS "\t YAH_ENABLE_TESTS:\t\t${YAH_ENABLE_TESTS}")
message(STATUS "\t YAH_ENABLE_INSTALL:\t${YAH_ENABLE_INSTALL}")