find_package (Python3 COMPONENTS Interpreter Development)

#add_library(runtime SHARED 
#            interpreter.cpp
#            handlers_b.cpp
#            handlers_i.cpp
#            handlers_j.cpp
#            handlers_r.cpp
#            handlers_s.cpp
#            handlers_u.cpp
#            )
#target_link_libraries(runtime mem utils ${GTEST_LIBRARIES} pthread)
#target_include_directories(runtime PRIVATE ${PROJECT_SOURCE_DIR})

set(TEST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(GEN_TESTS_PY ${TEST_FOLDER}/gen_tests.py)
set(TESTS_I_DIR_SRC ${PROJECT_SOURCE_DIR}/third-party/riscv-arch-test/riscv-test-suite/rv32i_m/I/src/)
set(TESTS_I_DIR_DST ${CMAKE_CURRENT_BINARY_DIR})

set(TESTS_I 
    sltu-01
    sll-01
    or-01
    lw-align-01
    sw-align-01
    sb-align-01
    bge-01
    srai-01
    lh-align-01
    jal-01
    sub-01
    xor-01
    addi-01
    bgeu-01
    sra-01
    srli-01
    lbu-align-01
    fence-01
    beq-01
    ori-01
    add-01
    sltiu-01
    and-01
    bne-01
    slt-01
    andi-01
    lb-align-01
    slli-01
    bltu-01
    lhu-align-01
    xori-01
    slti-01
    blt-01
    sh-align-01
    srl-01
    lui-01
    auipc-01
    jalr-01
    )

foreach(INS IN LISTS TESTS_I)
    add_custom_command(OUTPUT ${TESTS_I_DIR_DST}/${INS}.cpp
                       COMMAND ${Python3_EXECUTABLE} ${GEN_TESTS_PY} ${TESTS_I_DIR_SRC}/${INS}.S ${TESTS_I_DIR_DST}/${INS}.cpp
                       WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                       DEPENDS ${GEN_TESTS_PY}
                       )
endforeach()

set(TESTS_RUNTIME 
    ${TEST_FOLDER}/tests_interpreter.cpp
)

foreach(INS IN LISTS TESTS_I)
    list(APPEND TESTS_RUNTIME ${TESTS_I_DIR_DST}/${INS}.cpp)
endforeach()

add_executable(tests_interpreter ${TESTS_RUNTIME})
target_link_libraries(tests_interpreter utils mem ${GTEST_LIBRARIES} pthread)
target_include_directories(tests_interpreter PRIVATE ${PROJECT_SOURCE_DIR})