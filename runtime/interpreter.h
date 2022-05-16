#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
#include "mem/memory_interface.h"
#include "utils/ins.h"
#include "utils/isa.h"
#include "utils/macros.h"

#include <array>
#include <string>
#include <type_traits>
#include <vector>

template <class MemManager>
class Interpreter : public MemManager
{
    static_assert(std::is_base_of<MemoryInterface, MemManager>::value == true);

  public:
    Interpreter()
    {
        // init
    }
    // LOVE YA
    Interpreter(const std::vector<Ins>& program) : program_(program)
    {
        if (!program.empty()) {
            curr_ins_ = &(program_[PCToIndex(pc_)]);
        }
    }

    void Run()
    {
        // We need path to elf
        std::string path = "PathToELf";
        int fd = open(path, O_RDONLY)

                     program_loader.SetFD(fd);
        uint32_t entrypoint = program_loader.LoadElf32IntoMemory();
        pc_ = entrypoint;
        for (;;) { // Loop till last command
            uint32_t rawInstruction = GetRawInstr();
            // HandleRawIns(rawInstruction);
        }

        // Not updated during jumps, make change pc in instructions
        // while (PCToIndex(pc_) < program_.size() && HandleIns()) {
        //     // UpdatePc(); // No need in it
        // }

        if (err_.err_type_ != Err::ErrType::NONE) {
            PrintError();
            exit(1);
        }
    }

  private:
    void UpdatePc();
    bool HandleIns();
    bool HandleInsOperands_R();
    bool HandleInsOperands_I();
    bool HandleInsOperands_S();
    bool HandleInsOperands_B();
    bool HandleInsOperands_U();
    bool HandleInsOperands_J();
    bool HandleInsOperands_NOP();

#define OPLIST(ins, fmt, opcode, is_funct7, funct7, is_funct3, funct3, mnm)   \
    bool HandleIns_##ins();
    INSTRUCTION_LIST(OPLIST)
#undef OPLIST

    inline size_t PCToIndex(const uint32_t pc) const
    {
        return pc / 4;
    }

    inline void SetError(const Err::ErrType& err_type, const std::string& msg)
    {
        err_ = Err(err_type, msg, pc_, program_[PCToIndex(pc_)]);
    }

    inline void PrintError() const
    {
        std::cout << err_.ToString() << "\n";
    }

    uint32_t GetRawInstr()
    {
        ASSERT(pc_ % 4 == 0);
        return *reinterpret_cast<uint32_t*>(program_loader.Translate(pc_));
    }

    utils::ReadElf program_loader{};

    // interpreter_state_
    uint32_t pc_{};
    std::vector<Ins> program_ = {};
    Ins* curr_ins_ = nullptr;
    int32_t imm_{};
    uint32_t rd_{};
    uint32_t rs1_{};
    uint32_t rs2_{};
    bool is_jump_ins_ = 0;
    Err err_ = Err();
};

#include "handlers_b.hpp"
#include "handlers_i.hpp"
#include "handlers_j.hpp"
#include "handlers_nop.hpp"
#include "handlers_r.hpp"
#include "handlers_s.hpp"
#include "handlers_u.hpp"
#include "interpreter.hpp"

#endif