# Configure CMAKE
cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.14)

# Project
project(
    AppImageServices
    VERSION 0.2.0
    LANGUAGES C CXX
    DESCRIPTION "Language agnostic implementation of the AppImage user experience"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)
add_subdirectory(res)
add_subdirectory(tests)
