#include "ins.h"
#include "isa.h"

#include <map>
#include <sstream>

std::string Ins::ToString() const
{

    static std::map<InsMnemonic, std::string> mnm_to_string = {
#define OPLIST(ins, fmt, opcode, mnm) { InsMnemonic::ins, #ins },
        INSTRUCTION_LIST(OPLIST)
#undef OPLIST
    };

    std::stringstream s;

    s << "INS: " << mnm_to_string.at(mnm) << "\n";

    int32_t imm = 0;
    if (GetImm(&imm)) {
        s << "IMM: " << imm << "\n";
    }

    uint32_t reg = 0;
    if (GetRs2(&reg)) {
        s << "RS2: " << reg << "\n";
    }
    if (GetRs1(&reg)) {
        s << "RS1: " << reg << "\n";
    }
    if (GetRd(&reg)) {
        s << "RD:  " << reg << "\n";
    }

    s << "RAW: " << std::bitset<32>(ins_raw) << "\n";

    return s.str();
}

bool Ins::GetRs1(uint32_t* rs1) const
{
    assert(rs1 != nullptr);

    switch (fmt) {
    case InsFormat::R:
    case InsFormat::I:
    case InsFormat::S:
    case InsFormat::B:
        *rs1 = (ins_raw & MASK_RS1) >> 15;
        return true;
    default:
        return false;
    }
}

bool Ins::GetRs2(uint32_t* rs2) const
{
    assert(rs2 != nullptr);

    switch (fmt) {
    case InsFormat::R:
    case InsFormat::S:
    case InsFormat::B:
        *rs2 = (ins_raw & MASK_RS2) >> 20;
        return true;
    default:
        return false;
    }
}

bool Ins::GetRd(uint32_t* rd) const
{
    assert(rd != nullptr);

    switch (fmt) {
    case InsFormat::R:
    case InsFormat::I:
    case InsFormat::U:
    case InsFormat::J:
        *rd = (ins_raw & MASK_RD) >> 7;
        return true;
    default:
        return false;
    }
}

bool Ins::GetImm(int32_t* imm) const
{
    assert(imm != nullptr);

    uint32_t res = 0;

    switch (fmt) {
    case InsFormat::R:
        return false;
    case InsFormat::I: {
        bool is_shift = (mnm == InsMnemonic::SLLI ||
                         mnm == InsMnemonic::SRLI || mnm == InsMnemonic::SRAI);

        if (is_shift) {
            res = (ins_raw & MASK_I_IMM_4_0) >> 20;
        } else {
            res = (ins_raw & MASK_I_IMM_11_0) >> 20;
            res |= GetImmSign() ? 0xFFFFF800 : 0x0;
        }

        break;
    }
    case InsFormat::S: {
        uint32_t imm_4_0 = (ins_raw & MASK_S_IMM_4_0) >> 7;
        uint32_t imm_11_5 = (ins_raw & MASK_S_IMM_11_5) >> 25;

        res = (imm_11_5 << 5) | imm_4_0;
        res |= GetImmSign() ? 0xFFFFF800 : 0x0;

        break;
    }
    case InsFormat::B: {
        uint32_t imm_11 = (ins_raw & MASK_B_IMM_11) >> 7;
        uint32_t imm_4_1 = (ins_raw & MASK_B_IMM_4_1) >> 8;
        uint32_t imm_10_5 = (ins_raw & MASK_B_IMM_10_5) >> 25;

        res = 0b0 | (imm_4_1 << 1) | (imm_10_5 << 5) | (imm_11 << 11);
        res |= GetImmSign() ? 0xFFFFF000 : 0x0;

        break;
    }
    case InsFormat::U: {
        res = ins_raw & MASK_U_IMM_31_12;

        break;
    }
    case InsFormat::J: {
        uint32_t imm_19_12 = (ins_raw & MASK_J_IMM_19_12) >> 12;
        uint32_t imm_11 = (ins_raw & MASK_J_IMM_11) >> 20;
        uint32_t imm_10_1 = (ins_raw & MASK_J_IMM_10_1) >> 21;

        res = 0b0 | (imm_10_1 << 1) | (imm_11 << 11) | (imm_19_12 << 12);
        res |= GetImmSign() ? 0xFFF00000 : 0x0;

        break;
    }
    default:
        return false;
    }

    *imm = res;

    return true;
}