#pragma once

// Stringify
#define YAH_STR(a) #a

// Create custom relative include
#define YAH_TO_INCLUDE_STR(a, b) YAH_STR(a ## _ ## b.hpp)
#define YAH_INCLUDE(file, suffix) YAH_TO_INCLUDE_STR(file, suffix)
