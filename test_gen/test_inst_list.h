#define INSTRUCTION_LIST_R(_) \
    _(ADD)        \
    _(SUB)        \
    _(SLL)        \
    _(SLT)        \
    _(SLTU)      \
    _(XOR)        \
    _(SRL)        \
    _(SRA)        \
    _(OR)          \
    _(AND)

#define INSTRUCTION_LIST_I(_) \
    _(ADDI)   \
    _(SLTI) \
    _(SLTIU) \
    _(XORI)  \
    _(ORI) \
    _(ANDI) \
    _(SLLI) \
    _(SRLI) \
    _(SRAI)

#define INSTRUCTION_LIST_LS(_) \
    _(LB)   \
    _(LH) \
    _(LW) \
    _(SB)  \
    _(SH) \
    _(SW)
