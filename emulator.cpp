#include "emulator.h"
#include "runtime/interpreter.h"
#include <vector>

int main()
{
    Interpreter<MemoryManager> interpreter;

    std::string path_to_elf = "";
    interpreter.LoadElf(path_to_elf);

    interpreter.Run();

    return 0;
}