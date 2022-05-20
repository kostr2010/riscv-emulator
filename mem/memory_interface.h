#ifndef MEMORY_INTERFACE_INCLUDED
#define MEMORY_INTERFACE_INCLUDED

#include <cctype>
#include <cstdint>
#include <string>

class MemoryInterface
{
  public:
    virtual bool Read(const uint32_t vaddr, uint8_t* buf,
                      const uint32_t count) const = 0;
    virtual bool Write(const uint32_t vaddr, uint8_t* buf,
                       const uint32_t count) = 0;
    virtual void SetGPR(const uint32_t reg, const int32_t value) = 0;
    virtual int32_t GetGPR(const uint32_t reg) const = 0;
    virtual void SetCSR_S(const uint32_t reg, const std::string& field,
                          const int32_t value) = 0;
    virtual int32_t GetCSR_S(const uint32_t reg,
                             const std::string& field) const = 0;
    virtual void SetCSR_M(const uint32_t reg, const std::string& field,
                          const int32_t value) = 0;
    virtual int32_t GetCSR_M(const uint32_t reg,
                             const std::string& field) const = 0;
    virtual void SetPC(const int32_t value) = 0;
    virtual int32_t GetPC() const = 0;
};

#endif