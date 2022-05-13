#include "interpreter.h"
#include "utils/isa.h"

#include <iostream>

template <class MemManager>
void Interpreter<MemManager>::UpdatePc()
{
    switch (is_jump_ins_) {
    case 1:
        is_jump_ins_ = 0;
        break;
    case 0:
        pc_ += 4;
        break;
    }
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns()
{
    curr_ins_ = &(program_[PCToIndex(pc_)]);

    switch (curr_ins_->GetInsMnemonic()) {
#define OPLIST(ins, format, opcode, is_funct7, funct7, is_funct3, funct3,     \
               mnemonic)                                                      \
    case Ins::InsMnemonic::ins:                                               \
        if (!HandleInsOperands_##format()) {                                  \
            return false;                                                     \
        }                                                                     \
        HandleIns_##ins();                                                    \
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
