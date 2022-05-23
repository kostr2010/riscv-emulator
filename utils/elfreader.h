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

        lseek(fd, 0, SEEK_SET);
        size_ = lseek(fd, 0, SEEK_END);
        errno = 0;
        raw_file_ = (uint8_t*)mmap(NULL, size_, PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd, 0);
        assert(errno == 0);

        host_entrypoint_ = header_.e_entry;
        elf_start_addr_ = FindStartAddr(fd);

        // std::cout << "entrypoint is " << std::hex << host_entrypoint_
        //           << std::dec << "\n";

        // std::cout << "start addr is " << std::hex << elf_start_addr_
        //           << std::dec << "\n";

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

    uint32_t FindStartAddr(int32_t fd)
    {
        Elf32_Phdr* segment_header_table_ =
            (Elf32_Phdr*)calloc(header_.e_phnum, header_.e_phentsize);
        assert(segment_header_table_ != nullptr);
        assert(lseek(fd, header_.e_phoff, SEEK_SET) == header_.e_phoff);

        for (uint32_t i = 0; i < header_.e_phnum; ++i) {
            assert(read(fd, (void*)&segment_header_table_[i],
                        header_.e_phentsize) == header_.e_phentsize);
        }

        uint32_t result = segment_header_table_[1].p_vaddr;
        free(segment_header_table_);
        return result;
    }

    // void ReadSegments(int32_t fd)
    // {
    //     // number of exec sections is not bigger than 4?
    //     exec_segments_raw_.reserve(4);
    //     bool entry_set = 0;
    //     for (int i = 0; i < header_.e_shnum; ++i) {
    //         // if (!(segment_header_table_[i].p_type == PT_LOAD)) {
    //         //     continue;
    //         // }

    //         if (!entry_set) {
    //             elf_start_addr_ = segment_header_table_[i].sh_addr;
    //             // std::cout << "start addr is " << elf_start_addr_ << "\n";
    //             entry_set = 1;
    //         }

    //         uint32_t* ptr = (uint32_t*)calloc(
    //             segment_header_table_[i].sh_size / 4, sizeof(uint32_t));
    //         assert(ptr != nullptr);

    //         assert(lseek(fd, segment_header_table_[i].sh_offset, SEEK_SET)
    //         ==
    //                segment_header_table_[i].sh_offset);

    //         assert(read(fd, ptr, segment_header_table_[i].sh_size) ==
    //                segment_header_table_[i].sh_size);
    //         uint32_t size = segment_header_table_[i].sh_size;
    //         exec_segments_raw_.push_back({ ptr, size });
    //     }
    // }

    Elf32_Ehdr header_;

    uint32_t size_ = 0;

    uint32_t host_entrypoint_ = 0;
    uint32_t elf_start_addr_ = 0;
    bool is_elf_big_endian = 0;

    uint8_t* raw_file_;
};

#endif