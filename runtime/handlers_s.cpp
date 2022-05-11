#include "interpreter.h"

bool Interpreter::HandleInsOperands_S()
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

bool Interpreter::HandleIns_SB()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_SH()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_SW()
{
    NOIMPL;
    return true;
}