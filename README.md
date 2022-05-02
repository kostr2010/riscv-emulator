# INSTALL

`git clone git@github.com:kostr2010/riscv-emulator.git`
`cd riscv-emulator/`
`git submodule update --init --recursive`

# BUILD

from project's root:

`mkdir -p build/`
`cd build/`
`cmake ..`
`make -j8`

# STRUCTURE

- `tests/` - folder for emulator's tests. run `make tests` to build these tests
- `utils/` - folder for miscellneous stuff, like data structures, etc
- `scripts/` - folder for scripts for project maintaining and developing
- `modules/` - folder with implementation of RiscV modules like memory, etc
- `third-party/` - folder for 3d party dependencies
