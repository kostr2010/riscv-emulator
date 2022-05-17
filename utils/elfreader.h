#include <elf.h>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class ElfFile
{
  public:
    ElfFile(const char* path_)
    {
        int32_t fd = open(path_, O_RDONLY | O_SYNC);
        assert(fd != -1);

        read_elf_header(fd);
        assert(strncmp((char*)elf_header_.e_ident, "\177ELF", 4) == 0);

        // assert that binary is 32 bit
        assert(elf_header_.e_ident[EI_CLASS] == ELFCLASS32);

        Elf32_Phdr
    }

    uint8_t* GetRaw() const
    {
        return elf_raw_;
    }

  private:
    void read_elf_header(int32_t fd)
    {
        assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
        assert(read(fd, &elf_header_, sizeof(Elf32_Ehdr)) ==
               sizeof(Elf32_Ehdr));
    }

    Elf32_Ehdr elf_header_;
    uint8_t* elf_raw_ = nullptr;
};