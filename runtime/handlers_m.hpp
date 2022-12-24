#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_M()
{
    if (!curr_ins_.GetRd(&rd_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rd_");
        return false;
    }

    if (!curr_ins_.GetRs2(&rs2_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rs2_");
        return false;
    }

    if (!curr_ins_.GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get rs1_");
        return false;
    }

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_MUL()
{
    const int32_t result = MemManager::GetGPR(rs1_) * MemManager::GetGPR(rs2_);
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_MULH()
{
    const int64_t result = MemManager::GetGPR(rs1_) * MemManager::GetGPR(rs2_);
    static constexpr uint64_t MASK_H = 0xFFFFFFFF00000000;
    const int32_t res = ((result & MASK_H) >> 32);
    MemManager::SetGPR(rd_, res);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_MULHSU()
{
    const int64_t result = MemManager::GetGPR(rs1_) * static_cast<uint32_t>(MemManager::GetGPR(rs2_));
    static constexpr uint64_t MASK_H = 0xFFFFFFFF00000000;
    const int32_t res = static_cast<int32_t>((result & MASK_H) >> 32);
    MemManager::SetGPR(rd_, res);

    return true;
}

// maybe wrong handler
template <class MemManager>
bool Interpreter<MemManager>::HandleIns_MULHU()
{
    const int64_t result = static_cast<uint32_t>(MemManager::GetGPR(rs1_)) * static_cast<uint32_t>(MemManager::GetGPR(rs2_));
    static constexpr uint64_t MASK_H = 0xFFFFFFFF00000000;
    const int32_t res = static_cast<int32_t>((result & MASK_H) >> 32);
    MemManager::SetGPR(rd_, res);

    return true;
}

// maybe wrong handler
template <class MemManager>
bool Interpreter<MemManager>::HandleIns_DIV()
{
    const int32_t result = MemManager::GetGPR(rs1_) / MemManager::GetGPR(rs2_);
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_DIVU()
{
    const uint32_t result = static_cast<uint32_t>(MemManager::GetGPR(rs1_)) * static_cast<uint32_t>(MemManager::GetGPR(rs2_));
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_REM()
{
    const int32_t result = MemManager::GetGPR(rs1_) % MemManager::GetGPR(rs2_);
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_REMU()
{
    const uint32_t result = static_cast<uint32_t>(MemManager::GetGPR(rs1_)) % static_cast<uint32_t>(MemManager::GetGPR(rs2_));
    MemManager::SetGPR(rd_, result);

    return true;
}