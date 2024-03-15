#ifndef UTILS_NET
#define UTILS_NET
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include "absl/status/status.h"
#include "absl/status/statusor.h"
auto ListenOn(const std::string& host, const int port) -> int;

auto Accept(int listenfd) -> std::pair<sockaddr_in, int>;

absl::StatusOr<std::string> Send2Server(std::string const& host,
                 const int port,
                 const std::string& message,
                 bool keepalive = false);
#endif /* UTILS_NET */
