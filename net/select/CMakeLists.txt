project(net_select)


# add_executable(${PROJECT_NAME} main.cpp)
add_executable(${PROJECT_NAME}_select_accept_connect select_accept_connect.cpp)
add_executable(${PROJECT_NAME}_select_conn select_conn.cpp)
target_link_libraries(${PROJECT_NAME}_select_accept_connect PRIVATE utils)
target_link_libraries(${PROJECT_NAME}_select_conn PRIVATE utils)