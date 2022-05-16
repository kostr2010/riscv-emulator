#include "mem/mem.h"
#include <gtest/gtest.h>
#include <stdlib.h>

TEST(READ_WRITE_TEST, TEST1)
{
    MemoryManager mem;
    const int amount = 100;
    uint8_t write_buffer[amount];
    uint8_t read_buffer[amount];
    uint8_t* raw_mem = mem.GetRawMem();

    for (uint8_t i = 0; i < amount; ++i) {
        std::cout << i << " ";
        write_buffer[i] = i;
    }

    mem.Write(USER_SPACE_BEGIN, &write_buffer[0], amount);
    for (int i = 0; i < amount; ++i) {
        ASSERT_EQ(raw_mem[REGION2_ZONE_BEGIN + i * sizeof(uint8_t)], i);
    }

    mem.Read(USER_SPACE_BEGIN, &read_buffer[0], amount);
    for (int i = 0; i < amount; ++i) {
        ASSERT_EQ(read_buffer[i], i);
    }
}

TEST(READ_WRITE_TEST, TEST2)
{
    MemoryManager mem;
    const int amount = 1000;
    uint32_t write_buffer[amount];
    uint32_t read_buffer[amount];
    uint8_t* raw_mem = mem.GetRawMem();

    for (uint32_t i = 0; i < amount; ++i) {
        write_buffer[i] = i;
    }

    mem.Write(USER_SPACE_BEGIN, reinterpret_cast<uint8_t*>(&write_buffer[0]),
              amount);
    for (int i = 0; i < amount; ++i) {
        ASSERT_EQ(raw_mem[REGION2_ZONE_BEGIN + i * sizeof(uint8_t)], i);
    }

    mem.Read(USER_SPACE_BEGIN, &read_buffer[0], amount);
    for (int i = 0; i < amount; ++i) {
        ASSERT_EQ(read_buffer[i], i);
    }
}