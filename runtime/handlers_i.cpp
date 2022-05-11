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
    const auto result = GetRegVal(rs1_) + imm_;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLTI()
{
    const auto result = GetRegVal(rs1_) < imm_ ? 1 : 0;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLTIU()
{
    const auto result =
        static_cast<uint32_t>(registers_[rs1_]) < static_cast<uint32_t>(imm_)
            ? 1
            : 0;

    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_XORI()
{
    const auto result = GetRegVal(rs1_) ^ imm_;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_ORI()
{
    const auto result = GetRegVal(rs1_) | imm_;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_ANDI()
{
    const auto result = GetRegVal(rs1_) & imm_;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SLLI()
{
    const auto result = GetRegVal(rs1_) << imm_;
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRLI()
{
    const auto result =
        static_cast<uint32_t>(GetRegVal(rs1_)) >> static_cast<uint32_t>(imm_);
    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_SRAI()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = GetRegVal(rs1_) >> imm_;
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    SetRegVal(rd_, result);

    return true;
}

bool Interpreter::HandleIns_LB()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_LH()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_LW()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_LBU()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_LHU()
{
    NOIMPL;
    return true;
}

bool Interpreter::HandleIns_JALR()
{
    uint32_t next_ins = pc_ + 4;
    SetRegVal(rd_, next_ins);

    is_jump_ins_ = 1;
    uint32_t target_addr = GetRegVal(rs1_) + imm_;
    target_addr &= 0xFFFFFFFE;
    pc_ += target_addr;
    return true;
}