cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" CACHE STRING "Default CXX options" FORCE)
set(CMAKE_CXX_STANDARD 20)

project(simulator CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(modules)
add_subdirectory(utils)
add_subdirectory(mem)
# add_subdirectory(runtime)

add_executable(emulator emulator.cpp)

target_include_directories(emulator PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(emulator utils)
target_link_libraries(emulator modules)
target_link_libraries(emulator mem)
