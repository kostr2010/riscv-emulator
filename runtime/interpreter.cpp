#include "interpreter.h"
#include "utils/isa.h"

#include <iostream>

bool Interpreter::HandleIns()
{
    curr_ins_ = &(program_[PCToIndex(pc_)]);

    switch (curr_ins_->GetInsMnemonic()) {
#define OPLIST(ins, format, opcode, is_funct7, funct7, is_funct3, funct3,     \
               mnemonic)                                                      \
    case Ins::InsMnemonic::ins:                                               \
        if (!HandleInsOperands_##format()) {                                  \
            return false;                                                     \
        }                                                                     \
        Interpreter::HandleIns_##ins();                                       \
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
