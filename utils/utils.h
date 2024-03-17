#ifndef UTILS_UTILS
#define UTILS_UTILS

#include <arpa/inet.h>
#include <netinet/in.h>
#include <array>
#include <cstddef>
#include <cstring>
#include <string>
#include <type_traits>
inline void ZeroMem(void* start_addr, std::size_t len) {
  std::memset(start_addr, 0, len);
}

template <typename T>
void ZeroMem(T type) {
  ZeroMem(type, sizeof(std::remove_pointer_t<T>));
}

inline std::string modern_inet_ntop_v4(const in_addr& addr) {
  std::array<char, INET_ADDRSTRLEN> buf;
  inet_ntop(AF_INET, &addr, buf.data(), buf.size());
  std::string res;
  for (auto c : buf) {
    if (!c) {
      break;
    }
    res.push_back(c);
  }
  return res;
}
#endif /* UTILS_UTILS */
