#ifndef REGFILE_INCLUDED
#define REGFILE_INCLUDED

#include <array>
#include <cassert>
#include <unordered_map>

struct RegFile
{
    class CSR
    {
      public:
        enum class Spec
        {
            WIRI, // ignore w and r, writing to WIRI CSR results in illegal
                  // instruction exception
            WPRI, // write preserves original value, read is ignored
            WLRL, // returns legal values if legal values are written. if
                  // illegal value is written, illegal value may be returned
            WARL, // write any value, read always legal value
            NONE,
        };

        struct Field
        {
            Field(const uint32_t& m, const Spec& s, const uint8_t& o)
                : MASK_{ m }, spec_{ s }, off_{ o }
            {}

            uint32_t MASK_ = 0x11111111;
            Spec spec_ = Spec::NONE;
            uint8_t off_ = 0;
        };

        CSR(const std::unordered_map<std::string, Field>& fields = {})
            : fields_(fields), value_(0)
        {}

        bool Write(const std::string& field, int32_t value)
        {
            if (fields_.find(field) == fields_.end()) {
                // WPRI case, since WPRI fields don't have name
                return true;
            }

            const auto& f = fields_[field];

            if (f.spec_ == Spec::WIRI) {
                // throw ILLEGAL_INSTRUCTION
                return false;
            }

            if (f.spec_ == Spec::WPRI) {
                return true;
            }

            value_ &= ~f.MASK_;                      // zeroing prev value
            value_ |= ((value << f.off_) & f.MASK_); // setting filed to value

            return true;
        }

        bool Read(const std::string& field, int32_t* value)
        {
            assert(value != nullptr);

            if (fields_.find(field) == fields_.end()) {
                // WPRI case, since WPRI fields don't have name
                return true;
            }

            const auto& f = fields_[field];

            if (f.spec_ == Spec::WIRI || f.spec_ == Spec::WPRI) {
                return true;
            }

            *value = (value_ & f.MASK_) >> f.off_;

            return true;
        }

      private:
        std::unordered_map<std::string, Field> fields_;
        uint32_t value_;
    };

    enum GPR
    {
        ZERO = 0,
        X1,    // return adress register
        SP,    // stack pointer
        GP,    // global pointer
        TP,    // thread pointer
        T0,    // temporary registers
        T1,    // temporary registers
        T2,    // temporary registers
        S0_FP, // saved registers within function definitions
        S1,    // saved registers within function definitions
        A0,    // function arguments
        A1,    // function arguments
        A2,    // function arguments
        A3,    // function arguments
        A4,    // function arguments
        A5,    // function arguments
        A6,    // function arguments
        A7,    // function arguments
        S2,    // saved registers within function definitions
        S3,    // saved registers within function definitions
        S4,    // saved registers within function definitions
        S5,    // saved registers within function definitions
        S6,    // saved registers within function definitions
        S7,    // saved registers within function definitions
        S8,    // saved registers within function definitions
        S9,    // saved registers within function definitions
        S10,   // saved registers within function definitions
        S11,   // saved registers within function definitions
        T3,    // temporary registers
        T4,    // temporary registers
        T5,    // temporary registers
        T6,    // temporary registers
        GPR_COUNT,
    };

    enum CSR_S
    {
        sstatus,
        satp,
        CSR_S_COUNT,
    };

    enum CSR_M
    {
        mstatus,
        CSR_M_COUNT,
    };

    RegFile()
    {
        // In a straightforward implementation, reading or writing any field in
        // sstatus is equivalent to reading or writing the homonymous field in
        // mstatus.
        s_csr_regs_[CSR_S::sstatus] = CSR({
            { "SD",
              { 0b10000000000000000000000000000000, CSR::Spec::NONE, 31 } },
            // The MXR (Make eXecutable Readable) bit modifies the privilege
            // with which loads access virtual memory. When MXR=0, only loads
            // from pages marked readable (R=1 in Figure 4.18) will succeed.
            // When MXR=1, loads from pages marked either readable or
            // executable (R=1 or X=1) will succeed. MXR has no effect when
            // page-based virtual memory is not in effect.
            { "MXR",
              { 0b00000000000010000000000000000000, CSR::Spec::NONE, 19 } },
            // The SUM (permit Supervisor User Memory access) bit modifies the
            // privilege with which S-mode loads and stores access virtual
            // memory. When SUM=0, S-mode memory accesses to pages that are
            // accessible by U-mode (U=1 in Figure 4.18) will fault. When
            // SUM=1, these accesses are permitted. SUM has no effect when
            // page-based virtual memory is not in effect, nor when executing
            // in U-mode. Note that S-mode can never execute instructions from
            // user pages, regardless of the state of SUM. SUM is read-only 0
            // if satp.MODE is read-only 0.
            { "SUM",
              { 0b00000000000001000000000000000000, CSR::Spec::NONE, 18 } },
            { "XS",
              { 0b00000000000000011000000000000000, CSR::Spec::NONE, 15 } },
            { "FS",
              { 0b00000000000000000110000000000000, CSR::Spec::NONE, 13 } },
            { "VS",
              { 0b00000000000000000000011000000000, CSR::Spec::NONE, 9 } },
            // The SPP bit indicates the privilege level at which a hart was
            // executing before entering supervisor mode. When a trap is taken,
            // SPP is set to 0 if the trap originated from user mode, or 1
            // otherwise. When an SRET instruction (see Section 3.3.2) is
            // executed to return from the trap handler, the privilege level is
            // set to user mode if the SPP bit is 0, or supervisor mode if the
            // SPP bit is 1; SPP is then set to 0.
            { "SPP",
              { 0b00000000000000000000000100000000, CSR::Spec::NONE, 8 } },
            // The UBE bit is a WARL field that controls the endianness of
            // explicit memory accesses made from U-mode, which may differ from
            // the endianness of memory accesses in S-mode. An implementation
            // may make UBE be a read-only field that always specifies the same
            // endianness as for S-mode. UBE controls whether explicit load and
            // store memory accesses made from U-mode are little-endian (UBE=0)
            // or big-endian (UBE=1). UBE has no effect on instruction fetches,
            // which are implicit memory accesses that are always
            // little-endian. For implicit accesses to supervisor-level memory
            // management data structures, such as page tables, S-mode
            // endianness always applies and UBE is ignored.
            { "UBE",
              { 0b00000000000000000000000001000000, CSR::Spec::WARL, 6 } },
            // The SPIE bit indicates whether supervisor interrupts were
            // enabled prior to trapping into supervisor mode. When a trap is
            // taken into supervisor mode, SPIE is set to SIE, and SIE is set
            // to 0. When an SRET instruction is executed, SIE is set to SPIE,
            // then SPIE is set to 1.
            { "SPIE",
              { 0b00000000000000000000000000100000, CSR::Spec::NONE, 5 } },
            // The SIE bit enables or disables all interrupts in supervisor
            // mode. When SIE is clear, interrupts are not taken while in
            // supervisor mode. When the hart is running in user-mode, the
            // value in SIE is ignored, and supervisor-level interrupts are
            // enabled. The supervisor can disable individual interrupt sources
            // using the sie CSR.
            { "SIE",
              { 0b00000000000000000000000000000010, CSR::Spec::NONE, 1 } },
        });
    }

    int32_t pc_;
    std::array<int32_t, GPR::GPR_COUNT> gpr_ = {};
    std::array<CSR, CSR_M::CSR_M_COUNT> s_csr_regs_ = {};
    std::array<CSR, CSR_M::CSR_M_COUNT> m_csr_regs_ = {};
};

#endif
