#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// S

TEST(InsCreationTest_S, SB)
{
    Ins ins = Ins::MakeIns_SB(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SB(55, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010110001011000101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 55);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}

TEST(InsCreationTest_S, SH)
{
    Ins ins = Ins::MakeIns_SH(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SH(119, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000110110001011001101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}

TEST(InsCreationTest_S, SW)
{
    Ins ins = Ins::MakeIns_SW(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000010000000100011);
    int32_t imm = 0;
    uint32_t rs2 = 0;
    uint32_t rs1 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);

    ins = Ins::MakeIns_SW(119, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000110110001011010101110100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 119);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 12);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
}
