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
    const auto result = RegGetVal(rs1_) + RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SUB()
{
    const auto result = RegGetVal(rs1_) - RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLL()
{
    const auto result = RegGetVal(rs1_) << RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}

// maybe wrong handler
bool Interpreter::HandleIns_SLT()
{
    const auto result = (RegGetVal(rs1_) < RegGetVal(rs2_)) ? 1 : 0;
    RegSetVal(rd_, result);

    return true;
}

// maybe wrong handler
bool Interpreter::HandleIns_SLTU()
{
    const auto result = (static_cast<uint32_t>(RegGetVal(rs1_)) <
                         static_cast<uint32_t>(RegGetVal(rs2_)))
                            ? 1
                            : 0;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_XOR()
{
    const auto result = RegGetVal(rs1_) ^ RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRL()
{
    const auto result = static_cast<uint32_t>(RegGetVal(rs1_)) >>
                        static_cast<uint32_t>(RegGetVal(rs2_));

    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRA()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = RegGetVal(rs1_) >> RegGetVal(rs2_);
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_OR()
{
    const auto result = RegGetVal(rs1_) | RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_AND()
{
    const auto result = RegGetVal(rs1_) & RegGetVal(rs2_);
    RegSetVal(rd_, result);

    return true;
}