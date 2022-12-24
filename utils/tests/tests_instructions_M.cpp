#include "utils/ins.h"
#include <bitset>
#include <gtest/gtest.h>
#include <iostream>

// M

TEST(InsCreationTest_M, MUL)
{
    Ins ins = Ins::MakeIns_MUL(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010000000000000000000110011);

    ins = Ins::MakeIns_MUL(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010110101100000010110110011);
}

TEST(InsCreationTest_M, DIV)
{
    Ins ins = Ins::MakeIns_DIV(0, 0, 0);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010000000000100000000110011);

    ins = Ins::MakeIns_DIV(13, 12, 11);
    ASSERT_EQ(ins.GetInsRaw(), 0b00000010110101100100010110110011);
}
