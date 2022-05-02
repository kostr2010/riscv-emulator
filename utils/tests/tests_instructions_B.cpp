#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// B

TEST(InsCreationTest_B, BEQ)
{
    Ins ins = Ins::MakeIns_BEQ(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BEQ(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011000101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BEQ(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011000101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}

TEST(InsCreationTest_B, BNE)
{
    Ins ins = Ins::MakeIns_BNE(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BNE(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011001101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BNE(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011001101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}

TEST(InsCreationTest_B, BLT)
{
    Ins ins = Ins::MakeIns_BLT(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000100000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BLT(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011100101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BLT(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011100101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}

TEST(InsCreationTest_B, BGE)
{
    Ins ins = Ins::MakeIns_BGE(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000101000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BGE(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011101101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BGE(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011101101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}

TEST(InsCreationTest_B, BLTU)
{
    Ins ins = Ins::MakeIns_BLTU(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000110000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BLTU(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011110101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BLTU(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011110101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}

TEST(InsCreationTest_B, BGEU)
{
    Ins ins = Ins::MakeIns_BGEU(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000111000001100011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rs2 = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 0);

    ins = Ins::MakeIns_BGEU(1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01010100010101011111101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);

    ins = Ins::MakeIns_BGEU(-1364, 5, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b11010100010101011111101001100011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -1364);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 11);
    ASSERT_EQ(ins.GetRs2(&rs2), true);
    ASSERT_EQ(rs2, 5);
}
