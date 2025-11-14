cmake_minimum_required(VERSION 3.25)

set(REPO_NAME C_Examples)

project(${REPO_NAME} C CXX)

set(C_EXAMPLES 
    "./src/connect_device_simple.c"
    "./src/build_instance_simple.c"
    "./src/discover_devices_simple.c"
    "./src/signals_from_device_simple.c")

add_subdirectory(external)

foreach(src ${C_EXAMPLES})
    get_filename_component(exec_name ${src} NAME_WE)
    add_executable(${exec_name} ${src})
    target_link_libraries(${exec_name} PRIVATE opendaq)
endforeach()