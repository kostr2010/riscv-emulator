#include "mem/mem.h"
#include "runtime/interpreter.h"
#include "utils/ins.h"

#include "utils/elfreader.h"
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: ./emulator <path to elf file>\n";
        std::exit(1);
    }

    Interpreter<MemoryManager> interpreter;

    ElfFile elf_file(argv[1]);

    interpreter.LoadElf(elf_file);

    interpreter.Run();

    return 0;
}