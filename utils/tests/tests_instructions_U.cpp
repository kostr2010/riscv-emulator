#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// U

TEST(InsCreationTest_U, LUI)
{
    Ins ins = Ins::MakeIns_LUI(0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000110111);
    int32_t imm = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LUI(4096, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001001010110111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 4096);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);

    ins = Ins::MakeIns_LUI(-4096, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000000000000001001010110111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -4096);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);
}
