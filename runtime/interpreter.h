#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
#include "mem/mem.h"
#include "mem/memory_interface.h"
#include "mem/regfile.h"
#include "utils/elfreader.h"
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

    // Interpreter(const std::vector<Ins>& program) : program_(program)
    // {
    //     SetHostEndianness();

    //     if (!program.empty()) {
    //         curr_ins_ = &(program_[PCToIndex(pc_)]);
    //     }
    // }

    void Run()
    {
        assert(is_elf_loaded);

        pc_ = host_entrypoint_;

        int counter = 0;
        while (pc_ != RegFile::GPR::ZERO) {
            if (counter == 10)
                break;
            FetchIns();
            HandleIns();
            UpdatePc();
            counter++;
        }

        if (err_.err_type_ != Err::ErrType::NONE) {
            PrintError();
            exit(1);
        }
    }

    static uint32_t ReverseBytes32(uint32_t value)
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

    static uint16_t ReverseBytes16(uint16_t value)
    {
        uint8_t b1 = 0, b2 = 0;
        uint8_t mask = 0x000000FF;
        b2 = (value >> 8) & mask;
        b1 = (value >> 0) & mask;

        return ((uint16_t)b1 << 8) + b2;
    }

    void RunLoader(ElfFile& elf_file)
    {
        std::vector<std::pair<uint32_t*, uint32_t> >& elf_raw =
            elf_file.GetRaw();

        // now we load only .data section, likely it will be changed later
        std::cout << "elf raw size is " << elf_raw.size() << "\n";
        std::cout << "section size is " << elf_raw[0].second << "\n";
        assert(elf_raw.size() == 1);
        MemManager::Write(USER_SPACE_BEGIN,
                          reinterpret_cast<uint8_t*>(elf_raw[0].first),
                          elf_raw[0].second);

        MemoryManager::MemEntry entry = { USER_SPACE_BEGIN,
                                          USER_SPACE_BEGIN + elf_raw[0].second,
                                          MEM_EXEC | MEM_READ };
        MemManager::AddMemMapEntry(entry);

        host_entrypoint_ = elf_file.GetHostEntrypoint();
        InitStack(USER_SPACE_BEGIN + elf_raw[0].second);

        is_elf_loaded = true;
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

    void FetchIns()
    {
        uint32_t vaddr = USER_SPACE_BEGIN + pc_ - host_entrypoint_;
        uint32_t ins_raw = 0;
        assert(
            MemManager::Read(vaddr, reinterpret_cast<uint8_t*>(&ins_raw), 4));
        std::cout << "Fetched raw ins " << ins_raw << "\n";
        curr_ins_ = Ins(ins_raw);

        std::cout << curr_ins_.ToString() << "\n";
    }

    void InitStack(uint32_t start_vaddr)
    {
        // subtraction of 3 is needed for 4 byte allignment
        uint32_t stack_root = VM_SPACE_END - 3;
        MemoryManager::MemEntry entry = { start_vaddr, stack_root,
                                          MEM_WRITE | MEM_READ };
        MemManager::AddMemMapEntry(entry);

        MemManager::SetGPR(RegFile::GPR::SP, stack_root);
    }

    // inline size_t PCToIndex(const uint32_t pc) const
    // {
    //     return pc / 4;
    // }

    inline void SetError(const Err::ErrType& err_type, const std::string& msg)
    {
        err_ = Err(err_type, msg, pc_, curr_ins_);
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

    // interpreter_state_
    bool is_host_big_endian = true;
    bool is_elf_big_endian = true;
    bool is_elf_loaded = false;

    uint32_t pc_{};
    uint32_t host_entrypoint_{};
    // std::vector<Ins> program_ = {};
    Ins curr_ins_{};
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