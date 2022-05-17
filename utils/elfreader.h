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

        program_header_table_ =
            (Elf32_Phdr*)calloc(header_.e_phnum, header_.e_phentsize);

        ReadProgramHeaderTable(fd);
        ReadExecutionSection(fd);

        entrypoint_ = header_.e_entry;
    }

    ~ElfFile()
    {
        free(program_header_table_);
        for (uint32_t i = 0; i < executable_section_count; ++i) {
            free(exec_sections_raw_[i].first);
        }
    }

    std::vector<std::pair<uint8_t*, uint32_t> > GetRaw() const
    {
        return exec_sections_raw_;
    }

    void DumpExecSection(uint32_t num)
    {
        assert(num < executable_section_count);
        for (uint32_t i = 0; i < exec_sections_raw_[num].second; ++i) {
            if (i % 4 == 0) {
                std::cout << "\n";
            }
            // std::cout << std::bitset<8>(exec_sections_raw_[num].first[i])
            //          << " ";
            std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex
                      << unsigned(exec_sections_raw_[num].first[i]) << " ";
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

    void ReadProgramHeaderTable(int32_t fd)
    {
        assert(lseek(fd, header_.e_phoff, SEEK_SET) == header_.e_phoff);

        for (uint32_t i = 0; i < header_.e_phnum; ++i) {
            assert(read(fd, (void*)&program_header_table_[i],
                        header_.e_phentsize) == header_.e_phentsize);
        }
    }

    void ReadExecutionSection(int32_t fd)
    {
        exec_sections_raw_.reserve(header_.e_phnum);
        uint32_t counter = 0;
        for (int i = 0; i < header_.e_phnum; ++i) {
            if (program_header_table_[i].p_type != PT_LOAD) {
                continue;
            }
            exec_sections_raw_[counter].first = (uint8_t*)calloc(
                program_header_table_[i].p_memsz, sizeof(uint8_t));

            assert(lseek(fd, program_header_table_[i].p_offset, SEEK_SET) ==
                   program_header_table_[i].p_offset);

            assert(read(fd, exec_sections_raw_[counter].first,
                        program_header_table_[i].p_memsz) ==
                   program_header_table_[i].p_memsz);
            exec_sections_raw_[counter].second =
                program_header_table_[i].p_memsz;
            counter++;
        }
        executable_section_count = counter;
    }

    uint32_t executable_section_count = 0;
    uint32_t entrypoint_ = 0;
    Elf32_Phdr* program_header_table_;
    Elf32_Ehdr header_;
    // first - data, second -size of data
    std::vector<std::pair<uint8_t*, uint32_t> > exec_sections_raw_;
};