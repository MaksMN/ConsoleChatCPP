#pragma once
#include <string>
#include <iostream>

#if defined(_WIN32)
typedef unsigned int32_t uint;
#endif

#if defined(_WIN64)
typedef unsigned int uint;
#endif

#if defined(__linux__)
typedef unsigned int uint;
#endif