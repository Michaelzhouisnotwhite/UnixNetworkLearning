#ifndef UTILS_UTILS
#define UTILS_UTILS

#include <cstddef>
#include <cstring>
#include <type_traits>
inline void ZeroMem(void* start_addr, std::size_t len) {
    std::memset(start_addr, 0, len);
}

template <typename T>
void ZeroMem(T type){
  ZeroMem(type, sizeof(std::remove_pointer_t<T>));
}
#endif /* UTILS_UTILS */
