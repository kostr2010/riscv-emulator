#include "mem.h"

const uint32_t KERNEL_SPACE_END = 0x00400000;
const uint32_t USER_SPACE_BEGIN = 0xFC800000;
const uint32_t VM_SPACE_END = 0xFFFFFFFF;

const uint32_t N_PAGES = N_ENTRIES_PT_OUTER * MAX_ENTRIES_PT;
const uint32_t PAGE_SIZE = 4 * 1024;
const uint32_t TOTAL_RAM = N_PAGES * PAGE_SIZE;