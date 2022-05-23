#ifndef TESTS_MACROS_H_INCLUDED
#define TESTS_MACROS_H_INCLUDED

#define MAKE_TEST_INST_B(INS_MNEMONIC, rd, rs1, rs2, rs1_val, rs2_val,        \
                         imm_val, ...)                                        \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {}

#define TEST_IMM_OP(INS_MNEMONIC, rd, rs1, EXPECTED, rs1_val, imm, ...)       \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
        if (rd == RegFile::GPR::X1 || rs1 == RegFile::GPR::X1) {              \
            return;                                                           \
        }                                                                     \
        Interpreter<MemoryManager> interpreter{};                             \
        interpreter.RunLoader({ Ins::MakeIns_##INS_MNEMONIC(imm, rs1, rd) }); \
        interpreter.SetGPR(rs1, rs1_val);                                     \
        interpreter.Run();                                                    \
        ASSERT_EQ(interpreter.GetGPR(rd), EXPECTED);                          \
    }

#define MAKE_TEST_INST_J(INS_MNEMONIC, unknown, rd, imm_val, ...)             \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {}

#define TEST_RR_OP(INS_MNEMONIC, rd, rs1, rs2, EXPECTED, rs1_val, rs2_val,    \
                   ...)                                                       \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
        if (rd == RegFile::GPR::X1 || rs1 == RegFile::GPR::X1 ||              \
            rs2 == RegFile::GPR::X1) {                                        \
            return;                                                           \
        }                                                                     \
        Interpreter<MemoryManager> interpreter{};                             \
        interpreter.RunLoader({ Ins::MakeIns_##INS_MNEMONIC(rs2, rs1, rd) }); \
        interpreter.SetGPR(rs1, rs1_val);                                     \
        interpreter.SetGPR(rs2, rs2_val);                                     \
        interpreter.Run();                                                    \
        ASSERT_EQ(interpreter.GetGPR(rd), EXPECTED);                          \
    }

#define TEST_LOAD(unknown1, unknown2, unknown3, rs1, rd, imm_val, unknown4,   \
                  INS_MNEMONIC, align)                                        \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
        if (rd == RegFile::GPR::X1 || rs1 == RegFile::GPR::X1) {              \
            return;                                                           \
        }                                                                     \
        int32_t buf = std::numeric_limits<int32_t>::min();                    \
        uint32_t addr = static_cast<uint32_t>(0x1000);                        \
        uint32_t reg_buf = rs1 <= 3 ? 11 : rs1 - 1;                           \
        Interpreter<MemoryManager> interpreter{};                             \
        interpreter.RunLoader(                                                \
            { Ins::MakeIns_SW(imm_val, reg_buf, rs1),                         \
              Ins::MakeIns_##INS_MNEMONIC(imm_val, rs1, rd) });               \
        interpreter.SetGPR(reg_buf, buf);                                     \
        interpreter.SetGPR(rs1, addr);                                        \
        if (rd == 0) {                                                        \
            EXPECT_DEATH(interpreter.Run(), ".*");                            \
            return;                                                           \
        } else {                                                              \
            interpreter.Run();                                                \
        }                                                                     \
                                                                              \
        Ins::InsMnemonic mnemonic = Ins::InsMnemonic::INS_MNEMONIC;           \
        int32_t value = 0;                                                    \
        switch (mnemonic) {                                                   \
        case Ins::InsMnemonic::LW: {                                          \
            value = buf;                                                      \
            break;                                                            \
        }                                                                     \
        case Ins::InsMnemonic::LH: {                                          \
            value = (((buf & 0xFFFF0000) >> 16) & 0x0000FFFF) | 0xFFFF0000;   \
            break;                                                            \
        }                                                                     \
        case Ins::InsMnemonic::LB: {                                          \
            value = (((buf & 0xFF000000) >> 24) & 0x000000FF) | 0xFFFFFF00;   \
            break;                                                            \
        }                                                                     \
        case Ins::InsMnemonic::LHU: {                                         \
            value = ((buf & 0xFFFF0000) >> 16) & 0x0000FFFF;                  \
            break;                                                            \
        }                                                                     \
        case Ins::InsMnemonic::LBU: {                                         \
            value = ((buf & 0xFF000000) >> 24) & 0x000000FF;                  \
            break;                                                            \
        }                                                                     \
        default: {                                                            \
            break;                                                            \
        }                                                                     \
        }                                                                     \
        ASSERT_EQ(interpreter.GetGPR(rd), value);                             \
    }

// TODO
#define MAKE_TEST_INST_U(INS_MNEMONIC, rd, rs1, rs2, EXPECTED, rs1_val,       \
                         rs2_val, ...)                                        \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {}

#endif