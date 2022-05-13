#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_B()
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
bool Interpreter<MemManager>::HandleIns_BEQ()
{
    if (MemManager::GetIntReg(rs1_) == MemManager::GetIntReg(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_BNE()
{
    if (MemManager::GetIntReg(rs1_) != MemManager::GetIntReg(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_BLT()
{
    if (MemManager::GetIntReg(rs1_) < MemManager::GetIntReg(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_BGE()
{
    if (MemManager::GetIntReg(rs1_) >= MemManager::GetIntReg(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_BLTU()
{
    if (static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) <
        static_cast<uint32_t>(MemManager::GetIntReg(rs2_))) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_BGEU()
{
    if (static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) >=
        static_cast<uint32_t>(MemManager::GetIntReg(rs2_))) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}