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
        assert(strncmp((char*)header_.e_ident, "\177ELF", 4) == 0);
        assert(header_.e_ident[EI_CLASS] == ELFCLASS32);

        ReadSegmentHeaderTable(fd);
        ReadSegments(fd);

        host_entrypoint_ = header_.e_entry;
        // std::cout << "entrypoint is " << std::hex << host_entrypoint_
        //           << std::dec << "\n";

        is_elf_big_endian = header_.e_ident[EI_DATA] == ELFDATA2MSB ? 1 : 0;

        exec_segments_raw_.shrink_to_fit();
    }

    ~ElfFile()
    {
        free(segment_header_table_);
        for (uint32_t i = 0; i < exec_segments_raw_.size(); ++i) {
            free(exec_segments_raw_[i].first);
        }
    }

    std::vector<std::pair<uint32_t*, uint32_t> >& GetRaw()
    {
        return exec_segments_raw_;
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

    void DumpExecSection(uint32_t num) const
    {
        assert(num < exec_segments_raw_.size());
        for (uint32_t i = 0; i < exec_segments_raw_[num].second / 4; ++i) {
            std::cout << "0x" << std::setfill('0') << std::setw(8) << std::hex
                      << unsigned(exec_segments_raw_[num].first[i]) << "\n";
        }
        std::cout << std::dec;
    }

  private:
    void ReadElfHeader(int32_t fd)
    {
        assert(lseek(fd, 0, SEEK_SET) == 0);
        assert(read(fd, &header_, sizeof(Elf32_Ehdr)) == sizeof(Elf32_Ehdr));
    }

    void ReadSegmentHeaderTable(int32_t fd)
    {
        segment_header_table_ =
            (Elf32_Phdr*)calloc(header_.e_phnum, header_.e_phentsize);
        assert(segment_header_table_ != nullptr);
        assert(lseek(fd, header_.e_phoff, SEEK_SET) == header_.e_phoff);

        for (uint32_t i = 0; i < header_.e_phnum; ++i) {
            assert(read(fd, (void*)&segment_header_table_[i],
                        header_.e_phentsize) == header_.e_phentsize);
        }
    }

    void ReadSegments(int32_t fd)
    {
        // number of exec sections is not bigger than 4?
        exec_segments_raw_.reserve(4);
        bool entry_set = 0;
        for (int i = 0; i < header_.e_phnum; ++i) {
            if (!(segment_header_table_[i].p_type == PT_LOAD)) {
                continue;
            }

            if (!entry_set) {
                elf_start_addr_ = segment_header_table_[i].p_vaddr;
                // std::cout << "start addr is " << elf_start_addr_ << "\n";
                entry_set = 1;
            }

            uint32_t* ptr = (uint32_t*)calloc(
                segment_header_table_[i].p_memsz / 4, sizeof(uint32_t));

            assert(lseek(fd, segment_header_table_[i].p_offset, SEEK_SET) ==
                   segment_header_table_[i].p_offset);

            assert(read(fd, ptr, segment_header_table_[i].p_memsz) ==
                   segment_header_table_[i].p_memsz);
            uint32_t size = segment_header_table_[i].p_memsz;
            exec_segments_raw_.push_back({ ptr, size });
        }
    }
    Elf32_Ehdr header_;
    Elf32_Phdr* segment_header_table_;

    uint32_t host_entrypoint_ = 0;
    uint32_t elf_start_addr_ = 0;
    bool is_elf_big_endian = 0;

    // first - data, second - size of data in bytes
    std::vector<std::pair<uint32_t*, uint32_t> > exec_segments_raw_;
};

#endif