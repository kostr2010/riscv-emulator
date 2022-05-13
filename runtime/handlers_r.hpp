#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_R()
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

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ADD()
{
    const auto result =
        MemManager::GetIntReg(rs1_) + MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SUB()
{
    const auto result =
        MemManager::GetIntReg(rs1_) - MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLL()
{
    const auto result = MemManager::GetIntReg(rs1_)
                        << MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

// maybe wrong handler
template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLT()
{
    const auto result =
        (MemManager::GetIntReg(rs1_) < MemManager::GetIntReg(rs2_)) ? 1 : 0;
    MemManager::SetIntReg(rd_, result);

    return true;
}

// maybe wrong handler
template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLTU()
{
    const auto result = (static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) <
                         static_cast<uint32_t>(MemManager::GetIntReg(rs2_)))
                            ? 1
                            : 0;
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_XOR()
{
    const auto result =
        MemManager::GetIntReg(rs1_) ^ MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRL()
{
    const auto result = static_cast<uint32_t>(MemManager::GetIntReg(rs1_)) >>
                        static_cast<uint32_t>(MemManager::GetIntReg(rs2_));

    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRA()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = MemManager::GetIntReg(rs1_) >> MemManager::GetIntReg(rs2_);
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_OR()
{
    const auto result =
        MemManager::GetIntReg(rs1_) | MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_AND()
{
    const auto result =
        MemManager::GetIntReg(rs1_) & MemManager::GetIntReg(rs2_);
    MemManager::SetIntReg(rd_, result);

    return true;
}