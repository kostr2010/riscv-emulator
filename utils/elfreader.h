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

        entrypoint_ = header_.e_entry;

        // std::cout << "entry count " << header_.e_phnum << "\n";

        // std::cout << "Elf header sizeof " << sizeof(Elf32_Ehdr) << "\n";
        // std::cout << "Program header sizeof " << sizeof(Elf32_Phdr) << "\n";

        // std::cout << "Entrypoint is " << std::hex << unsigned(entrypoint_)
        //           << "\n";
        // std::cout << "exec sections count " << executable_section_count
        //           << "\n";
        // std::cout << "e_shstrndx " << header_.e_shstrndx << "\n";
    }

    ~ElfFile()
    {
        free(section_header_table_);
        for (uint32_t i = 0; i < executable_section_count; ++i) {
            free(exec_sections_raw_[i].first);
        }
    }

    std::vector<std::pair<uint32_t*, uint32_t> > GetRaw() const
    {
        return exec_sections_raw_;
    }

    void DumpExecSection(uint32_t num)
    {
        assert(num < executable_section_count);
        for (uint32_t i = 0; i < exec_sections_raw_[num].second / 4; ++i) {
            std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex
                      << unsigned(exec_sections_raw_[num].first[i]) << "\n";
        }
    }

    uint32_t GetExecSectionNum() const
    {
        return executable_section_count;
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
        exec_sections_raw_.reserve(header_.e_phnum);
        uint32_t counter = 0;
        for (int i = 0; i < header_.e_phnum; ++i) {
            // 6 = SHF_EXECINSTR + SHF_ALLOC
            if (!(section_header_table_[i].sh_type == SHT_PROGBITS &&
                  section_header_table_[i].sh_flags == 6)) {
                continue;
            }
            exec_sections_raw_[counter].first = (uint32_t*)calloc(
                section_header_table_[i].sh_size / 4, sizeof(uint32_t));

            assert(lseek(fd, section_header_table_[i].sh_offset, SEEK_SET) ==
                   section_header_table_[i].sh_offset);

            assert(read(fd, exec_sections_raw_[counter].first,
                        section_header_table_[i].sh_size) ==
                   section_header_table_[i].sh_size);
            exec_sections_raw_[counter].second =
                section_header_table_[i].sh_size;
            counter++;
        }
        executable_section_count = counter;

        // text section
        // raw_data_ = exec_sections_raw_[0].first + sizeof(Elf32_Ehdr) +
        //             header_.e_phnum * header_.e_phentsize;

        // instr_count_ = program_header_table_[0].p_memsz
    }
    Elf32_Shdr* section_header_table_;
    Elf32_Ehdr header_;

    uint32_t entrypoint_ = 0;
    uint32_t executable_section_count = 0;
    // uint32_t instr_count_ = 0;

    // uint32_t* raw_data_ = nullptr;
    // first - data, second -size of data
    std::vector<std::pair<uint32_t*, uint32_t> > exec_sections_raw_;
};