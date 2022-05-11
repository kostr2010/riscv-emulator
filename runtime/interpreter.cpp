#include "interpreter.h"
#include "utils/isa.h"

#include <iostream>

bool Interpreter::HandleIns()
{
    curr_ins_ = &(program_[PCToIndex(pc_)]);

    switch (curr_ins_->GetInsMnemonic()) {
#define OPLIST(instr, fmt, opcode, mnm)                                       \
    case Ins::InsMnemonic::instr:                                             \
        if (!HandleInsOperands_##fmt()) {                                     \
            return false;                                                     \
        }                                                                     \
        Interpreter::HandleIns_##instr();                                     \
        break;
        INSTRUCTION_LIST(OPLIST)
#undef OPLIST
    default:
        SetError(Err::ErrType::UNKNOWN_INSTRUCTION, "invalid mnemonic");
        return false;
    }

    if (err_.err_type_ != Err::ErrType::NONE) {
        return false;
    }

    return true;
}

bool Interpreter::HandleIns_NOP()
{
    return true;
}
