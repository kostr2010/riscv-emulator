#ifndef REGFILE_INCLUDED
#define REGFILE_INCLUDED

#include <array>
#include <cassert>
#include <unordered_map>
#include <cstdint>
#include <string>

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
            Field() = default;

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

            auto& f = fields_.at(field);

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

        bool Read(const std::string& field, int32_t* value) const
        {
            assert(value != nullptr);

            if (fields_.find(field) == fields_.end()) {
                // WPRI case, since WPRI fields don't have name
                return true;
            }

            const auto& f = fields_.at(field);

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
        mstatush,
        CSR_M_COUNT,
    };

    RegFile();

    int32_t pc_;
    std::array<int32_t, GPR::GPR_COUNT> gpr_ = {};
    std::array<CSR, CSR_M::CSR_M_COUNT> s_csr_regs_ = {};
    std::array<CSR, CSR_M::CSR_M_COUNT> m_csr_regs_ = {};
};

#endif
