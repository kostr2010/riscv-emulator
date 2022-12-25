add_library(utils SHARED ins.cpp)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_INSTRUCTIONS 
    ${TEST_FOLDER}/tests_utils.cpp 
    ${TEST_FOLDER}/tests_instructions_M.cpp
    ${TEST_FOLDER}/tests_instructions_R.cpp
    ${TEST_FOLDER}/tests_instructions_I.cpp
    ${TEST_FOLDER}/tests_instructions_S.cpp
    ${TEST_FOLDER}/tests_instructions_B.cpp
    ${TEST_FOLDER}/tests_instructions_U.cpp
    ${TEST_FOLDER}/tests_instructions_J.cpp
    )

set(GTEST_INCLUDE_DIR third-party/googletest/googletest/include)

add_executable(tests_utils ${TESTS_INSTRUCTIONS})
target_link_libraries(tests_utils utils gtest pthread)
target_include_directories(tests_utils PRIVATE ${PROJECT_SOURCE_DIR} SHARED ${GTEST_INCLUDE_DIR}) 