#include "interpreter.h"

bool Interpreter::HandleInsOperands_B()
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

bool Interpreter::HandleIns_BEQ()
{
    if (GetRegVal(rs1_) == GetRegVal(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }

    return true;
}

bool Interpreter::HandleIns_BNE()
{
    if (GetRegVal(rs1_) != GetRegVal(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

bool Interpreter::HandleIns_BLT()
{
    if (GetRegVal(rs1_) < GetRegVal(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

bool Interpreter::HandleIns_BGE()
{
    if (GetRegVal(rs1_) >= GetRegVal(rs2_)) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

bool Interpreter::HandleIns_BLTU()
{
    if (static_cast<uint32_t>(GetRegVal(rs1_)) <
        static_cast<uint32_t>(GetRegVal(rs2_))) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}

bool Interpreter::HandleIns_BGEU()
{
    if (static_cast<uint32_t>(GetRegVal(rs1_)) >=
        static_cast<uint32_t>(GetRegVal(rs2_))) {
        is_jump_ins_ = 1;
        pc_ += imm_;
    }
    return true;
}