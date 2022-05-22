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
        int32_t fd = open(path_, O_RDONLY | O_SYNC);
        assert(fd != -1);
        ReadElfHeader(fd);
        // assert that file is in elf format
        assert(strncmp((char*)header_.e_ident, "\177ELF", 4) == 0);
        // assert that binary is 32 bit
        assert(header_.e_ident[EI_CLASS] == ELFCLASS32);

        ReadSectionHeaderTable(fd);
        ReadExecSection(fd);

        host_entrypoint_ = header_.e_entry;

        // std::cout << "entry count " << header_.e_phnum << "\n";

        // std::cout << "Elf header sizeof " << sizeof(Elf32_Ehdr) << "\n";
        // std::cout << "Program header sizeof " << sizeof(Elf32_Phdr) << "\n";

        // std::cout << "Entrypoint is " << std::hex << unsigned(entrypoint_)
        //           << "\n";
        // std::cout << "exec sections count " << executable_section_count
        //           << "\n";
        // std::cout << "e_shstrndx " << header_.e_shstrndx << "\n";
        exec_sections_raw_.shrink_to_fit();
    }

    ~ElfFile()
    {
        free(section_header_table_);
        for (uint32_t i = 0; i < exec_sections_raw_.size(); ++i) {
            free(exec_sections_raw_[i].first);
        }
    }

    std::vector<std::pair<uint32_t*, uint32_t> >& GetRaw()
    {
        return exec_sections_raw_;
    }

    uint32_t GetHostEntrypoint() const
    {
        return host_entrypoint_;
    }

    uint32_t GetElfStartAddr() const
    {
        return elf_start_addr_;
    }

    void DumpExecSection(uint32_t num) const
    {
        assert(num < exec_sections_raw_.size());
        for (uint32_t i = 0; i < exec_sections_raw_[num].second / 4; ++i) {
            std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex
                      << unsigned(exec_sections_raw_[num].first[i]) << "\n";
        }
        std::cout << std::dec;
    }

  private:
    void ReadElfHeader(int32_t fd)
    {
        assert(lseek(fd, 0, SEEK_SET) == 0);
        assert(read(fd, &header_, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
    }

    void ReadSectionHeaderTable(int32_t fd)
    {
        section_header_table_ =
            (Elf32_Shdr*)calloc(header_.e_shnum, header_.e_shentsize);
        assert(section_header_table_ != nullptr);
        assert(lseek(fd, header_.e_shoff, SEEK_SET) == header_.e_shoff);

        for (uint32_t i = 0; i < header_.e_shnum; ++i) {
            assert(read(fd, (void*)&section_header_table_[i],
                        header_.e_shentsize) == header_.e_shentsize);
        }
    }

    void ReadExecSection(int32_t fd)
    {
        // number of exec sections is not bigger than 4?
        exec_sections_raw_.reserve(4);
        uint32_t counter = 0;
        for (int i = 0; i < header_.e_shnum; ++i) {
            // 6 = SHF_EXECINSTR + SHF_ALLOC
            if (!(section_header_table_[i].sh_type == SHT_PROGBITS &&
                  section_header_table_[i].sh_flags == 6)) {
                continue;
            }
            elf_start_addr_ = section_header_table_[i].sh_addr;
            uint32_t* ptr = (uint32_t*)calloc(
                section_header_table_[i].sh_size / 4, sizeof(uint32_t));

            assert(lseek(fd, section_header_table_[i].sh_offset, SEEK_SET) ==
                   section_header_table_[i].sh_offset);

            assert(read(fd, ptr, section_header_table_[i].sh_size) ==
                   section_header_table_[i].sh_size);
            uint32_t size = section_header_table_[i].sh_size;
            exec_sections_raw_.push_back({ ptr, size });
            counter++;
        }
    }
    Elf32_Ehdr header_;
    Elf32_Shdr* section_header_table_;

    uint32_t host_entrypoint_ = 0;
    uint32_t elf_start_addr_ = 0;

    // first - data, second - size of data in bytes
    std::vector<std::pair<uint32_t*, uint32_t> > exec_sections_raw_;
};

#endif