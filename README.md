# INSTALL

`git clone git@github.com:kostr2010/riscv-emulator.git`

`cd riscv-emulator/`

`git submodule update --init --recursive`

# BUILD

from project's root:

`mkdir -p build/`

`cd build/`

`cmake ..`

`make -j[nproc]`

# STRUCTURE

Each of the below mentioned folders contain it's own README with further expplanation

- `tests/` - folder for emulator's tests. run `make tests` to build these tests
- `utils/` - folder for miscellneous stuff, like data structures, etc
- `third-party/` - folder for 3d party dependencies
- `mem/` - folder for emulator's memory. memory intefrace and memory implementation
- `runtime/` - folder for interpreter

# AUTHORS

- Ilya Trubachev(https://github.com/Trigger2000)
- Max Morozov(https://github.com/MaximMorozovMIPT)
- Konstantin Nazarov(https://github.com/kostr2010)

# CONTRIBUTION

Almost all work was done via VSCode's Live Share extension, so commits aren't actually representing the authorship of any changes. All work was split +- evenly and averyone's contribution was vital to the project
