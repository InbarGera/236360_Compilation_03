#ifndef COMPILATION_03_REGALLOC_HPP
#define COMPILATION_03_REGALLOC_HPP

#include <cassert>
#include <vector>
#include "utills.hpp"

class regClass{
    int myIndex;
public:
    regClass() : myIndex(-1) {};
    regClass(int i);
    std::string toString();
    bool isFree();
    int index();
};

regClass regAlloc();
void regFree(regClass toFree);
void assertAllRegistersAreFree();
std::vector<regClass> getAllUsedRegisters();

#endif //COMPILATION_03_REGALLOC_HPP