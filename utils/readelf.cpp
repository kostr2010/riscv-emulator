#include "readelf.h"
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>

bool ReadElf::ValidateElfHeader(const Elf32_Ehdr& elf_header)
{
    int cmp = memcmp(&elf_header.e_ident,
                     "\x7f"
                     "ELF",
                     4);
    return (cmp == 0) && (elf_header.e_type == ET_EXEC) &&
           (elf_header.e_machine == EM_RISCV);
}

const Elf32_Ehdr& ReadElf::ReadElfHeader()
{
    if (!header_was_init) {
        if (pread(fd_, &elf_header, sizeof(elf_header), 0) !=
            sizeof(elf_header)) {
            throw ElfLoadException("failed to read ELF header");
        }
        if (!ValidateElfHeader()) {
            throw ElfLoadException("invalid ELF header");
        }
        header_was_init = true;
    }
    return elf_header;
}

const std::vector<Elf32_Phdr>& ReadElf::ReadProgramHeaderTable()
{
    if (!table_was_init) {
        ph_table.resize(elf_header.e_phnum);
        long int table_bytes_size = sizeof(Elf32_Phdr) * elf_header.e_phnum;
        if (pread(fd_, ph_table.data(), table_bytes_size,
                  elf_header.e_phoff) != table_bytes_size) {
            throw ElfLoadException("failed to load Program Header Table");
        }
        table_was_init = true;
    }
    return ph_table;
}

uint32_t ReadElf::LoadElf32IntoMemory()
{
    ReadElfHeader();
    ReadProgramHeaderTable();
    for (const auto& p_header : ph_table) {
        if (p_header.p_type != PT_LOAD &&
            p_header.p_memsz == 0) { // must be loadable
            continue;
        }

        int prot = PROT_NONE;
        if (p_header.p_flags & PF_R) {
            prot |= PROT_READ;
        }
        if (p_header.p_flags & PF_W) {
            prot |= PROT_WRITE;
        }
        if (p_header.p_flags & PF_X) {
            prot |= PROT_EXEC;
        }

        auto vaddr = p_header.p_vaddr;
        auto mem_size = p_header.p_memsz;
        auto file_size = p_header.p_filesz;
        auto* paddr = AllocateMemory(vaddr, mem_size, prot);

        if (pread(fd_, paddr, file_size, p_header.p_offset) != file_size) {
            throw ElfLoadException("failed to copy segment into memory");
        }
        if (mem_size > file_size) {
            Memset(vaddr + file_size, 0, mem_size - file_size);
        }
    }
    return elf_header.e_entry;
}

// TODO mmap munmap
// First page allocation and cheking of other pages available
void ReadElf::InitMemory()
{
    memory_ = reinterpret_cast<uint8_t*>(
        mmap(NULL, // Need our own realization of mmap
             Size, // Need to choose our own max Size to mmap
             PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

    if (memory_ == MAP_FAILED) {
        throw ElfLoadException("Failed to init memory");
    }
    if (munmap(memory_ + PageSize,
               Size - PageSize)) { // Need our own realization of munmap
        throw ElfLoadException("Failed to init memory"); // Checked that enough
                                                         // memory is available
    } // Leave only one page of memory to save base ptr
}

uint32_t ReadElf::AlignUp(uint32_t vaddr)
{
    return ((vaddr + (PageSize - 1)) / PageSize) * PageSize;
}

// Get the begginning of page
uint32_t ReadElf::AlignDown(uint32_t vaddr)
{
    return vaddr - vaddr % PageSize;
}

// TODO mmap
uint8_t* ReadElf::AllocateMemory(uint32_t vaddr, size_t length, int prot)
{
    auto aligned_vaddr =
        AlignDown(vaddr); // addr in mmap % 4096 == 0 else fault
    length = AlignUp(length + vaddr -
                     aligned_vaddr); //  len in mmap % 4096 == 0 else fault
    if (Size - length < vaddr) {
        throw ElfLoadException("failed to allocate " + std::to_string(length) +
                               " bytes starting at " + std::to_string(vaddr));
    }

    void* paddr = mmap(Translate(aligned_vaddr), length,
                       prot | PROT_WRITE, // Need our own realization of mmap
                       MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (paddr == MAP_FAILED) {
        throw ElfLoadException("failed to allocate memory");
    }
    allocated_chunks_.emplace_back(paddr, length);

    return reinterpret_cast<uint8_t*>(paddr) + (vaddr - aligned_vaddr);
}

// TODO memset
void* ReadElf::Memset(uint32_t vaddr, int fill_byte, size_t n_bytes)
{
    return memset(Translate(vaddr), fill_byte,
                  n_bytes); // Need our own realization of memset
}
