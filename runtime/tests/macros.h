#ifndef TESTS_MACROS_H_INCLUDED
#define TESTS_MACROS_H_INCLUDED

#define MAKE_TEST_INST_B(INS_MNEMONIC, rd, rs1, rs2, rs1_val, rs2_val,        \
                         imm_val, ...)                                        \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
    }

#define TEST_IMM_OP(INS_MNEMONIC, rd, rs1, EXPECTED, rs1_val, imm_val, ...)   \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
        Interpreter<MemoryManager> inter(                                     \
            { Ins::MakeIns_##INS_MNEMONIC(imm_val, rs1, rd) });               \
        inter.SetIntReg(rs1, rs1_val);                                        \
        inter.Run();                                                          \
        ASSERT_EQ(inter.GetIntReg(rd), EXPECTED);                             \
    }

#define MAKE_TEST_INST_J(INS_MNEMONIC, unknown, rd, imm_val, ...)             \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
    }

#define TEST_RR_OP(INS_MNEMONIC, rd, rs1, rs2, EXPECTED, rs1_val, rs2_val,    \
                   ...)                                                       \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
        Interpreter<MemoryManager> inter(                                     \
            { Ins::MakeIns_##INS_MNEMONIC(rs2, rs1, rd) });                   \
        inter.SetIntReg(rs1, rs1_val);                                        \
        inter.SetIntReg(rs2, rs2_val);                                        \
        inter.Run();                                                          \
        ASSERT_EQ(inter.GetIntReg(rd), EXPECTED);                             \
    }

#define MAKE_TEST_INST_S(unknown1, unknown2, unknown3, EXPECTED, rs1, rs2,    \
                         rs2_val, imm_val, unknown4, INS_MNEMONIC, align)     \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
    }

// TODO
#define MAKE_TEST_INST_U(INS_MNEMONIC, rd, rs1, rs2, EXPECTED, rs1_val,       \
                         rs2_val, ...)                                        \
    TEST(TESTS_##INS_MNEMONIC, __LINE__)                                      \
    {                                                                         \
    }

#endif