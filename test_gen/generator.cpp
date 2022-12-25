#include "generator.h"
#include "mem/mem.h"

Ins TestGenerator::GenerateRMInst()
{
    uint32_t inst = rm_inst_distr_(gen_);
    uint32_t dst_reg = reg_distr_(gen_);
    uint32_t src1_reg = reg_distr_(gen_);
    uint32_t src2_reg = reg_distr_(gen_);

    if (regmap_[src1_reg] == 0 && regmap_[src2_reg] == 0) {
        uint32_t new_src_reg = reg_distr_(gen_);
        while (regmap_[new_src_reg] == 0) {
            new_src_reg = reg_distr_(gen_);
        }
        if (regmap_[src1_reg] == 0)
            src1_reg = new_src_reg;
        else
            src2_reg = new_src_reg;
    }
    regmap_[dst_reg] = 1;

    return creator_table_rm_[inst](src2_reg, src1_reg, dst_reg);
}

Ins TestGenerator::GenerateIInst()
{
    uint32_t inst = i_inst_distr_(gen_);
    uint32_t dst_reg = reg_distr_(gen_);
    uint32_t src_reg = reg_distr_(gen_);
    int32_t imm = imm_distr_(gen_);
    regmap_[dst_reg] = 1;

    return creator_table_i_[inst](imm, src_reg, dst_reg);
}

Ins TestGenerator::GenerateLSInst()
{
    uint32_t inst = ls_inst_distr_(gen_);
    uint32_t reg = reg_distr_(gen_);
    uint32_t adr = adr_distr_(gen_);
    return creator_table_ls_[inst](adr, RegFile::GPR::SP, reg);
}

Ins TestGenerator::GenerateInst()
{
    int32_t random_num = inst_distr_(gen_);
    if (random_num == 0) {
        return GenerateIInst();
    } else if (random_num == 1) {
        return GenerateRMInst();
    } else {
        return GenerateLSInst();
    }
}
