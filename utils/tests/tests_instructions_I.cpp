#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// I

// ARITHMETIC

TEST(InsCreationTest_I_ARITHMETIC, ADDI)
{
    Ins ins = Ins::MakeIns_ADDI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_ADDI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100000010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_ADDI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100000010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, SLTI)
{
    Ins ins = Ins::MakeIns_SLTI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000010000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_SLTI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100010010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_SLTI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100010010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, SLTUI)
{
    Ins ins = Ins::MakeIns_SLTUI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000011000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_SLTUI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100011010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_SLTUI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100011010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, XORI)
{
    Ins ins = Ins::MakeIns_XORI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000100000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_XORI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100100010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_XORI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100100010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, ORI)
{
    Ins ins = Ins::MakeIns_ORI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000110000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_ORI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100110010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_ORI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100110010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, ANDI)
{
    Ins ins = Ins::MakeIns_ANDI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000111000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_ANDI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100111010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_ANDI(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100111010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, SLLI)
{
    Ins ins = Ins::MakeIns_SLLI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_SLLI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100001010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, SRLI)
{
    Ins ins = Ins::MakeIns_SRLI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000101000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_SRLI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100101010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_ARITHMETIC, SRAI)
{
    Ins ins = Ins::MakeIns_SRAI(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000000000000101000000010011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_SRAI(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000110101100101010110010011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

// LOAD

TEST(InsCreationTest_I_LOAD, LB)
{
    Ins ins = Ins::MakeIns_LB(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000000011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LB(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100000010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_LB(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100000010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_LOAD, LH)
{
    Ins ins = Ins::MakeIns_LH(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000000011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LH(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100001010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_LH(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100001010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_LOAD, LW)
{
    Ins ins = Ins::MakeIns_LW(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000010000000000011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LW(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100010010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_LW(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100010010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_LOAD, LBU)
{
    Ins ins = Ins::MakeIns_LBU(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000100000000000011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LBU(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100100010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_LBU(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100100010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

TEST(InsCreationTest_I_LOAD, LHU)
{
    Ins ins = Ins::MakeIns_LHU(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000101000000000011);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_LHU(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100101010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_LHU(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100101010110000011);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}

// JALR

TEST(InsCreationTest_I_JALR, JALR)
{
    Ins ins = Ins::MakeIns_JALR(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000001100111);
    int32_t imm = 0;
    uint32_t rs1 = 0;
    uint32_t rd = 0;
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 0);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 0);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 0);

    ins = Ins::MakeIns_JALR(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100000010111100111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, 13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);

    ins = Ins::MakeIns_JALR(-13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b10000000110101100000010111100111);
    ASSERT_EQ(ins.GetImm(&imm), true);
    ASSERT_EQ(imm, -13);
    ASSERT_EQ(ins.GetRs1(&rs1), true);
    ASSERT_EQ(rs1, 12);
    ASSERT_EQ(ins.GetRd(&rd), true);
    ASSERT_EQ(rd, 11);
}