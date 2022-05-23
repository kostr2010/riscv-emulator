# CONTENTS

### Compilation

./riscv32-unknown-elf-gcc [-nostdlib] [-fPIC] -static [-nostartfiles] [--entry=main]

### Objdump

./riscv32-unknown-elf-objdump -D[-d] -M no-aliases

### How to run tests

- first, build `emulator` and `tests`
- then, execute `./emulator path/to/test/binary`. in the end it will print out return code. it should be the same as `.c` return code
