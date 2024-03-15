project(utils)
add_library(utils net.h net.cpp)
target_link_libraries(utils PUBLIC absl_status absl_statusor)
target_include_directories(${PROJECT_NAME} INTERFACE .)