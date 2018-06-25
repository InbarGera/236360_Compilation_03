#ifndef COMPILATION_03_REGALLOC_HPP
#define COMPILATION_03_REGALLOC_HPP

#include <cassert>
#include "utills.hpp"

class regClass{
    int myIndex;
    int offset_to_restore;
    bool saved;
public:
    regClass() : myIndex(-1) {};
    regClass(int i);
    std::string toString();
    int index();
};

regClass regAlloc();
void regFree(regClass toFree);
void assertAllRegistersAreFree();


#endif //COMPILATION_03_REGALLOC_HPP