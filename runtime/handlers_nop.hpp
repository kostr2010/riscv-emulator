#include "interpreter.h"

template <class MemManager>
inline bool Interpreter<MemManager>::HandleInsOperands_NOP()
{
    return true;
}

template <class MemManager>
inline bool Interpreter<MemManager>::HandleIns_NOP()
{
    return true;
}