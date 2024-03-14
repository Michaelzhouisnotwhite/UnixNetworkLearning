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
  sockaddr_in addr;
  ZeroMem(&addr);
  int ret = 0;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port);

  int listenfd = socket(PF_INET, SOCK_STREAM, 0);
  assert(listenfd >= 0);
  ret = bind(listenfd, (sockaddr*)&addr, sizeof(addr));
  assert(ret != -1);
  ret = listen(listenfd, 50);
  assert(ret != -1);
  return listenfd;
}

auto Accept(int listenfd) -> std::pair<sockaddr_in, int> {
  sockaddr_in client_addr;
  socklen_t client_addrlen = sizeof(client_addr);
  int connfd = accept(listenfd, (sockaddr*)&client_addr, &client_addrlen);
  if (connfd < 0) {
    std::cout << "errno is:" << errno << "\n";
    close(listenfd);
    assert(connfd);
  }
  return {client_addr, connfd};
}

void Send2Server(std::string const& host,
                 const int port,
                 const std::string& message,
                 bool keepalive) {
  sockaddr_in addr;
  ZeroMem(&addr);
  int ret = 0;
  addr.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &addr.sin_addr);
  addr.sin_port = htons(port);

  int sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if(connect(sockfd, (sockaddr*)&addr, sizeof(addr))!=0){
    
  };
}