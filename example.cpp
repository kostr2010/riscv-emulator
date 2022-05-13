#include <iostream>
#include <type_traits>
// runtime/
class MemoryInterface // interface
{
  public:
    virtual void foo() = 0;
};

template <class T>
class Interpreter : public T
{
    static_assert(std::is_base_of<MemoryInterface, T>::value == true);

  public:
    void call_foo()
    {
        T::bar();
    }
};

// mem/
class MemManager : MemoryInterface // mem manager
{
  public:
    void foo()
    {
        std::cout << "foo C\n";
    }

  private:
    void bar()
    {
        std::cout << "kek\n";
    }
};

class MemManagerTXT : MemoryInterface // mem manager
{
  public:
    void foo()
    {
        std::cout << "foo C\n";
    }
};

int main()
{
    Interpreter<MemManager> example;
    example.call_foo();
    return 0;
}