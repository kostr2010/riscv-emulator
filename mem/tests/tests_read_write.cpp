#include "mem/mem.h"
#include <gtest/gtest.h>
#include <stdlib.h>

TEST(READ_WRITE_TEST, TEST1)
{
    MemoryManager mem;
    const int count = 100;
    uint8_t write_buffer[count];
    uint8_t read_buffer[count];
    uint8_t* raw_mem = mem.GetRawMem();

    for (uint8_t i = 0; i < count; ++i) {
        write_buffer[i] = i;
    }

    mem.Write(USER_SPACE_BEGIN, write_buffer, count);
    for (int i = 0; i < count; ++i) {
        ASSERT_EQ(raw_mem[REGION2_ZONE_BEGIN + i * sizeof(uint8_t)], i);
    }

    mem.Read(USER_SPACE_BEGIN, read_buffer, count);
    for (int i = 0; i < count; ++i) {
        ASSERT_EQ(read_buffer[i], i);
    }
}

TEST(READ_WRITE_TEST, TEST2)
{
    MemoryManager mem;
    const int count = 1000;
    uint32_t write_buffer[count];
    uint32_t read_buffer[count];
    uint8_t* raw_mem = mem.GetRawMem();

    for (uint32_t i = 0; i < count; ++i) {
        write_buffer[i] = i;
    }

    mem.Write(USER_SPACE_BEGIN, reinterpret_cast<uint8_t*>(write_buffer),
              count * sizeof(uint32_t) / sizeof(uint8_t));

    for (uint32_t i = 0; i < count; ++i) {
        ASSERT_EQ(i, *reinterpret_cast<uint32_t*>(
                         raw_mem + REGION2_ZONE_BEGIN + i * sizeof(uint32_t)));
    }

    mem.Read(USER_SPACE_BEGIN, reinterpret_cast<uint8_t*>(read_buffer),
             count * sizeof(uint32_t) / sizeof(uint8_t));
    for (uint32_t i = 0; i < count; ++i) {
        ASSERT_EQ(read_buffer[i], i);
    }
}

TEST(READ_WRITE_TEST, TEST3)
{
    MemoryManager mem;
    const uint32_t count = 9 * 1024;
    uint8_t write_buffer[count];
    uint8_t read_buffer[count];

    memset(write_buffer, '7', count);

    mem.Write(USER_SPACE_BEGIN + 137, write_buffer, count);
    mem.Read(USER_SPACE_BEGIN + 137, read_buffer, count);

    ASSERT_EQ(memcmp(read_buffer, write_buffer, count), 0);
}

TEST(READ_WRITE_TEST, TEST4)
{
    MemoryManager mem;
    const uint32_t count = 2;
    int32_t write_buffer;
    int32_t read_buffer;

    write_buffer = -42424242;

    mem.Write(USER_SPACE_BEGIN + 137,
              reinterpret_cast<uint8_t*>(&write_buffer), count);
    mem.Read(USER_SPACE_BEGIN + 137, reinterpret_cast<uint8_t*>(&read_buffer),
             count);

    ASSERT_EQ(memcmp(&read_buffer, &write_buffer, count), 0);
}