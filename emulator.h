#ifndef EMULATOR_H_INCLUDED
#define EMULATOR_H_INCLUDED

#include "mem/mem.h"
#include "runtime/interpreter.h"
#include "utils/ins.h"

class Emulator
{
  public:
  private:
    Interpreter<MemoryManager> interpreter_;
    // MemoryManager mem_;
    // std::vector<Ins> program_;
};

#endif