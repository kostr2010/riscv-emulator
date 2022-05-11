#include "interpreter.h"

bool Interpreter::HandleInsOperands_J()
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

bool Interpreter::HandleIns_JAL()
{
    uint32_t next_ins = pc_ + 4;
    RegSetVal(rd_, next_ins);

    is_jump_ins = 1;
    pc_ += imm_;
    return true;
}