#include <arpa/inet.h>
#include <unistd.h>

#include <array>
#include <iostream>
#include <set>

#include <sys/epoll.h>
#include "fmt/core.h"
#include "net.h"
#include "utils.h"
int main(int argc, char* argv[]) {
  int epoll_fd = epoll_create1(0);
  auto listenfd = ListenOn("0.0.0.0", 8089);
  if (epoll_fd == -1) {
    fmt::println(stderr, "Failed to create epoll file descriptor\n");
    return 1;
  }
  epoll_event listen_event;
  listen_event.events = EPOLLIN;
  listen_event.data.fd = listenfd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &listen_event)) {
    fmt::println(stderr, "listen event register failed");
    close(epoll_fd);
    return -1;
  };

  std::array<char, 1024> buf{0};
  std::vector<epoll_event> epoll_events{listen_event};
  while (true) {
    ZeroMem(buf.data(), buf.size());
    auto remove_from_epoll_events = [&epoll_events](int target_fd) {
      auto target_iter = std::find_if(epoll_events.begin(), epoll_events.end(),
                                      [target_fd](epoll_event _pfd) {
                                        return _pfd.data.fd == target_fd;
                                      });
      epoll_events.erase(target_iter);
    };
    auto event_ready_count = epoll_wait(epoll_fd, epoll_events.data(),
                                        static_cast<int>(epoll_events.size()), -1);
    for (int i = 0; i < event_ready_count; ++i) {
      if (epoll_events.at(i).data.fd == listenfd) {
        auto [addr, connfd] = Accept(listenfd);
        fmt::println("client {}: {}", modern_inet_ntop_v4(addr.sin_addr),
                     ntohs(addr.sin_port));
        epoll_event event;
        event.data.fd = connfd;
        event.events = EPOLLIN | EPOLLERR | EPOLLRDHUP;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event)) {
          fmt::println(stderr, "listen event register failed");
          continue;
        }
        epoll_events.push_back(event);
      } else {
        auto event = epoll_events.at(i);
        auto pfd = event.data.fd;
        if (event.events & EPOLLIN) {
          auto lenrecv = recv(event.data.fd, buf.data(), buf.size() - 1, 0);
          if (lenrecv > 0) {
            std::printf("data recv: %s\n", buf.data());
            event.events |= EPOLLOUT;
            epoll_ctl(epoll_fd, EPOLL_CTL_MOD, pfd, &event);
            epoll_events.at(i) = event;
          }
          if (lenrecv == 0) {
            std::printf("conn closed");
            close(event.data.fd);
            remove_from_epoll_events(event.data.fd);
            continue;
          }
        }
        if (event.events & EPOLLRDHUP || event.events & EPOLLERR) {
          close(pfd);
          remove_from_epoll_events(pfd);
        }
        if (event.events & EPOLLOUT) {
          std::string msg{"data received: good luck"};
          auto send_code = send(pfd, msg.data(), msg.length(), 0);
          if (send_code) {
            remove_from_epoll_events(pfd);
            close(pfd);
            continue;
          }
        }
      }
    }
  }
  if (close(epoll_fd)) {
    fmt::println(stderr, "Failed to close epoll file descriptor\n");
    return 1;
  }

  return 0;
}