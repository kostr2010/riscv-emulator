set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(mem SHARED mem.cpp)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_MEM
    ${TEST_FOLDER}/tests_mem.cpp
    )

add_executable(tests_mem ${TESTS_MEM})
target_link_libraries(tests_mem mem ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_mem PRIVATE ${PROJECT_SOURCE_DIR})