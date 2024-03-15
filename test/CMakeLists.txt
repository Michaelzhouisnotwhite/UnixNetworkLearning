project(test)

add_executable(test_send test_send.cpp)
target_link_libraries(test_send PRIVATE utils)