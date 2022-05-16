add_library(modules SHARED mem.cpp)

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)

set(TESTS_MODULES
    ${TEST_FOLDER}/tests_modules.cpp
    )

add_executable(tests_modules ${TESTS_MODULES})
target_link_libraries(tests_modules modules ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_modules PRIVATE ${PROJECT_SOURCE_DIR})