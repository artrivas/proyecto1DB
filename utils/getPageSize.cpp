
#include "getPageSize.hpp"

#ifdef _WIN32
DWORD getPageSize() {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    return systemInfo.dwPageSize; // uint 32 bits
}

#elif defined(__linux__) || defined(__APPLE__)
size_t getPageSize() {
    long pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize == -1) return 0; // default value on error
    return static_cast<size_t>(pageSize);
}
#endif
