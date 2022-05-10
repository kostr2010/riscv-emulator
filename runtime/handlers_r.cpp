#include "interpreter.h"

bool Interpreter::HandleInsOperands_R()
{
    if (!curr_ins_->GetRd(&rd_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rd_");
        return false;
    }

    if (!curr_ins_->GetRs2(&rs2_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rs2_");
        return false;
    }

    if (!curr_ins_->GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rs1_");
        return false;
    }

    return true;
}

bool Interpreter::HandleIns_ADD()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] + registers_[rs2_];

    return true;
}

bool Interpreter::HandleIns_SUB()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] - registers_[rs2_];

    return true;
}

bool Interpreter::HandleIns_SLL()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] << registers_[rs2_];

    return true;
}

// maybe wrong handler
bool Interpreter::HandleIns_SLT()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = static_cast<int32_t>(registers_[rs1_]) <
                              static_cast<int32_t>(registers_[rs2_])
                          ? 1
                          : 0;

    return true;
}

// maybe wrong handler
bool Interpreter::HandleIns_SLTU()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] < registers_[rs2_] ? 1 : 0;

    return true;
}

bool Interpreter::HandleIns_XOR()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] ^ registers_[rs2_];

    return true;
}

bool Interpreter::HandleIns_SRL()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] >> registers_[rs2_];

    return true;
}

bool Interpreter::HandleIns_SRA()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    int sign_bit = rs1_ & Ins::MASK_MSB;
    registers_[rd_] = registers_[rs1_] >> registers_[rs2_];
    if (sign_bit)
        registers_[rd_] |= Ins::MASK_MSB;

    return true;
}

bool Interpreter::HandleIns_OR()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] | registers_[rs2_];

    return true;
}

bool Interpreter::HandleIns_AND()
{
    std::cout << "hanlde " << curr_ins_->ToString() << "\n";

    registers_[rd_] = registers_[rs1_] & registers_[rs2_];

    return true;
}