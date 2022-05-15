#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_I()
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

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ADDI()
{
    const auto result = MemManager::GetIntReg(rs1_) + imm_;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLTI()
{
    const auto result = MemManager::GetIntReg(rs1_) < imm_ ? 1 : 0;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLTIU()
{
    const auto result = static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) <
                                static_cast<uint32_t>(imm_)
                            ? 1
                            : 0;

    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_XORI()
{
    const auto result = MemManager::GetIntReg(rs1_) ^ imm_;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ORI()
{
    const auto result = MemManager::GetIntReg(rs1_) | imm_;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ANDI()
{
    const auto result = MemManager::GetIntReg(rs1_) & imm_;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLLI()
{
    const auto result = MemManager::GetIntReg(rs1_) << imm_;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRLI()
{
    const auto result = static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) >>
                        static_cast<uint32_t>(imm_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRAI()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = MemManager::GetIntReg(rs1_) >> imm_;
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LB()
{
    assert(rd_ != RegFile::IntRegister::ZERO);
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = 0;
    MemManager::Read(adr, &buf, 1);

    // sign extend
    const uint32_t MASK_7_BIT = 0x00000080;
    buf |= buf & MASK_7_BIT == 1 ? 0xFFFFFF00 : 0x0;

    MemManager::SetIntReg(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LH()
{
    assert(rd_ != RegFile::IntRegister::ZERO);
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = 0;
    MemManager::Read(adr, &buf, 2);

    // sign extend
    const uint32_t MASK_15_BIT = 0x00008000;
    buf |= buf & MASK_15_BIT == 1 ? 0xFFFF0000 : 0x0;

    MemManager::SetIntReg(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LW()
{
    assert(rd_ != RegFile::IntRegister::ZERO);
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = 0;
    MemManager::Read(adr, &buf, 4);
    MemManager::SetIntReg(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LBU()
{
    assert(rd_ != RegFile::IntRegister::ZERO);
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = 0;
    MemManager::Read(adr, &buf, 1);
    MemManager::SetIntReg(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LHU()
{
    assert(rd_ != RegFile::IntRegister::ZERO);
    uint32_t adr = MemManager::GetIntReg(rs1_) + imm_;
    uint32_t buf = 0;
    MemManager::Read(adr, &buf, 2);
    MemManager::SetIntReg(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_JALR()
{
    uint32_t next_ins = pc_ + 4;
    MemManager::SetIntReg(rd_, next_ins);

    is_jump_ins_ = 1;
    uint32_t target_addr = MemManager::GetIntReg(rs1_) + imm_;
    target_addr &= 0xFFFFFFFE;
    pc_ += target_addr;
    return true;
}