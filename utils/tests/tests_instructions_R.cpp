#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// R

TEST(InsCreationTest_R, ADD)
{
    Ins ins = Ins::MakeIns_ADD(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000000000000110011);

    ins = Ins::MakeIns_ADD(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100000010110110011);
}

TEST(InsCreationTest_R, SUB)
{
    Ins ins = Ins::MakeIns_SUB(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000000000000000000000110011);

    ins = Ins::MakeIns_SUB(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000110101100000010110110011);
}

TEST(InsCreationTest_R, SLL)
{
    Ins ins = Ins::MakeIns_SLL(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000001000000110011);

    ins = Ins::MakeIns_SLL(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100001010110110011);
}

TEST(InsCreationTest_R, SLT)
{
    Ins ins = Ins::MakeIns_SLT(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000010000000110011);

    ins = Ins::MakeIns_SLT(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100010010110110011);
}

TEST(InsCreationTest_R, SLTU)
{
    Ins ins = Ins::MakeIns_SLTU(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000011000000110011);

    ins = Ins::MakeIns_SLTU(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100011010110110011);
}

TEST(InsCreationTest_R, XOR)
{
    Ins ins = Ins::MakeIns_XOR(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000100000000110011);

    ins = Ins::MakeIns_XOR(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100100010110110011);
}

TEST(InsCreationTest_R, SRL)
{
    Ins ins = Ins::MakeIns_SRL(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000101000000110011);

    ins = Ins::MakeIns_SRL(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100101010110110011);
}

TEST(InsCreationTest_R, SRA)
{
    Ins ins = Ins::MakeIns_SRA(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000000000000101000000110011);

    ins = Ins::MakeIns_SRA(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b01000000110101100101010110110011);
}

TEST(InsCreationTest_R, OR)
{
    Ins ins = Ins::MakeIns_OR(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000110000000110011);

    ins = Ins::MakeIns_OR(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100110010110110011);
}

TEST(InsCreationTest_R, AND)
{
    Ins ins = Ins::MakeIns_AND(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000000000000111000000110011);

    ins = Ins::MakeIns_AND(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000000110101100111010110110011);
}
