#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "memory_interface.h"
#include "regfile.h"

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <stdlib.h>
#include <unordered_map>

constexpr uint32_t REGION1_ZONE_BEGIN = 0x00100000;
constexpr uint32_t REGION2_ZONE_BEGIN = 0x00300000;
constexpr uint32_t RESERVED2_ZONE_BEGIN = 0x03F00000;
constexpr uint32_t PM_SPACE_END = 0x04000000;

// TODO: re calculate so that N_ENTRIES_FIRST_PT_INNER = MAX_ENTRIES_PT
// TODO: tests memory (read-write-read)
// TODO: API for LOAD / STORE

// TODO: attributes / exceptions
// TODO: run simulation
// TODO:

constexpr uint32_t KERNEL_SPACE_END = 0x00200000;
constexpr uint32_t USER_SPACE_BEGIN = 0xFC400000;
constexpr uint32_t VM_SPACE_END = 0xFFFFFFFF;

constexpr uint32_t N_ENTRIES_PT_OUTER = 16;
constexpr uint32_t N_ENTRIES_FIRST_PT_INNER = 512;
constexpr uint32_t MAX_ENTRIES_PT = 1024; // 1024 because of 10 bit address

constexpr uint32_t PT_OUTER_IDX_UPPER_REGION_START = 1009;

constexpr uint32_t N_PAGES = N_ENTRIES_PT_OUTER * MAX_ENTRIES_PT;
constexpr uint32_t PAGE_SIZE = 4 * 1024;
constexpr uint32_t TOTAL_RAM = N_PAGES * PAGE_SIZE;

constexpr uint32_t MEM_END = N_PAGES * PAGE_SIZE - 1; // 0x003FFFFF

// 64 MB - total phisical memory space
//
// physical memory layout
/*---------------------------------- 0x00000000
|              Reserved 1 1MB
|----------------------------------- REGION1_ZONE_BEGIN
|              PM Region 1 2MB
|----------------------------------- REGION2_ZONE_BEGIN
|              PM Region 2 60MB
|----------------------------------- RESERVED2_ZONE_BEGIN
|              Reserved 2 1MB
|----------------------------------- PM_SPACE_END
*/

// virtual memory map
/*----------------------------------    0x00000000    |-------------
|              Kernel space 2MB                         VM Region 1
|----------------------------------- KERNEL_SPACE_END |-------------
|               RESERVED
|----------------------------------- USER_SPACE_BEGIN |-------------
|               Text (ELF)
|-----------------------------------
|               Global data
|-----------------------------------
|            stack (grows down)                         VM Region 2
|                  |
|-----------------------------------
|                  |
|            Heap (grows up)
|-----------------------------------    VM_SPACE_END  |-------------
*/

class MemoryManager : public MemoryInterface
{
    class Uint32_t_Ptr
    {
      public:
        static const uint32_t MASK_PT_OUTER = 0xFFC00000;
        static const uint32_t MASK_PT_INNER = 0x003FF000;
        static const uint32_t MASK_OFFSET = 0x00000FFF;

        Uint32_t_Ptr(uint32_t ptr)
        {
            raw = ptr;
        }

        inline void ZeroOffset()
        {
            raw &= ~MASK_OFFSET;
        }

        inline void NextPage()
        {
            ZeroOffset();
            raw += MASK_OFFSET + 1;
        }

        inline uint32_t PTIdxOuter() const
        {
            return raw & MASK_PT_OUTER >> 22;
        }

        inline uint32_t PTIdxInner() const
        {
            return raw & MASK_PT_INNER >> 12;
        }

        inline uint32_t Offset() const
        {
            return raw & MASK_OFFSET;
        }

        // private:
        uint32_t raw;
    };

  public:
    MemoryManager()
    {
        mem_ = (uint8_t*)calloc(N_PAGES * PAGE_SIZE, sizeof(uint8_t));
        assert(mem_ != nullptr);

        InitPageTable();
    }

