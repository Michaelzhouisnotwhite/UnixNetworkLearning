project(net_poll)


# add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_simple_server simple_poll_server.cpp)
target_link_libraries(${PROJECT_NAME}_simple_server PRIVATE utils)