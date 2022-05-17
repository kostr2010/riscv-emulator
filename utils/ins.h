#ifndef INS_H_INCLUDED
#define INS_H_INCLUDED

#include <bitset>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>

#include "isa.h"

class Ins
{
  public:
    static const uint32_t INS_WIDTH = 32;
    static const uint32_t MASK_MSB = 0x80000000;

    // common
    static const uint32_t MASK_OPCODE = 0x0000007F;
    static const uint32_t MASK_RD = 0x00000F80;
    static const uint32_t MASK_RS1 = 0x000F8000;
    static const uint32_t MASK_RS2 = 0x01F00000;
    static const uint32_t MASK_FUNCT3 = 0x00007000;
    static const uint32_t MASK_FUNCT7 = 0xFE000000;

    // I-type
    static const uint32_t MASK_I_IMM_11_0 = 0xFFF00000;
    static const uint32_t MASK_I_IMM_4_0 = 0x01F00000;

    // S-type
    static const uint32_t MASK_S_IMM_4_0 = 0x00000F80;
    static const uint32_t MASK_S_IMM_11_5 = 0xFE000000;

    // B-type
    static const uint32_t MASK_B_IMM_11 = 0x00000080;
    static const uint32_t MASK_B_IMM_4_1 = 0x00000F00;
    static const uint32_t MASK_B_IMM_10_5 = 0x7E000000;
    static const uint32_t MASK_B_IMM_12 = 0x80000000;

    // U-type
    static const uint32_t MASK_U_IMM_31_12 = 0xFFFFF000;

    // J-type
    static const uint32_t MASK_J_IMM_19_12 = 0x000FF000;
    static const uint32_t MASK_J_IMM_11 = 0x00100000;
    static const uint32_t MASK_J_IMM_10_1 = 0x7FE00000;
    static const uint32_t MASK_J_IMM_20 = 0x80000000;

    enum class InsFormat
    {
        R,
        I,
        S,
        B,
        U,
        J,
        NOP,
    };

    enum InsOpcode
    {
        R = 0b0110011,
        I_ARITHMETIC = 0b0010011,
        I_LOAD = 0b0000011,
        I_JALR = 0b1100111,
        S = 0b0100011,
        B = 0b1100011,
        U_LUI = 0b0110111,
        U_AUIPC = 0b0010111,
        J = 0b1101111,
        NOP = 0b0,
    };

    enum class InsMnemonic
    {
        // R
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND,

        // I
        // ARITHMETIC
        ADDI,
        SLTI,
        SLTIU,
        XORI,
        ORI,
        ANDI,
        SLLI,
        SRLI,
        SRAI,
        // LOAD
        LB,
        LH,
        LW,
        LBU,
        LHU,
        // JALR
        JALR,

        // S
        SB,
        SH,
        SW,

        // B
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,

        // U
        LUI,
        AUIPC,

        // J
        JAL,

        // NOP
        NOP,
    };

    Ins() : ins_raw(0), fmt(InsFormat::NOP), mnm(InsMnemonic::NOP)
    {}
    ~Ins() = default;

    Ins(const uint32_t bits, const InsFormat format,
        const InsMnemonic mnemonic)
        : ins_raw(bits), fmt(format), mnm(mnemonic)
    {}

