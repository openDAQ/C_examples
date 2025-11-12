set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.25)

set(REPO_NAME C_Examples)
set(REPO_OPTION_PREFIX C_EXAMPLES)

project(${REPO_NAME} C)

set(C_EXAMPLES 
    "./src/connect_device_simple.c"
    "./src/build_instance_simple.c"
    "./src/discover_devices_simple.c"
    "./src/signals_from_device.c")

add_subdirectory(external)