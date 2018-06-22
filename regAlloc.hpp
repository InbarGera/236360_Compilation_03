#ifndef COMPILATION_03_REGALLOC_HPP
#define COMPILATION_03_REGALLOC_HPP

#include <cassert>
#include "utills.hpp"

class regClass{
    int myIndex;
public:
    reg(int i);
    std::string toString();
    int index();
};

regClass regAlloc();
void regFree(reg toFree);
void assertAllRegistersAreFree();

#endif //COMPILATION_03_REGALLOC_HPP
