#include<iostream>
class I {
  public:
    virtual ~I() {
        std::cout << " base class I deconstruct call" << std::endl;
    }
    virtual void Output() = 0;
};
