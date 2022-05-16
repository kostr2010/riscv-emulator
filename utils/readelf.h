#ifndef UTILS_READELF_H_INCLUDED
#define UTILS_READELF_H_INCLUDED

#include <elf.h>
#include <fcntl.h>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using Pointer = uint8_t*;

class ElfLoadException : public std::runtime_error
{
  public:
    explicit ElfLoadException(const std::string& mes)
        : std::runtime_error(mes){};
    explicit ElfLoadException(const char* mes) : std::runtime_error(mes){};
};

class ReadElf
{
  public:
    explicit ReadElf()
        : fd_(-1), elf_header{}, header_was_init(false), table_was_init(false)
    {}

    explicit ReadElf(int fd)
        : fd_(fd), elf_header{}, header_was_init(false), table_was_init(false)
    {}

    ~ReadElf()
    {
        for (const auto& it : allocated_chunks_) {
            if (munmap(it.first, it.second)) {
            }
        }
        allocated_chunks_.clear();
        if (munmap(memory_, PageSize)) {
        }
    }

    static bool ValidateElfHeader(const Elf32_Ehdr& elf_header);
    bool ValidateElfHeader() const
    {
        return ValidateElfHeader(elf_header);
    }
    const Elf32_Ehdr& ReadElfHeader();
    const std::vector<Elf32_Phdr>& ReadProgramHeaderTable();
    uint32_t LoadElf32IntoMemory();
    // void InitMemory();

    static constexpr uint32_t Size = (0u - 1u); // TODO change to actual size
    static constexpr uint32_t PageSize = 4096;

    // uint32_t AlignUp(uint32_t vaddr);
    // uint32_t AlignDown(uint32_t vaddr);
    // uint8_t* AllocateMemory(uint32_t vaddr, size_t length, int prot);
    // void* Memset(uint32_t vaddr, int fill_byte, size_t n_bytes);

  private:
    int fd_;
    Elf32_Ehdr elf_header;
    bool header_was_init;
    bool table_was_init;
    std::vector<Elf32_Phdr> ph_table;
    // TODO
    Pointer memory_; // Realize pointer
    std::vector<std::pair<void*, size_t> > allocated_chunks_;

    inline uint8_t* Translate(uint32_t vaddr) const
    {
        return memory_ + vaddr;
    }
};

#endif