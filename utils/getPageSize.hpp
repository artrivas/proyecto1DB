
#pragma once

#ifdef _WIN32
#include <windows.h>
DWORD getPageSize();
#elif defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
size_t getPageSize();
#endif
