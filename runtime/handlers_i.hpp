#include "interpreter.h"

template <class MemManager>
bool Interpreter<MemManager>::HandleInsOperands_I()
{
    if (!curr_ins_.GetImm(&imm_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get IMM");
        return false;
    }

    if (!curr_ins_.GetRd(&rd_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RD");
        return false;
    }

    if (!curr_ins_.GetRs1(&rs1_)) {
        SetError(Err::ErrType::INVALID_INS_FORMAT,
                 "invalid instruction format! couldn't get RS1");
        return false;
    }

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ADDI()
{
    const auto result = MemManager::GetGPR(rs1_) + imm_;
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLTI()
{
    const auto result = (MemManager::GetGPR(rs1_) < imm_ ? 1 : 0);
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLTIU()
{
    const auto result = ((static_cast<uint32_t>(MemManager::GetGPR(rs1_)) <
                          static_cast<uint32_t>(imm_))
                             ? 1
                             : 0);

    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_XORI()
{
    const auto result = MemManager::GetGPR(rs1_) ^ imm_;
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ORI()
{
    const auto result = MemManager::GetGPR(rs1_) | imm_;
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ANDI()
{
    const auto result = MemManager::GetGPR(rs1_) & imm_;
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SLLI()
{
    const auto result = MemManager::GetGPR(rs1_) << imm_;
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRLI()
{
    const auto result = static_cast<uint32_t>(MemManager::GetGPR(rs1_)) >>
                        static_cast<uint32_t>(imm_);
    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_SRAI()
{
    const auto sign = rs1_ & Ins::MASK_MSB;

    auto result = MemManager::GetGPR(rs1_) >> imm_;
    if (sign) {
        result |= Ins::MASK_MSB;
    }

    MemManager::SetGPR(rd_, result);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LB()
{
    assert(rd_ != RegFile::GPR::ZERO);
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    adr += USER_SPACE_BEGIN - elf_start_addr_;
    uint8_t buf = 0;
    MemManager::Read(adr, reinterpret_cast<uint8_t*>(&buf), 1);
    uint32_t buf_extended = 0 | buf;
    buf_extended |= (((buf_extended & 0x00000080) != 0) ? 0xFFFFFF00 : 0x0);

    MemManager::SetGPR(rd_, buf_extended);
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LH()
{
    assert(rd_ != RegFile::GPR::ZERO);
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    adr += USER_SPACE_BEGIN - elf_start_addr_;
    uint16_t buf = 0;
    MemManager::Read(adr, reinterpret_cast<uint8_t*>(&buf), 2);
    uint32_t MASK_15_BIT = 0x00000080;
    if (is_host_big_endian != is_elf_big_endian) {
        buf = ReverseBytes16(buf);
        MASK_15_BIT = 0x00008000;
    }
    uint32_t buf_extended = 0 | buf;
    buf_extended |= (((buf_extended & MASK_15_BIT) != 0) ? 0xFFFF0000 : 0x0);

    MemManager::SetGPR(rd_, buf_extended);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LW()
{
    assert(rd_ != RegFile::GPR::ZERO);
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    adr += USER_SPACE_BEGIN - elf_start_addr_;
    std::cout << "adr is " << adr << "\n";
    uint32_t buf = 0;
    MemManager::Read(adr, reinterpret_cast<uint8_t*>(&buf), 4);

    uint32_t tmp = 0;
    MemManager::Read(adr - 4, reinterpret_cast<uint8_t*>(&tmp), 4);
    std::cout << "tmp is " << tmp << "\n";

    std::cout << "buf before " << buf << "\n";
    if (is_host_big_endian != is_elf_big_endian) {
        buf = ReverseBytes32(buf);
    }
    MemManager::SetGPR(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LBU()
{
    assert(rd_ != RegFile::GPR::ZERO);
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    adr += USER_SPACE_BEGIN - elf_start_addr_;
    uint32_t buf = 0;
    MemManager::Read(adr, reinterpret_cast<uint8_t*>(&buf), 1);
    MemManager::SetGPR(rd_, buf);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_LHU()
{
    assert(rd_ != RegFile::GPR::ZERO);
    uint32_t adr = MemManager::GetGPR(rs1_) + imm_;
    adr += USER_SPACE_BEGIN - elf_start_addr_;
    uint16_t buf = 0;
    MemManager::Read(adr, reinterpret_cast<uint8_t*>(&buf), 2);
    if (is_host_big_endian != is_elf_big_endian) {
        buf = ReverseBytes16(buf);
    }
    uint32_t buf_extended = 0 | buf;

    MemManager::SetGPR(rd_, buf_extended);

    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_JALR()
{
    MemManager::SetGPR(rd_, pc_ + 4);

    is_jump_ins_ = 1;
    uint32_t target_addr = MemManager::GetGPR(rs1_) + imm_;
    target_addr &= 0xFFFFFFFE;
    pc_ = target_addr;
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_ECALL()
{
    return true;
}

template <class MemManager>
bool Interpreter<MemManager>::HandleIns_EBREAK()
{
    return true;
}