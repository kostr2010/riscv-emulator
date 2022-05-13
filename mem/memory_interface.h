#ifndef MEMORY_INTERFACE_INCLUDED
#define MEMORY_INTERFACE_INCLUDED

#include <cctype>
#include <cstdint>

class MemoryInterface
{
  public:
    virtual bool Read(const uint32_t vaddr, uint8_t* buf,
                      const uint32_t count) const = 0;
    virtual bool Write(const uint32_t vaddr, uint8_t* buf,
                       const uint32_t count) = 0;
    virtual void SetIntReg(const uint32_t reg, const int32_t value) = 0;
    virtual int32_t GetIntReg(const uint32_t reg) const = 0;
    virtual void SetCSRReg(const uint32_t reg, const int32_t value) = 0;
    virtual int32_t GetCSRReg(const uint32_t reg) const = 0;
    virtual void SetPc(const int32_t value) = 0;
    virtual int32_t GetPc() const = 0;
};

#endif