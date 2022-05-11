#include "interpreter.h"

bool Interpreter::HandleInsOperands_U()
{
    if (!curr_ins_->GetImm(&imm_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get IMM");
        return false;
    }

    if (!curr_ins_->GetRd(&rd_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RD");
        return false;
    }

    return true;
}

bool Interpreter::HandleIns_LUI()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_AUIPC()
{
    const auto result = pc_ + imm_;
    SetRegVal(rd_, result);
    return true;
}