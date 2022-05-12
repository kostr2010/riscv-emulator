#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED

#include "mem/mem.h"
#include "runtime/interpreter.h"
#include "utils/ins.h"

class Emulator
{
  public:
  private:
    Interpreter interpreter_;
    std::vector<Ins> program_;
    pmemory pmem_; // array with data
    vmemory vmem_; // address array without data

    Memory pmem_adr API(vmem_adr)
};

class vmem
{
    std::array<vpage_> addrs_;
    void vread(int32_t vaddr)
    {
        is_vaddr_in_dynamic_programm_space();
        pread(convert_to_physical(vaddr));
    }
};

class pmem
{};

class vpage
{
    uint32_t addr_begin_region_;
    uint32_t addr_end_regiob_;
    atttibutes attrs_;
};

class ppage
{};

class KUSOK_PAMYATI
{

    GetKernelSpaceBegin();
    GetKernelSpaceEnd();
    CheckIfInKernelSpace();

    GetStackBegin();
    GetStackEnd();
    CheckIfInStack();

    data Read(uint32_t v_adr) void Write(uint32_t v_adr, data)

        private : void* GetPAdr(uint32_t v_adr)
    {
        assert(v_adr <)
    }

    void* p_adr_space; // 4 mb
}

#endif