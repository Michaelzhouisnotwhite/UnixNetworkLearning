#include "net.h"

int main(int argc, char* argv[]) {
  auto status = Send2Server("0.0.0.0", 8089, "Hello From Client");
  if (!status.ok()) {
    std::cout << status.status() << "\n";
  }else {
    std::puts(status->c_str());
  }

  return 0;
}