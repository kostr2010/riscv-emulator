#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// J

TEST(InsCreationTest_J, JAL)
{
    Ins ins = Ins::MakeIns_JAL(0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000001101111);
    int32_t imm = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_JAL(22, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000001011000000000001011101111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 22);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);

    ins = Ins::MakeIns_JAL(-22, 5);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000001011000000000001011101111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -22);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 5);
}
