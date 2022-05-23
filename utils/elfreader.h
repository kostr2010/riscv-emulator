#ifndef ELFREADER_H_INCLUDED
#define ELFREADER_H_INCLUDED

#include <assert.h>
#include <bitset>
#include <elf.h>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include <vector>

class ElfFile
{
  public:
    ElfFile(const char* path_)
    {
        int32_t fd = open(path_, O_RDWR);
        assert(fd != -1);
        ReadElfHeader(fd);
        assert(strncmp((char*)header_.e_ident, "\177ELF", 4) == 0);
        assert(header_.e_ident[EI_CLASS] == ELFCLASS32);

        ReadElf(fd);

        host_entrypoint_ = header_.e_entry;
        elf_start_addr_ = FindHostStartAddr(fd);

        is_elf_big_endian = header_.e_ident[EI_DATA] == ELFDATA2MSB ? 1 : 0;
        close(fd);
    }

    ~ElfFile()
    {
        munmap(raw_file_, size_);
    }

    uint8_t* GetRaw()
    {
        return raw_file_;
    }

    uint32_t GetHostEntrypoint() const
    {
        return host_entrypoint_;
    }

    uint32_t GetElfStartAddr() const
    {
        return elf_start_addr_;
    }

    bool IsElfBigEndian() const
    {
        return is_elf_big_endian;
    }

    uint32_t GetSize() const
    {
        return size_;
    }

    void Dump() const
    {
        std::cout << std::hex;
        for (uint32_t i = 0; i < size_; i += 4) {
            std::cout << "0x" << unsigned(raw_file_[i])
                      << unsigned(raw_file_[i + 1])
                      << unsigned(raw_file_[i + 2])
                      << unsigned(raw_file_[i + 3]) << "\n";
        }
        std::cout << std::dec;
    }

  private:
    void ReadElfHeader(int32_t fd)
    {
        assert(lseek(fd, 0, SEEK_SET) == 0);
        assert(read(fd, &header_, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
    }

    void ReadElf(int32_t fd)
    {
        lseek(fd, 0, SEEK_SET);
        size_ = lseek(fd, 0, SEEK_END);
        errno = 0;
        raw_file_ = (uint8_t*)mmap(NULL, size_, PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd, 0);
        assert(errno == 0);
    }

    uint32_t FindHostStartAddr(int32_t fd)
    {
        Elf32_Phdr* segment_header_table_ =
            (Elf32_Phdr*)calloc(header_.e_phnum, header_.e_phentsize);
        assert(segment_header_table_ != nullptr);
        assert(lseek(fd, header_.e_phoff, SEEK_SET) == header_.e_phoff);

        for (uint32_t i = 0; i < header_.e_phnum; ++i) {
            assert(read(fd, (void*)&segment_header_table_[i],
                        header_.e_phentsize) == header_.e_phentsize);
        }

        uint32_t result = segment_header_table_[1].p_vaddr; // 0 is empty
        free(segment_header_table_);
        return result;
    }

    Elf32_Ehdr header_;
    uint32_t size_ = 0;

    uint32_t host_entrypoint_ = 0;
    uint32_t elf_start_addr_ = 0;
    bool is_elf_big_endian = 0;

    uint8_t* raw_file_;
};

#endif