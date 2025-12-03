cmake_minimum_required(VERSION 3.25)
set(REPO_NAME c_examples)
set(REPO_OPTION_PREFIX EXAMPLE_MODULE)

project(${REPO_NAME} VERSION 1.0.0)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MESSAGE_CONTEXT ${REPO_NAME})
set(CMAKE_MESSAGE_CONTEXT_SHOW ON CACHE BOOL "Show CMake message context")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_definitions(-DFMT_HEADER_ONLY)

include(CommonUtils)
setup_repo(${REPO_OPTION_PREFIX})

add_subdirectory(external)
add_subdirectory(examples)
