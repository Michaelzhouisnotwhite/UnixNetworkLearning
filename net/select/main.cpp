#include <unistd.h>
#include <array>
#include <iostream>
#include "net.h"
#include "utils.h"
int main(int argc, char* argv[]) {
  // int reuse = 1;
  // if (setsockopt(*listen_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
  //   perror("setsockopt");
  //   return -1;
  // }
  auto lfd = ListenOn("0.0.0.0", 8088);
  auto [client_addr, connfd] = Accept(lfd);
  std::array<char, 1024> buf;
  const auto buf_start = &buf[0];
  fd_set read_fds;
  fd_set exception_fds;
  FD_ZERO(&read_fds);
  FD_ZERO(&exception_fds);
  std::int64_t ret = 0;
  while (true) {
    ZeroMem(buf.data(), buf.size());
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);
    timeval timeout;
    ZeroMem(&timeout);
    ret = select(connfd + 1, &read_fds, nullptr, &exception_fds, &timeout);
    if (ret < 0) {
      std::cout << "select failed"
                << "\n";
      break;
    }
    if (FD_ISSET(connfd, &read_fds)) {
      ret = recv(connfd, buf_start, buf.size() - 1, 0);
      if (ret < 0) {
        break;
      }
      std::printf("get %ld bytes of normal data: %s\n", ret, buf_start);
    } else if (FD_ISSET(connfd, &exception_fds)) {
      // MSG_OOB接受带外数据(out of bound) 我理解的是优先级高的数据
      ret = recv(connfd, buf_start, buf.size() - 1, MSG_OOB);
      if (ret <= 0) {
        break;
      }
      std::printf("get %ld bytes of oob data: %s\n", ret, buf_start);
    }
  }
  close(connfd);
  close(lfd);
  return 0;
}