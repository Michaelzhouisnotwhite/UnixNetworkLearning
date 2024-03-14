project(utils)

add_library(utils net.h net.cpp)
target_link_libraries(utils PUBLIC absl::Status)
target_include_directories(${PROJECT_NAME} INTERFACE .)