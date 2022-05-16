#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
#include "mem/memory_interface.h"
#include "mem/regfile.h"
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
        SetHostEndianness();
    }

    Interpreter(const std::vector<Ins>& program) : program_(program)
    {
        SetHostEndianness();

        if (!program.empty()) {
            curr_ins_ = &(program_[PCToIndex(pc_)]);
        }
    }

    void Run()
    {
        // We need path to elf
        // std::string path = "PathToELf";
        // int fd = open(path, O_RDONLY)

        //              program_loader.SetFD(fd);
        // uint32_t entrypoint = program_loader.LoadElf32IntoMemory();
        // pc_ = entrypoint;
        // for (;;) { // Loop till last command
        //     uint32_t rawInstruction = GetRawInstr();
        //     // HandleRawIns(rawInstruction);
        // }

        while (PCToIndex(pc_) < program_.size() && HandleIns()) {
            UpdatePc();
        }

        if (err_.err_type_ != Err::ErrType::NONE) {
            PrintError();
            exit(1);
        }
    }

    static uint32_t ReverseBits32(uint32_t value)
    {
        uint8_t b1 = 0, b2 = 0, b3 = 0, b4 = 0;
        uint8_t mask = 0x000000FF;
        b4 = (value >> 24) & mask;
        b3 = (value >> 16) & mask;
        b2 = (value >> 8) & mask;
        b1 = (value >> 0) & mask;

        return ((uint32_t)b1 << 24) + ((uint32_t)b2 << 16) +
               ((uint32_t)b3 << 8) + b4;
    }

    static uint16_t ReverseBits16(uint16_t value)
    {
        uint8_t b1 = 0, b2 = 0;
        uint8_t mask = 0x000000FF;
        b2 = (value >> 8) & mask;
        b1 = (value >> 0) & mask;

        return ((uint16_t)b1 << 8) + b2;
    }

    void LoadElf(const std::string& path)
    {
        std::cout << path;
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

    inline void SetHostEndianness()
    {
        uint32_t i = 1;
        uint8_t* p = reinterpret_cast<uint8_t*>(&i);
        is_host_big_endian = (p[0] == 1) ? false : true;
    }

    // uint32_t GetRawInstr()
    // {
    //     ASSERT(pc_ % 4 == 0);
    //     return *reinterpret_cast<uint32_t*>(program_loader.Translate(pc_));
    // }

    // ReadElf program_loader{};

    // interpreter_state_
    bool is_host_big_endian = true;
    bool is_elf_big_endian = true;

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