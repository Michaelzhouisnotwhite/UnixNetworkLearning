#include "net.h"

int main(int argc, char* argv[]) {
  auto status = Send2Server("127.0.0.1", 8088, "abc");
  if (!status.ok()) {
    std::cout << status.status() << "\n";
  }
  return 0;
}