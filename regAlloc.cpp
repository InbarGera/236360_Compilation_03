#include"regAlloc.hpp"

#define REG_NUM 17
#define FREE false
#define ALLOCATED true

static bool status[REG_NUM];
static bool initiated = false;

regClass::regClass(int i) {
    assert(i >= 0 && i < REG_NUM);
    myIndex = i;
}

std::string regClass::toString(){
    return "$" + num_to_string(myIndex + 8); // 8  is due to mips reg names convention
}

int regClass::index(){
    return myIndex;
}

regClass regAlloc(){
    if(!initiated) {
        for (int i = 0; i < REG_NUM; i++)
            status[i] = FREE;
        initiated = true;
    }

    for(int i=0; i<REG_NUM ;i++)
        if(status[i] == FREE) {
            status[i] == ALLOCATED;
            return reg(i);
        }
    assert(0); // should always be a free register
}

void regFree(regClass toFree){
    assert(status[toFree.index()] == ALLOCATED && initiated);
    status[toFree.index()] = FREE;
}

void assertAllRegistersAreFree(){
    for(int i=0; i<REG_NUM ;i++)
        assert(status[i] == FREE);
}
