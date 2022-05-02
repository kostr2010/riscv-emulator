cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" CACHE STRING "Default CXX options" FORCE)
set(CMAKE_CXX_STANDARD 20)

project(simulator CXX)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(modules)
add_subdirectory(utils)

add_executable(emulate emulator.cpp)

target_include_directories(emulate PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(emulate utils)
target_link_libraries(emulate modules)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS
    ${TEST_FOLDER}/tests.cpp
    )

add_executable(tests ${TESTS})
target_link_libraries(tests utils modules ${GTEST_LIBRARIES} pthread)
target_include_directories(tests PRIVATE ${PROJECT_SOURCE_DIR})