    bool Read(const uint32_t vaddr, uint8_t* buf,
              uint32_t count) const override
    {
        assert(buf != nullptr);
        assert(vaddr + count < MEM_END);

        // TODO: check access rights

        Uint32_t_Ptr ptr(vaddr);
        uint32_t buf_offset = 0;
        uint32_t bytes_to_read = 0;
        while (count) {
            bytes_to_read = (count > PAGE_SIZE - ptr.Offset())
                                ? PAGE_SIZE - ptr.Offset()
                                : count;
            memcpy(buf + buf_offset, VadrToPadr(ptr), bytes_to_read);

            ptr.NextPage();

            buf_offset += bytes_to_read;
            count -= bytes_to_read;
        }

        return true;
    }

    bool Write(const uint32_t vaddr, uint8_t* buf, uint32_t count) override
    {
        assert(buf != nullptr);
        assert(vaddr + count < MEM_END);

        // TODO: check access rights

        Uint32_t_Ptr ptr(vaddr);
        uint32_t buf_offset = 0;
        uint32_t bytes_to_write = 0;
        while (count) {
            bytes_to_write = (count > PAGE_SIZE - ptr.Offset())
                                 ? PAGE_SIZE - ptr.Offset()
                                 : count;
            memcpy(VadrToPadr(ptr), buf + buf_offset, bytes_to_write);

            ptr.NextPage();

            buf_offset += bytes_to_write;
            count -= bytes_to_write;
        }

        return true;
    }

    inline void SetIntReg(const uint32_t reg, const int32_t value) override
    {
        assert(reg < RegFile::IntRegister::INT_REGISTERS_COUNT);
        if (reg == RegFile::IntRegister::ZERO) {
            return;
        }
        regfile_.int_regs_[reg] = value;
    }

    inline int32_t GetIntReg(const uint32_t reg) const override
    {
        assert(reg < RegFile::IntRegister::INT_REGISTERS_COUNT);
        if (reg == RegFile::IntRegister::ZERO) {
            return 0x0;
        }
        return regfile_.int_regs_[reg];
    }

    inline void SetCSRReg(const uint32_t reg, const int32_t value) override
    {
        assert(reg < RegFile::CSRRegister::CSR_REGISTERS_COUNT);
        regfile_.csr_regs_[reg] = value;
    }

    inline int32_t GetCSRReg(const uint32_t reg) const override
    {
        assert(reg < RegFile::CSRRegister::CSR_REGISTERS_COUNT);
        return regfile_.csr_regs_[reg];
    }

    inline void SetPc(const int32_t value) override
    {
        regfile_.pc_ = value;
    }

    inline int32_t GetPc() const override
    {
        return regfile_.pc_;
    }

  private:
    void InitPageTable()
    {
        // Region1 memory mapping
        for (size_t i = 0; i < N_ENTRIES_FIRST_PT_INNER; ++i) {
            pt_[0][i] = mem_ + REGION1_ZONE_BEGIN + PAGE_SIZE * i;
        }

        // Region2 memory mapping
        size_t page_num_counter = 0;
        for (size_t i = PT_OUTER_IDX_UPPER_REGION_START; i < MAX_ENTRIES_PT;
             ++i) {
            for (size_t j = 0; j < MAX_ENTRIES_PT; ++j) {
                pt_[i][j] =
                    mem_ + REGION2_ZONE_BEGIN + PAGE_SIZE * page_num_counter;
                ++page_num_counter;
            }
        }

        assert(pt_.size() == N_ENTRIES_PT_OUTER);
    }

    inline uint8_t* VadrToPadr(const Uint32_t_Ptr& ptr) const
    {
        assert(ptr.raw >= USER_SPACE_BEGIN && ptr.raw < VM_SPACE_END);
        assert((ptr.PTIdxOuter() == 0) |
               (ptr.PTIdxOuter() >= PT_OUTER_IDX_UPPER_REGION_START));
        if (ptr.PTIdxOuter() == 0) {
            assert(ptr.PTIdxInner() < N_ENTRIES_FIRST_PT_INNER);
        }

        return pt_.at(ptr.PTIdxOuter()).at(ptr.PTIdxInner()) + ptr.Offset();
    }

    using PageTableInner = std::unordered_map<uint32_t, uint8_t*>;
    using PageTableOuter = std::unordered_map<uint32_t, PageTableInner>;
    PageTableOuter pt_;
    uint8_t* mem_ = nullptr;
    RegFile regfile_ = {};
};

#endif