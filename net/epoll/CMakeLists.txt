project(net_epoll)

# add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_simple_server simple_epoll_server.cpp)
target_link_libraries(${PROJECT_NAME}_simple_server PRIVATE utils)

target_link_libraries(${PROJECT_NAME}_simple_server PRIVATE fmt::fmt)
