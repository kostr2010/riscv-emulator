#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED

#include "mem/mem.h"
#include "runtime/interpreter.h"
#include "utils/ins.h"

class Emulator
{
  public:
    Emulator() = default;

  private:
    MemoryManager mem_;
    Interpreter<MemoryManager> interpreter_;
    // std::vector<Ins> program_;
};

#endif