cmake_minimum_required(VERSION 3.27)
project(UnixNetworkLearning)

set(CMAKE_CXX_STANDARD 17)


include(FetchContent)

FetchContent_Declare(
  absl
  GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
  OVERRIDE_FIND_PACKAGE
)
set(ABSL_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ABSL_BUILD_TEST_HELPERS OFF CACHE BOOL "" FORCE)
set(ABSL_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(absl)
add_subdirectory(net/select)
add_subdirectory(utils)
add_subdirectory(test)
