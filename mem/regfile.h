#ifndef REGFILE_INCLUDED
#define REGFILE_INCLUDED

#include <array>

struct RegFile
{
    enum IntRegister
    {
        ZERO = 0,
        RA,
        SP,
        GP,
        TP,
        T0,
        T1,
        T2,
        S0_FP,
        S1,
        A0,
        A1,
        A2,
        A3,
        A4,
        A5,
        A6,
        A7,
        S2,
        S3,
        S4,
        S5,
        S6,
        S7,
        S8,
        S9,
        S10,
        S11,
        T3,
        T4,
        T5,
        T6,
        INT_REGISTERS_COUNT,
    };

    enum CSRRegister
    {
        CSR_REGISTERS_COUNT,
    };

    std::array<int32_t, IntRegister::INT_REGISTERS_COUNT> int_regs_ = {};
    std::array<int32_t, CSRRegister::CSR_REGISTERS_COUNT> csr_regs_ = {};
    int32_t pc_;
};

#endif
