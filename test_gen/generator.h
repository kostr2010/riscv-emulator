#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <array>
#include <random>
#include <vector>

#include "test_inst_list.h"
#include "utils/ins.h"

constexpr int STACK_SIZE = 1024;

class TestGenerator {
public:
    TestGenerator() {
        std::random_device rd;
        gen_= std::mt19937(rd());
    }

    Ins GenerateInst();
    Ins GenerateIInst();
    Ins GenerateRInst();
    Ins GenerateLSInst();

private:
    std::bitset<32> regmap_;

    static constexpr int32_t int32_min = -2147483648;
    static constexpr int32_t int32_max = 2147483647;
    static constexpr int r_count = 10;
    static constexpr int i_count = 9;
    static constexpr int ls_count = 6;

    std::mt19937 gen_;
    std::uniform_int_distribution<> inst_distr_{0, 2};
    std::uniform_int_distribution<> reg_distr_{5, 31};
    std::uniform_int_distribution<> adr_distr_{0, STACK_SIZE};
    std::uniform_int_distribution<> imm_distr_{int32_min, int32_max};
    std::uniform_int_distribution<> r_inst_distr_{0, r_count - 1};
    std::uniform_int_distribution<> i_inst_distr_{0, i_count - 1};
    std::uniform_int_distribution<> ls_inst_distr_{0, ls_count - 1};

    #define INST_CREATOR(name) Ins::MakeIns_##name,
    constexpr static std::array<Ins (*)(uint32_t, uint32_t, uint32_t), r_count> creator_table_r_ = {
        INSTRUCTION_LIST_R(INST_CREATOR)
    };

    constexpr static std::array<Ins (*)(int32_t, uint32_t, uint32_t), i_count> creator_table_i_ = {
        INSTRUCTION_LIST_I(INST_CREATOR)
    };

    constexpr static std::array<Ins (*)(int32_t, uint32_t, uint32_t), ls_count> creator_table_ls_ = {
        INSTRUCTION_LIST_LS(INST_CREATOR)
    };
    #undef INST_CREATOR
};

#endif // GENERATOR_H_INCLUDED
