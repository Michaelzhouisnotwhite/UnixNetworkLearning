//
// Created by MichaelZhou on 2024/3/16.
//

#include <arpa/inet.h>
#include <unistd.h>

#include <array>
#include <iostream>
#include <set>

#include <sys/poll.h>
#include "net.h"
#include "utils.h"
template <>
struct std::hash<pollfd> {
  std::size_t operator()(const pollfd& fd) noexcept {
    return std::hash<int>()(fd.fd);
  }
};

int main(int argc, char* argv[]) {
  auto listenfd = ListenOn("0.0.0.0", 8089);
  std::vector<pollfd> pollfds;
  auto remove_from_pollfds = [&pollfds](int target_fd) {
    auto target_iter =
        std::find_if(pollfds.begin(), pollfds.end(), [target_fd](pollfd _pfd) {
          return _pfd.fd == target_fd;
        });
    pollfds.erase(target_iter);
  };
  std::array<char, 1024> buf{0};
  pollfds.emplace_back(pollfd{.fd = listenfd, .events = POLLIN | POLLERR, .revents = 0});
  while (true) {
    auto ret = poll(pollfds.data(), pollfds.size(), -1);  // -1 永远阻塞
    ZeroMem(buf.data(), buf.size());
    auto enum_vec = std::vector(pollfds);
    for (auto pfd : enum_vec) {
      if (pfd.revents & POLLRDHUP || pfd.revents & POLLERR) {
        close(pfd.fd);
        remove_from_pollfds(pfd.fd);
      } else if (pfd.fd & listenfd && pfd.revents & POLLIN) {
        auto [client_addr, connfd] = Accept(pfd.fd);
        std::printf("new connection client %s:%d\n", inet_ntoa(client_addr.sin_addr),
                    ntohs(client_addr.sin_port));
        pollfds.push_back(
            pollfd{.fd = connfd, .events = POLLIN | POLLRDHUP | POLLERR | POLLOUT});
      } else if (pfd.revents & POLLIN) {
        auto lenrecv = recv(pfd.fd, buf.data(), buf.size() - 1, 0);
        if (lenrecv > 0) {
          std::printf("data recv: %s\n", buf.data());
        }
        if (lenrecv == 0) {
          std::printf("conn closed");
          close(pfd.fd);
          remove_from_pollfds(pfd.fd);
        }
      } else if (pfd.revents & POLLOUT) {
        std::string msg{"data received: good luck"};
        auto send_code = send(pfd.fd, msg.data(), msg.length(), 0);
        if (send_code) {
          remove_from_pollfds(pfd.fd);
          close(pfd.fd);
        }
      }
    }
  }
  return 0;
}
