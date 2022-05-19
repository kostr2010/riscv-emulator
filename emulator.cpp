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

    // for (uint32_t i = 0; i < elf_file.GetExecSectionNum(); ++i) {
    //     std::cout << "Section " << i << "\n";
    //     elf_file.DumpExecSection(i);
    //     std::cout << "\n";
    // }

    interpreter.LoadElf(elf_file);

    interpreter.Run();

    return 0;
}