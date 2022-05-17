#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_U()
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

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LUI()
{
    NOIMPL;
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_AUIPC()
{
    const auto result = pc_ + imm_;
    MemManager::SetGPR(rd_, result);
    return true;
}