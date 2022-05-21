#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_S()
{
    if (!curr_ins_.GetImm(&imm_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get IMM");
        return false;
    }

    if (!curr_ins_.GetRs2(&rs2_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS2");
        return false;
    }

    if (!curr_ins_.GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS1");
        return false;
    }

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SB()
{
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    uint8_t buf = MemManager::GetGPR(rs2_);
    MemManager::Write(adr, reinterpret_cast<uint8_t*>(&buf), 1);
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SH()
{
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    uint16_t buf = MemManager::GetGPR(rs2_);
    if (is_host_big_endian != is_elf_big_endian) {
        buf = ReverseBytes16(buf);
    }
    MemManager::Write(adr, reinterpret_cast<uint8_t*>(&buf), 2);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SW()
{
    uint32_t adr = static_cast<uint32_t>(MemManager::GetGPR(rs1_)) + imm_;
    uint32_t buf = MemManager::GetGPR(rs2_);
    if (is_host_big_endian != is_elf_big_endian) {
        buf = ReverseBytes32(buf);
    }

    MemManager::Write(adr, reinterpret_cast<uint8_t*>(&buf), 4);

    return true;
}