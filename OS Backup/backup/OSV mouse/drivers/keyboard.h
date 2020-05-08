#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "/home/bradon/OS/OSV/Interrupts/port.h"
#include "/home/bradon/OS/OSV/Interrupts/interrupt.h"
#include "/home/bradon/OS/OSV/types.h"


class KeyboardDrive : public InterruptHandler{
    
    Port8Bit dataport;
    Port8Bit commandport;
    
public:
    KeyboardDrive(InterruptManager* manager);
    ~KeyboardDrive();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    
    
};




#endif
