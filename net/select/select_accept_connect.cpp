#include <arpa/inet.h>
#include <unistd.h>

#include <array>
#include <iostream>
#include <set>

#include "net.h"
#include "utils.h"

int main(int argc, char* argv[]) {
  // int reuse = 1;
  // if (setsockopt(*listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
  //   perror("setsockopt");
  //   return -1;
  // }
  auto lfd = ListenOn("0.0.0.0", 8089);
  std::set<decltype(lfd)> sockfds_set;

  std::array<char, 1024> buf{};
  const auto buf_start = &buf[0];
  fd_set read_fds;
  fd_set exception_fds;
  fd_set write_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&exception_fds);
  std::int64_t ret = 0;
  while (true) {
    ZeroMem(buf.data(), buf.size());
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);
    FD_ZERO(&write_fds);
    FD_SET(lfd, &read_fds);
    for (const auto fd : sockfds_set) {
      FD_SET(fd, &read_fds);
      FD_SET(fd, &write_fds);
    }
    timeval timeout{};
    ZeroMem(&timeout);
    ret = select(1024, &read_fds, &write_fds, &exception_fds, nullptr);

    if (ret < 0) {
      std::cout << "select failed"
                << "\n";
      break;
    }

    decltype(sockfds_set) socket_del_set{};
    for (const auto fd : sockfds_set) {
      if (FD_ISSET(fd, &read_fds)) {
        ret = recv(fd, buf_start, buf.size() - 1, 0);
        if (ret < 0) {
          break;
        }
        if (ret == 0) {
          std::puts("conn closed");

          close(fd);
          socket_del_set.insert(fd);
          continue;
        }
        std::printf("get %ld bytes of normal data: %s\n", ret, buf_start);
      } else if (FD_ISSET(fd, &exception_fds)) {
        // MSG_OOB接受带外数据(out of bound) 我理解的是优先级高的数据
        ret = recv(fd, buf_start, buf.size() - 1, MSG_OOB);
        if (ret <= 0) {
          break;
        }
        std::printf("get %ld bytes of oob data: %s\n", ret, buf_start);
      }else if (FD_ISSET(fd, &write_fds)) {
        std::string _buf{"server received: good luck!"};
        ret = write(fd, _buf.data(), _buf.size());
        if (ret) {
          close(fd);
          socket_del_set.insert(fd);
        }
      }
    }
    for (const auto del_fd : socket_del_set) {
      sockfds_set.erase(del_fd);
    }
    if (FD_ISSET(lfd, &read_fds)) {
      auto [client_addr, connfd] = Accept(lfd);
      sockfds_set.insert(connfd);
      std::printf("new connection client %s:%d\n", inet_ntoa(client_addr.sin_addr),
                  ntohs(client_addr.sin_port));
    }
  }

  close(lfd);
  return 0;
}