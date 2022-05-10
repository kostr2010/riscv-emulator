#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
#include "utils/ins.h"
#include "utils/isa.h"
#include "utils/register.h"

#include <array>
#include <vector>

class Interpreter
{
  public:
    Interpreter(const std::vector<Ins>& program) : program_(program)
    {
        if (!program.empty()) {
            curr_ins_ = &(program_[PCToIndex(pc_)]);
        }
    }

    void Run()
    {
        while (PCToIndex(pc_) < program_.size() && HandleIns()) {
            UpdatePc();
        }

        if (err_.err_type_ != Err::ErrType::NONE) {
            PrintError();
            exit(1);
        }
    }

  private:
    void UpdatePc()
    {
        // switch (PC_SOURCE) {
        // case BRANCH:
        //     pc = imm_;
        // case NONE:
        //     pc++;
        // }

        ++pc_;
    }

    bool HandleIns();
    bool HandleInsOperands_R();
    bool HandleInsOperands_I();
    bool HandleInsOperands_S();
    bool HandleInsOperands_B();
    bool HandleInsOperands_U();
    bool HandleInsOperands_J();
    inline bool HandleInsOperands_NOP()
    {
        return true;
    }

#define OPLIST(ins, fmt, opcode, mnm) bool HandleIns_##ins();
    INSTRUCTION_LIST(OPLIST)
#undef OPLIST

    inline size_t PCToIndex(const uint32_t pc) const
    {
        return pc;
    }

    inline void SetError(const Err::ErrType& err_type, const std::string& msg)
    {
        err_ = Err(err_type, msg, pc_, program_[PCToIndex(pc_)]);
    }

    inline void PrintError() const
    {
        std::cout << err_.ToString() << "\n";
    }

    uint32_t pc_{};
    std::vector<Ins> program_ = {};
    Ins* curr_ins_ = nullptr;
    int32_t imm_{};
    uint32_t rd_{};
    uint32_t rs1_{};
    uint32_t rs2_{};
    std::array<uint32_t, Register::REGISTERS_COUNT> registers_ = {};
    Err err_ = Err();
};

#endif