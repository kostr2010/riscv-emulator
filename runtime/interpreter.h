#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
#include "utils/ins.h"
#include "utils/isa.h"
#include "utils/macros.h"
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

    inline void SetRegVal(const size_t reg, const int32_t value)
    {
        if (reg == Register::ZERO) {
            return;
        }
        registers_[reg] = value;
    }

    inline int32_t GetRegVal(const size_t reg) const
    {
        return registers_[reg];
    }

  private:
    void UpdatePc()
    {
        switch (is_jump_ins_) {
        case 1:
            is_jump_ins_ = 0;
            break;
        case 0:
            pc_ += 4;
            break;
        }
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

    uint32_t pc_{};
    std::vector<Ins> program_ = {};
    Ins* curr_ins_ = nullptr;
    int32_t imm_{};
    uint32_t rd_{};
    uint32_t rs1_{};
    uint32_t rs2_{};
    bool is_jump_ins_ = 0;
    std::array<int32_t, Register::REGISTERS_COUNT> registers_ = {};
    Err err_ = Err();
};

#endif