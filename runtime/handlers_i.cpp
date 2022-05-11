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
    const auto result = RegGetVal(rs1_) + imm_;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLTI()
{
    const auto result = RegGetVal(rs1_) < imm_ ? 1 : 0;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLTIU()
{
    const auto result =
        static_cast<uint32_t>(registers_[rs1_]) < static_cast<uint32_t>(imm_)
            ? 1
            : 0;

    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_XORI()
{
    const auto result = RegGetVal(rs1_) ^ imm_;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_ORI()
{
    const auto result = RegGetVal(rs1_) | imm_;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_ANDI()
{
    const auto result = RegGetVal(rs1_) & imm_;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLLI()
{
    const auto result = RegGetVal(rs1_) << imm_;
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRLI()
{
    const auto result =
        static_cast<uint32_t>(RegGetVal(rs1_)) >> static_cast<uint32_t>(imm_);
    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRAI()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = RegGetVal(rs1_) >> imm_;
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    RegSetVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_LB()
{
    return true;
}

bool Interpreter::HandleIns_LH()
{
    return true;
}

bool Interpreter::HandleIns_LW()
{
    return true;
}

bool Interpreter::HandleIns_LBU()
{
    return true;
}

bool Interpreter::HandleIns_LHU()
{
    return true;
}

bool Interpreter::HandleIns_JALR()
{
    return true;
}