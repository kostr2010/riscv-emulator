#include "runtime/tests/macros.h"
#include "runtime/interpreter.h"
#include "mem/mem.h"
#include <gtest/gtest.h>

TEST_RR_OP(MUL, 8, 23, 29, 64, 8, 8)
TEST_RR_OP(MUL, 8, 23, 29, -43, -1, 43)
TEST_RR_OP(MULH, 8, 23, 29, 0, 8, 8)
TEST_RR_OP(MULH, 8, 23, 29, 0x1, 0xFFFF01, 0x2)

TEST_RR_OP(DIV, 8, 23, 29, 9, 72, 8)
TEST_RR_OP(DIV, 8, 23, 29, -9, -73, 8)
TEST_RR_OP(DIVU, 8, 23, 29, 9, 72, 8)
TEST_RR_OP(DIVU, 8, 23, 29, 9, 73, 8)

TEST_RR_OP(REM, 8, 23, 29, 1, 73, 8)
TEST_RR_OP(REM, 8, 23, 29, -1, -73, 8)
TEST_RR_OP(REMU, 8, 23, 29, 0, 72, 8)
TEST_RR_OP(REMU, 8, 23, 29, 1, 73, 8)