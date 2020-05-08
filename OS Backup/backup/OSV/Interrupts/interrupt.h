#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "/home/bradon/OS/OSV/types.h"
#include "port.h"

class InterruptManager{//created class called interrupt manager
public:
    static uint32_t handlerInterrupt(uint8_t interruptNumber, uint32_t esp);//get the interrupt number and the stack pointer
};
#endif
