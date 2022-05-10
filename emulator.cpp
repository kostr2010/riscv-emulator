#include "emulator.h"
#include "runtime/interpreter.h"
#include <vector>

int main()
{
    std::vector<Ins> program;

    Interpreter interpreter(program);

    return 0;
}