set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -g")

add_library(runtime SHARED 
            interpreter.cpp
            handlers_b.cpp
            handlers_i.cpp
            handlers_j.cpp
            handlers_r.cpp
            handlers_s.cpp
            handlers_u.cpp
            )

target_include_directories(runtime PRIVATE ${PROJECT_SOURCE_DIR})

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_RUNTIME
    ${TEST_FOLDER}/tests_interpreter.cpp
    ${TEST_FOLDER}/tests_b.cpp
    ${TEST_FOLDER}/tests_i.cpp
    ${TEST_FOLDER}/tests_j.cpp
    ${TEST_FOLDER}/tests_r.cpp
    ${TEST_FOLDER}/tests_s.cpp
    ${TEST_FOLDER}/tests_u.cpp
    )

add_executable(tests_interpreter ${TESTS_RUNTIME})
target_link_libraries(tests_interpreter mem ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_interpreter PRIVATE ${PROJECT_SOURCE_DIR})