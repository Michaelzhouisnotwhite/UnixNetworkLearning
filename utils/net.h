#ifndef NET_PUBLIC
#define NET_PUBLIC
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
auto ListenOn(const std::string& host, const int port) -> int;

auto Accept(int listenfd) -> std::pair<sockaddr_in, int>;

void Send2Server(std::string const& host,
                 const int port,
                 const std::string& message,
                 bool keepalive = false);
#endif /* NET_PUBLIC */
