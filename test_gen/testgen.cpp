#include <string>

#include "generator.h"
#include "mem/mem.h"
#include "runtime/interpreter.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: ./testgen <inst count>\n";
        return 0;
    }
    if (std::stoi(argv[1]) < 5) {
        std::cout << "inst count should be >= 10\n";
        return 0;
    }

    uint32_t inst_count = std::stoi(argv[1]);
    std::vector<Ins> generated_program;
    generated_program.reserve(inst_count + 1);
    TestGenerator test_gen;

    generated_program.push_back(
        Ins::MakeIns_ADDI(-STACK_SIZE, RegFile::GPR::SP, RegFile::GPR::SP));

    for (int i = 0; i < 5; ++i) {
        generated_program.push_back(test_gen.GenerateIInst());
    }
    inst_count -= 5;
    for (; inst_count > 0; --inst_count) {
        generated_program.push_back(test_gen.GenerateInst());
    }

    Interpreter<MemoryManager> interpreter;
    interpreter.RunLoader(generated_program);
    interpreter.Run();

    return 0;
}
