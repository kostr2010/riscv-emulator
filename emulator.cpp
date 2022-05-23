#include "mem/mem.h"
#include "runtime/interpreter.h"
#include "utils/elfreader.h"
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: ./emulator <path to elf file>\n";
        std::exit(1);
    }

    Interpreter<MemoryManager> interpreter;

    ElfFile elf_file(argv[1]);

    interpreter.RunLoader(elf_file);

    uint32_t result = interpreter.Run();

    std::cout << "Result: " << result << "\n";

    return 0;
}