    Ins(const uint32_t bits) : ins_raw(bits)
    {
        const auto ins_opcode = bits & MASK_OPCODE;
        const auto ins_funct7 = bits & MASK_FUNCT7;
        const auto ins_funct3 = bits & MASK_FUNCT3;

#define OPLIST(ins, format, opcode, is_funct7, funct7, is_funct3, funct3,     \
               mnemonic)                                                      \
    if ((opcode == ins_opcode) &&                                             \
        (funct7 == (is_funct7 ? ins_funct7 : funct7)) &&                      \
        (funct3 == (is_funct3 ? ins_funct3 : funct3))) {                      \
        fmt = InsFormat::format;                                              \
        mnm = InsMnemonic::ins;                                               \
        return;                                                               \
    }
        INSTRUCTION_LIST(OPLIST)
#undef OPLIST
    }

    static inline Ins GetInsFromRaw(const uint32_t bits)
    {
        return Ins(bits);
    }

    std::string ToString() const;

    inline InsFormat GetInsFormat() const
    {
        return fmt;
    }

    inline InsMnemonic GetInsMnemonic() const
    {
        return mnm;
    }

    inline uint32_t GetInsRaw() const
    {
        return ins_raw;
    }

    bool GetRs1(uint32_t* rs1) const;
    bool GetRs2(uint32_t* rs2) const;
    bool GetRd(uint32_t* rd) const;
    bool GetImm(int32_t* imm) const;

    inline uint32_t GetImmSign() const
    {
        return ins_raw & MASK_MSB;
    }

    inline bool IsNOP() const
    {
        return (mnm == InsMnemonic::NOP) && (fmt == InsFormat::NOP);
    }

    static inline Ins MakeIns_NOP()
    {
        return Ins();
    }

    static inline Ins MakeIns_ADD(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b000, rd, InsMnemonic::ADD);
    }

    static inline Ins MakeIns_SUB(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0100000, rs2, rs1, 0b000, rd, InsMnemonic::SUB);
    }

    static inline Ins MakeIns_SLL(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b001, rd, InsMnemonic::SLL);
    }

    static inline Ins MakeIns_SLT(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b010, rd, InsMnemonic::SLT);
    }

    static inline Ins MakeIns_SLTU(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b011, rd, InsMnemonic::SLTU);
    }

    static inline Ins MakeIns_XOR(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b100, rd, InsMnemonic::XOR);
    }

    static inline Ins MakeIns_SRL(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b101, rd, InsMnemonic::SRL);
    }

    static inline Ins MakeIns_SRA(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0100000, rs2, rs1, 0b101, rd, InsMnemonic::SRA);
    }

    static inline Ins MakeIns_OR(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                 uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b110, rd, InsMnemonic::OR);
    }

    static inline Ins MakeIns_AND(uint32_t rs2 = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_R(0b0000000, rs2, rs1, 0b111, rd, InsMnemonic::AND);
    }

    static inline Ins MakeIns_ADDI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::ADDI);
    }

    static inline Ins MakeIns_SLTI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b010, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::SLTI);
    }

    static inline Ins MakeIns_SLTIU(int32_t imm = 0, uint32_t rs1 = 0,
                                    uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b011, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::SLTIU);
    }

    static inline Ins MakeIns_XORI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b100, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::XORI);
    }

    static inline Ins MakeIns_ORI(int32_t imm = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b110, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::ORI);
    }

    static inline Ins MakeIns_ANDI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b111, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::ANDI);
    }

    static inline Ins MakeIns_SLLI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b001, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::SLLI);
    }

    static inline Ins MakeIns_SRLI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        imm &= 0b000000011111;

        return MakeIns_I(imm, rs1, 0b101, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::SRLI);
    }

    static inline Ins MakeIns_SRAI(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        imm &= 0b000000011111;
        imm |= 0b010000000000;
        return MakeIns_I(imm, rs1, 0b101, rd, InsOpcode::I_ARITHMETIC,
                         InsMnemonic::SRAI);
    }

    static inline Ins MakeIns_LB(int32_t imm = 0, uint32_t rs1 = 0,
                                 uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, InsOpcode::I_LOAD,
                         InsMnemonic::LB);
    }

    static inline Ins MakeIns_LH(int32_t imm = 0, uint32_t rs1 = 0,
                                 uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b001, rd, InsOpcode::I_LOAD,
                         InsMnemonic::LH);
    }

    static inline Ins MakeIns_LW(int32_t imm = 0, uint32_t rs1 = 0,
                                 uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b010, rd, InsOpcode::I_LOAD,
                         InsMnemonic::LW);
    }

    static inline Ins MakeIns_LBU(int32_t imm = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b100, rd, InsOpcode::I_LOAD,
                         InsMnemonic::LBU);
    }

    static inline Ins MakeIns_LHU(int32_t imm = 0, uint32_t rs1 = 0,
                                  uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b101, rd, InsOpcode::I_LOAD,
                         InsMnemonic::LHU);
    }

    static inline Ins MakeIns_JALR(int32_t imm = 0, uint32_t rs1 = 0,
                                   uint32_t rd = 0)
    {
        return MakeIns_I(imm, rs1, 0b000, rd, InsOpcode::I_JALR,
                         InsMnemonic::JALR);
    }

    static inline Ins MakeIns_SB(int32_t imm = 0, uint32_t rs2 = 0,
                                 uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b000, InsMnemonic::SB);
    }

    static inline Ins MakeIns_SH(int32_t imm = 0, uint32_t rs2 = 0,
                                 uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b001, InsMnemonic::SH);
    }

    static inline Ins MakeIns_SW(int32_t imm = 0, uint32_t rs2 = 0,
                                 uint32_t rs1 = 0)
    {
        return MakeIns_S(imm, rs2, rs1, 0b010, InsMnemonic::SW);
    }

    static inline Ins MakeIns_BEQ(int32_t imm = 0, uint32_t rs2 = 0,
                                  uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b000, InsMnemonic::BEQ);
    }

    static inline Ins MakeIns_BNE(int32_t imm = 0, uint32_t rs2 = 0,
                                  uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b001, InsMnemonic::BNE);
    }

    static inline Ins MakeIns_BLT(int32_t imm = 0, uint32_t rs2 = 0,
                                  uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b100, InsMnemonic::BLT);
    }

    static inline Ins MakeIns_BGE(int32_t imm = 0, uint32_t rs2 = 0,
                                  uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b101, InsMnemonic::BGE);
    }

    static inline Ins MakeIns_BLTU(int32_t imm = 0, uint32_t rs2 = 0,
                                   uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b110, InsMnemonic::BLTU);
    }

    static inline Ins MakeIns_BGEU(int32_t imm = 0, uint32_t rs2 = 0,
                                   uint32_t rs1 = 0)
    {
        return MakeIns_B(imm, rs2, rs1, 0b111, InsMnemonic::BGEU);
    }

    static inline Ins MakeIns_LUI(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_U(imm, rd, InsOpcode::U_LUI, InsMnemonic::LUI);
    }

    static inline Ins MakeIns_AUIPC(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_U(imm, rd, InsOpcode::U_AUIPC, InsMnemonic::AUIPC);
    }

    static inline Ins MakeIns_JAL(int32_t imm = 0, uint32_t rd = 0)
    {
        return MakeIns_J(imm, rd, InsMnemonic::JAL);
    }

  private:
    static Ins MakeIns_R(uint32_t funct7, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, uint32_t rd, InsMnemonic mnemonic)
    {
        // [funct7][rs2][rs1][funct3][rd][opcode]
        // [     7][  5][  5][     3][ 5][     7]

        uint32_t ins = 0;
        ins = InsSetValueMask(ins, InsOpcode::R, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, funct7, MASK_FUNCT7, 25);

        return Ins(ins, InsFormat::R, mnemonic);
    }

    static Ins MakeIns_I(int32_t imm, uint32_t rs1, uint32_t funct3,
                         uint32_t rd, InsOpcode opcode, InsMnemonic mnemonic)
    {
        // [imm[11:0]][rs1][funct3][rd][opcode]
        // [       12][  5][     3][ 5][     7]
        assert(opcode == InsOpcode::I_ARITHMETIC ||
               opcode == InsOpcode::I_LOAD || opcode == InsOpcode::I_JALR);

        int8_t sign = (imm < 0) ? 1 : 0;
        imm &= 0x00000FFF;
        imm |= sign ? 0xFFFFF800 : 0x0;

        uint32_t ins = 0;
        ins = InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, funct3, MASK_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, imm, MASK_I_IMM_11_0, 20);

        return Ins(ins, InsFormat::I, mnemonic);
    }

    static Ins MakeIns_S(int32_t imm, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, InsMnemonic mnemonic)
    {
        // [imm[11:5]][rs2][rs1][funct3][imm[4:0]][opcode]
        // [        7][  5][  5][     3][       5][     7]

        int8_t sign = (imm < 0) ? 1 : 0;
        uint32_t imm_4_0 = imm & 0x0000001F;
        uint32_t imm_11_5 = (imm & 0x00000FE0) >> 5;
        imm_11_5 |= sign ? 0xFFFFFF40 : 0x0;

        uint32_t ins = 0;
        ins = InsSetValueMask(ins, InsOpcode::S, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_4_0, MASK_S_IMM_4_0, 7);
        ins = InsSetValueMask(ins, funct3, MASK_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, imm_11_5, MASK_S_IMM_11_5, 25);

        return Ins(ins, InsFormat::S, mnemonic);
    }

    static Ins MakeIns_B(int32_t imm, uint32_t rs2, uint32_t rs1,
                         uint32_t funct3, InsMnemonic mnemonic)
    {
        // [imm[12|10:5]][rs2][rs1][funct3][imm[4:1|11]][opcode]
        // [           7][  5][  5][     3][          5][     7]

        int8_t sign = (imm < 0) ? 1 : 0;
        uint32_t imm_4_1 = (imm & 0x0000001E) >> 1;
        uint32_t imm_10_5 = (imm & 0x000007E0) >> 5;
        uint32_t imm_11 = (imm & 0x00000800) >> 11;
        uint32_t imm_12 = sign;
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, InsOpcode::B, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, imm_11, MASK_B_IMM_11, 7);
        ins = InsSetValueMask(ins, imm_4_1, MASK_B_IMM_4_1, 8);
        ins = InsSetValueMask(ins, funct3, MASK_FUNCT3, 12);
        ins = InsSetValueMask(ins, rs1, MASK_RS1, 15);
        ins = InsSetValueMask(ins, rs2, MASK_RS2, 20);
        ins = InsSetValueMask(ins, imm_10_5, MASK_B_IMM_10_5, 25);
        ins = InsSetValueMask(ins, imm_12, MASK_B_IMM_12, 31);

        return Ins(ins, InsFormat::B, mnemonic);
    }

    static Ins MakeIns_U(int32_t imm, uint32_t rd, InsOpcode opcode,
                         InsMnemonic mnemonic)
    {
        // [imm[31:12]][rd][opcode]
        // [        20][ 5][     7]
        assert(opcode == InsOpcode::U_AUIPC || opcode == InsOpcode::U_LUI);

        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_31_12 = imm & MASK_U_IMM_31_12;
        imm_31_12 |= (sign ? MASK_MSB : 0x0);
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, opcode, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, imm_31_12, MASK_U_IMM_31_12, 0);

        return Ins(ins, InsFormat::U, mnemonic);
    }

    static Ins MakeIns_J(int32_t imm, uint32_t rd, InsMnemonic mnemonic)
    {
        // [imm[20|10:1|11|19:12]][rd][opcode]
        // [                   20][ 5][     7]

        int8_t sign = (imm < 0) ? 1 : 0;
        imm = abs(imm);

        uint32_t imm_10_1 = (imm & 0x000007FE) >> 1;
        uint32_t imm_11 = (imm & 0x00000800) >> 11;
        uint32_t imm_19_12 = (imm & 0x000FF000) >> 12;
        uint32_t imm_20 = sign;
        uint32_t ins = 0;

        ins = InsSetValueMask(ins, InsOpcode::J, MASK_OPCODE, 0);
        ins = InsSetValueMask(ins, rd, MASK_RD, 7);
        ins = InsSetValueMask(ins, imm_19_12, MASK_J_IMM_19_12, 12);
        ins = InsSetValueMask(ins, imm_11, MASK_J_IMM_11, 20);
        ins = InsSetValueMask(ins, imm_10_1, MASK_J_IMM_10_1, 21);
        ins = InsSetValueMask(ins, imm_20, MASK_J_IMM_20, 31);

        return Ins(ins, InsFormat::J, mnemonic);
    }

    static inline uint32_t InsSetValueMask(uint32_t ins, uint32_t val,
                                           uint32_t mask, size_t offset)
    {
        ins &= (~mask);
        val = (val << offset) & mask;
        return ins | val;
    }

    uint32_t ins_raw;
    InsFormat fmt;
    InsMnemonic mnm;
};

#endif