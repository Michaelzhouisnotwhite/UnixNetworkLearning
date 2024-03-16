project(test)
file(GLOB srcs *.cpp)

foreach (src ${srcs})
    get_filename_component(filename ${src} NAME_WLE)
    add_executable(${filename} ${src})
    target_link_libraries(${filename} PRIVATE utils)

endforeach ()
