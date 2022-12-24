set(TEST_GEN_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/generator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/testgen.cpp
)

add_executable(testgen ${TEST_GEN_SOURCES})
target_link_libraries(testgen utils mem ${GTEST_LIBRARIES} pthread)
target_include_directories(testgen PRIVATE ${PROJECT_SOURCE_DIR})
