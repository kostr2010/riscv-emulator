#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

#include "memory_interface.h"
#include "regfile.h"

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unordered_map>

constexpr uint32_t REGION1_ZONE_BEGIN = 0x00100000;
constexpr uint32_t REGION2_ZONE_BEGIN = 0x00700000;
constexpr uint32_t REGION1_ZONE_END = 0x00500000;
constexpr uint32_t REGION2_ZONE_END = 0x03f00000;
constexpr uint32_t PM_SPACE_END = 0x04000000;

// TODO: tests memory (read-write-read)

// TODO: attributes / exceptions
// TODO: run simulation
// TODO:

constexpr uint32_t KERNEL_SPACE_END = 0x00400000;
constexpr uint32_t USER_SPACE_BEGIN = 0xFC800000;
constexpr uint32_t VM_SPACE_END = 0xFFFFFFFF;

constexpr uint32_t N_ENTRIES_PT_OUTER = 15;
constexpr uint32_t MAX_ENTRIES_PT = 1024;

constexpr uint32_t PT_OUTER_IDX_UPPER_REGION_START = 1010;

constexpr uint32_t N_PAGES = N_ENTRIES_PT_OUTER * MAX_ENTRIES_PT;
constexpr uint32_t PAGE_SIZE = 4 * 1024;
constexpr uint32_t TOTAL_RAM = N_PAGES * PAGE_SIZE;

// 64 MB - total physical memory space
//
// physical memory layout
/*---------------------------------- 0x00000000
|              Reserved1 1MB
|----------------------------------- REGION1_ZONE_BEGIN
|              PM Region 1 4MB
|----------------------------------- REGION1_ZONE_END
|              Reserved2 2MB
|----------------------------------- REGION2_ZONE_BEGIN
|              PM Region 2 56MB
|----------------------------------- REGION2_ZONE_END
|              Reserved3 1MB
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
            raw_ = ptr;
        }

        /*
        inline void ZeroOffset()
        {
            raw &= ~MASK_OFFSET;
        }

        inline void NextPage()
        {
            ZeroOffset();
            raw += MASK_OFFSET + 1;
        } */

        inline uint32_t PTIdxOuter() const
        {
            return (raw_ & MASK_PT_OUTER) >> 22;
        }

        inline uint32_t PTIdxInner() const
        {
            return (raw_ & MASK_PT_INNER) >> 12;
        }

        inline uint32_t Offset() const
        {
            return raw_ & MASK_OFFSET;
        }

        uint32_t raw_;
    };

  public:
    MemoryManager()
    {
        // mem_ = (uint8_t*)calloc(N_PAGES * PAGE_SIZE, sizeof(uint8_t));
        mem_ = new uint8_t[N_PAGES * PAGE_SIZE];
        // errno = 0;
        // mem_ = reinterpret_cast<uint8_t*>(mmap(NULL, N_PAGES * PAGE_SIZE,
        //                                        PROT_READ | PROT_WRITE,
        //                                        MAP_ANONYMOUS, 0, 0));
        // std::cout << errno << "\n";
        // assert(mem_ != MAP_FAILED);

        assert(mem_ != nullptr);

        InitPageTable();
    }

    ~MemoryManager()
    {
        // assert(munmap(mem_, N_PAGES * PAGE_SIZE) == 0);
        // free(mem_);
        delete[] mem_;
        mem_ = nullptr;
    }

    bool Read(const uint32_t vaddr, uint8_t* buf,
              uint32_t count) const override
    {
        assert(buf != nullptr);
        assert(vaddr >= USER_SPACE_BEGIN && vaddr + count < VM_SPACE_END);

        // TODO: check access rights

        Uint32_t_Ptr ptr(vaddr);
        /* uint32_t buf_offset = 0;
        uint32_t bytes_to_read = 0;
        while (count) {
            bytes_to_read = (count > PAGE_SIZE - ptr.Offset())
                                ? PAGE_SIZE - ptr.Offset()
                                : count;
            memcpy(buf + buf_offset, VadrToPadr(ptr), bytes_to_read);

            ptr.NextPage();

            buf_offset += bytes_to_read;
            count -= bytes_to_read;
        } */

        memcpy(buf, VadrToPadr(ptr), count);

        return true;
    }

    bool Write(const uint32_t vaddr, uint8_t* buf, uint32_t count) override
    {
        assert(buf != nullptr);
        assert(vaddr >= USER_SPACE_BEGIN && vaddr + count < VM_SPACE_END);

        // TODO: check access rights

        Uint32_t_Ptr ptr(vaddr);
        /* uint32_t buf_offset = 0;
        uint32_t bytes_to_write = 0;
        while (count) {
            bytes_to_write = (count > PAGE_SIZE - ptr.Offset())
                                 ? PAGE_SIZE - ptr.Offset()
                                 : count;
            memcpy(VadrToPadr(ptr), buf + buf_offset, bytes_to_write);

            ptr.NextPage();

            buf_offset += bytes_to_write;
            count -= bytes_to_write;
        } */
        memcpy(VadrToPadr(ptr), buf, count);

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

    inline uint8_t* GetRawMem()
    {
        return mem_;
    }

  private:
    void InitPageTable()
    {
        assert(mem_ != nullptr);
        // Region1 memory mapping
        for (size_t i = 0; i < MAX_ENTRIES_PT; ++i) {
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

        // printf("Addr is %p\n", (void*)(pt_[1010][0] - mem_));

        assert(pt_.size() == N_ENTRIES_PT_OUTER);
    }

    inline uint8_t* VadrToPadr(const Uint32_t_Ptr& vadr_ptr) const
    {
        assert(vadr_ptr.raw_ >= USER_SPACE_BEGIN &&
               vadr_ptr.raw_ < VM_SPACE_END);
        assert((vadr_ptr.PTIdxOuter() == 0) |
               (vadr_ptr.PTIdxOuter() >= PT_OUTER_IDX_UPPER_REGION_START));

        // // std::cout << "Raw addres " << vadr_ptr.raw_ << "\n";
        // // std::cout << "Outer index " << vadr_ptr.PTIdxOuter() << " Inner
        // index "
        //           << vadr_ptr.PTIdxInner() << " Offset is "
        //           << vadr_ptr.Offset() << "\n";
        return pt_.at(vadr_ptr.PTIdxOuter()).at(vadr_ptr.PTIdxInner()) +
               vadr_ptr.Offset();
    }

    using PageTableInner = std::unordered_map<uint32_t, uint8_t*>;
    using PageTableOuter = std::unordered_map<uint32_t, PageTableInner>;
    PageTableOuter pt_;
    uint8_t* mem_ = nullptr;
    RegFile regfile_ = {};
};

#endif