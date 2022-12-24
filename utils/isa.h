#define INSTRUCTION_LIST(_)                                                   \
    _(MUL, M, M, true, 0b0000001, true, 0b000, false, 0b000000, "MUL")        \
    _(MULH, M, M, true, 0b0000001, true, 0b001, false, 0b000000, "MULH")        \
    _(MULHSU, M, M, true, 0b0000001, true, 0b010, false, 0b000000, "MULHSU")        \
    _(MULHU, M, M, true, 0b0000001, true, 0b011, false, 0b000000, "MULHU")        \
    _(DIV, M, M, true, 0b0000001, true, 0b100, false, 0b000000, "DIV")        \
    _(DIVU, M, M, true, 0b0000001, true, 0b101, false, 0b000000, "DIVU")        \
    _(REM, M, M, true, 0b0000001, true, 0b110, false, 0b000000, "REM")        \
    _(REMU, M, M, true, 0b0000001, true, 0b111, false, 0b000000, "REMU")        \
    _(ADD, R, R, true, 0b0000000, true, 0b000, false, 0b000000, "ADD")        \
    _(SUB, R, R, true, 0b0100000, true, 0b000, false, 0b000000, "SUB")        \
    _(SLL, R, R, true, 0b0000000, true, 0b001, false, 0b000000, "SLL")        \
    _(SLT, R, R, true, 0b0000000, true, 0b010, false, 0b000000, "SLT")        \
    _(SLTU, R, R, true, 0b0000000, true, 0b011, false, 0b000000, "SLTU")      \
    _(XOR, R, R, true, 0b0000000, true, 0b100, false, 0b000000, "XOR")        \
    _(SRL, R, R, true, 0b0000000, true, 0b101, false, 0b000000, "SRL")        \
    _(SRA, R, R, true, 0b0100000, true, 0b101, false, 0b000000, "SRA")        \
    _(OR, R, R, true, 0b0000000, true, 0b110, false, 0b000000, "OR")          \
    _(AND, R, R, true, 0b0000000, true, 0b111, false, 0b000000, "AND")        \
    _(ADDI, I, I_ARITHMETIC, false, 0b0000000, true, 0b000, false, 0b000000,  \
      "ADDI")                                                                 \
    _(SLTI, I, I_ARITHMETIC, false, 0b0000000, true, 0b010, false, 0b000000,  \
      "SLTI")                                                                 \
    _(SLTIU, I, I_ARITHMETIC, false, 0b0000000, true, 0b011, false, 0b000000, \
      "SLTIU")                                                                \
    _(XORI, I, I_ARITHMETIC, false, 0b0000000, true, 0b100, false, 0b000000,  \
      "XORI")                                                                 \
    _(ORI, I, I_ARITHMETIC, false, 0b0000000, true, 0b110, false, 0b000000,   \
      "ORI")                                                                  \
    _(ANDI, I, I_ARITHMETIC, false, 0b0000000, true, 0b111, false, 0b000000,  \
      "ANDI")                                                                 \
    _(SLLI, I, I_ARITHMETIC, false, 0b0000000, true, 0b001, false, 0b000000,  \
      "SLLI")                                                                 \
    _(SRLI, I, I_ARITHMETIC, false, 0b0000000, true, 0b101, true, 0b000000,   \
      "SRLI")                                                                 \
    _(SRAI, I, I_ARITHMETIC, false, 0b0000000, true, 0b101, true, 0b010000,   \
      "SRAI")                                                                 \
    _(LB, I, I_LOAD, false, 0b0000000, true, 0b000, false, 0b000000, "LB")    \
    _(LH, I, I_LOAD, false, 0b0000000, true, 0b001, false, 0b000000, "LH")    \
    _(LW, I, I_LOAD, false, 0b0000000, true, 0b010, false, 0b000000, "LW")    \
    _(LBU, I, I_LOAD, false, 0b0000000, true, 0b100, false, 0b000000, "LBU")  \
    _(LHU, I, I_LOAD, false, 0b0000000, true, 0b101, false, 0b000000, "LHU")  \
    _(JALR, I, I_JALR, false, 0b0000000, true, 0b000, false, 0b000000,        \
      "JALR")                                                                 \
    _(ECALL, I, I_E, false, 0b0000000, false, 0b000, false, 0b000000,         \
      "ECALL")                                                                \
    _(EBREAK, I, I_E, false, 0b0000000, false, 0b000, false, 0b000000,        \
      "EBREAK")                                                               \
    _(SB, S, S, false, 0b0000000, true, 0b000, false, 0b000000, "SB")         \
    _(SH, S, S, false, 0b0000000, true, 0b001, false, 0b000000, "SH")         \
    _(SW, S, S, false, 0b0000000, true, 0b010, false, 0b000000, "SW")         \
    _(BEQ, B, B, false, 0b0000000, true, 0b000, false, 0b000000, "BEQ")       \
    _(BNE, B, B, false, 0b0000000, true, 0b001, false, 0b000000, "BNE")       \
    _(BLT, B, B, false, 0b0000000, true, 0b100, false, 0b000000, "BLT")       \
    _(BGE, B, B, false, 0b0000000, true, 0b101, false, 0b000000, "BGE")       \
    _(BLTU, B, B, false, 0b0000000, true, 0b110, false, 0b000000, "BLTU")     \
    _(BGEU, B, B, false, 0b0000000, true, 0b111, false, 0b000000, "BGEU")     \
    _(LUI, U, U_LUI, false, 0b0000000, false, 0b000, false, 0b000000, "LUI")  \
    _(AUIPC, U, U_AUIPC, false, 0b0000000, false, 0b000, false, 0b000000,     \
      "AUIPC")                                                                \
    _(JAL, J, J, false, 0b0000000, false, 0b000, false, 0b000000, "JAL")      \
    _(NOP, NOP, NOP, false, 0b0000000, false, 0b000, false, 0b000000, "NOP")
