#include "interpreter.h"

bool Interpreter::HandleInsOperands_I()
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

    if (!curr_ins_->GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS1");
        return false;
    }

    return true;
}

bool Interpreter::HandleIns_ADDI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_SLTI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_SLTUI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_XORI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_ORI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_ANDI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_SLLI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_SRLI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_SRAI()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_LB()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_LH()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_LW()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_LBU()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_LHU()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}

bool Interpreter::HandleIns_JALR()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    return true;
}