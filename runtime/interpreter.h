#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED

#include "err.h"
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

    uint32_t Run()
    {
        assert(is_elf_loaded);

        std::cout << "entrypoint is " << host_entrypoint_ << "\n";

        pc_ = host_entrypoint_;

        const auto RA = unsigned(MemManager::GetGPR(RegFile::GPR::X1));

        while (pc_ != RA) {
            // std::cout << "RA:" << RA << "\n";
            // std::cout << "pc_:" << pc_ << "\n";

            std::cout << "fetching ins\n";
            FetchIns();
            std::cout << "fetched ins\n";
            HandleIns();
            std::cout << "handled ins\n";
            UpdatePc();
            std::cout << "updated pc\n";
        }

        if (err_.err_type_ != Err::ErrType::NONE) {
            PrintError();
            exit(1);
        }

        return MemManager::GetGPR(RegFile::GPR::A0);
    }

    void RunLoader(ElfFile& elf_file)
    {
        uint8_t* elf_raw = elf_file.GetRaw();
        uint32_t size = elf_file.GetSize();

        MemManager::Write(USER_SPACE_BEGIN, elf_raw, size);

        host_entrypoint_ = elf_file.GetHostEntrypoint();
        elf_start_addr_ = elf_file.GetElfStartAddr();
        is_elf_big_endian = elf_file.IsElfBigEndian();

        InitStack();

        MemManager::SetGPR(RegFile::GPR::X1, 0);

        is_elf_loaded = true;
    }

    void RunLoader(std::vector<Ins> program, bool is_big_endian = true)
    {
        is_elf_big_endian = is_big_endian;

        int32_t* inss = (int32_t*)calloc(program.size(), sizeof(int32_t));
        for (size_t i = 0; i < program.size(); ++i) {
            inss[i] = program[i].GetInsRaw();
        }

        MemManager::Write(USER_SPACE_BEGIN, reinterpret_cast<uint8_t*>(inss),
                          program.size() * 4);

        host_entrypoint_ = 0;
        elf_start_addr_ = 0;

        InitStack();

        MemManager::SetGPR(RegFile::GPR::X1, 4 * program.size());

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

#define OPLIST(ins, fmt, opcode, is_funct7, funct7, is_funct3, funct3,        \
               is_imm_11_6, imm_11_6, mnm)                                    \
    bool HandleIns_##ins();
    INSTRUCTION_LIST(OPLIST)
#undef OPLIST

    void FetchIns()
    {
        uint32_t vaddr = pc_ + USER_SPACE_BEGIN - elf_start_addr_;
        uint32_t ins_raw = 0;
        assert(
            MemManager::Read(vaddr, reinterpret_cast<uint8_t*>(&ins_raw), 4));
        curr_ins_ = Ins(ins_raw);

        for (int i = 0; i < 32; ++i) {
            std::cout << "x" << i << " " << MemManager::GetGPR(i) << "\n";
        }

        std::cout << std::hex << pc_ << " " << curr_ins_.ToString() << "\n"
                  << std::flush << std::dec;
    }

    void InitStack()
    {
        // subtraction of 3 is needed for 4 byte allignment
        uint32_t stack_root =
            VM_SPACE_END - 3 - (USER_SPACE_BEGIN - elf_start_addr_);

        MemManager::SetGPR(RegFile::GPR::SP, stack_root);
    }

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

    bool is_host_big_endian = true;
    bool is_elf_big_endian = true;
    bool is_elf_loaded = false;

    uint32_t pc_{};
    uint32_t host_entrypoint_{};
    uint32_t elf_start_addr_{};

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