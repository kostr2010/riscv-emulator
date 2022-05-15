#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_S()
{
    if (!curr_ins_->GetImm(&imm_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get IMM");
        return false;
    }

    if (!curr_ins_->GetRs2(&rs2_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS2");
        return false;
    }

    if (!curr_ins_->GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS1");
        return false;
    }

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SB()
{
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = MemManager::GetIntReg(rs2_) & 0x000000FF;
    MemManager::Write(adr, &buf, 1);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SH()
{
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = MemManager::GetIntReg(rs2_) & 0x0000FFFF;
    MemManager::Write(adr, &buf, 2);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SW()
{
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = MemManager::GetIntReg(rs2_);
    MemManager::Write(adr, &buf, 4);

    return true;
}