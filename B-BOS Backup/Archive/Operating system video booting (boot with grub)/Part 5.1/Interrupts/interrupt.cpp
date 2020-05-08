#include "interrupt.h"
#include "/home/bradon/OS/OSV/std/printf.h"

uint32_t InterruptManager::handlerInterrupt(uint8_t interruptNumber, uint32_t esp){//this is for stack switching. When we call a handler, we will get the interupt and put the program informaiton in the esp pointer
    printf("  INTERRUPT");
    
    return esp; //just return the stack pointer for now
};
