#include "net.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include "utils.h"

auto ListenOn(const std::string& host, const int port) -> int {
  sockaddr_in addr{};
  ZeroMem(&addr);
  int ret = 0;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port);

  int listenfd = socket(PF_INET, SOCK_STREAM, 0);
  int yes = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return -1;
  }

  assert(listenfd >= 0);
  ret = bind(listenfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
  assert(ret != -1);
  ret = listen(listenfd, 50); // 处于未accept状态的队列
  assert(ret != -1);
  return listenfd;
}

auto Accept(int listenfd) -> std::pair<sockaddr_in, int> {
  sockaddr_in client_addr{};
  socklen_t client_addrlen = sizeof(client_addr);
  int connfd = accept(listenfd, reinterpret_cast<sockaddr*>(&client_addr), &client_addrlen);
  if (connfd < 0) {
    std::cout << "errno is:" << errno << "\n";
    close(listenfd);
    assert(connfd);
  }
  return {client_addr, connfd};
}

absl::StatusOr<std::string> Send2Server(std::string const& host,
                                        const int port,
                                        const std::string& message,
                                        bool keepalive) {
  sockaddr_in addr{};
  ZeroMem(&addr);
  int ret = 0;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    return absl::InternalError("connection with the server failed...\n");
  }
  auto wret = write(sockfd, message.c_str(), message.size());
  if (wret == -1) {
    return absl::InternalError("send error");
  }
  std::array<char, 1024> recv_buf{0};
  wret = recv(sockfd, recv_buf.data(), recv_buf.size(), 0);
  if (wret == -1) {
    return absl::InternalError("recv error");
  }
  std::string result;
  for (const auto& c : recv_buf) {
    if (c) {
      result.push_back(c);
    } else {
      break;
    }
  }
  close(sockfd);
  return result;
}