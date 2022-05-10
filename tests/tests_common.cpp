#include "emulator.h"
#include <gtest/gtest.h>

TEST(ZERO_TEST, ZERO)
{
    static const std::vector<Ins> program = {
        Ins::MakeIns_ADDI(1, 4, 1),
        Ins::MakeIns_ADDI(-2, 4, 2),
        Ins::MakeIns_ADD(1, 2, 4),
    };

    Interpreter interpreter(program);

    interpreter.Run();
}