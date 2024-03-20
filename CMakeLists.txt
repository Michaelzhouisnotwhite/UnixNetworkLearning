cmake_minimum_required(VERSION 3.20)
project(UnixNetworkLearning)

set(CMAKE_CXX_STANDARD 17)

include(cmake/3rd.cmake)
add_subdirectory(net/poll)
add_subdirectory(net/select)
add_subdirectory(net/epoll)
add_subdirectory(utils)
add_subdirectory(test)