add_library(mem SHARED mem.cpp)
target_include_directories(mem PRIVATE ${PROJECT_SOURCE_DIR})

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_MEM
    ${TEST_FOLDER}/tests_mem.cpp
    ${TEST_FOLDER}/tests_read_write.cpp
    )

add_executable(tests_mem ${TESTS_MEM})
target_link_libraries(tests_mem mem ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_mem PRIVATE ${PROJECT_SOURCE_DIR})