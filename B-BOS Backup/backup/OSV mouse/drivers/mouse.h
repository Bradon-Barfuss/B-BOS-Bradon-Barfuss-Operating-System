#ifndef __MOUSE_H
#define __MOUSE_H

#include "/home/bradon/OS/OSV/Interrupts/port.h"
#include "/home/bradon/OS/OSV/Interrupts/interrupt.h"
#include "/home/bradon/OS/OSV/types.h"


class MouseDriver : public InterruptHandler{
    
    Port8Bit dataport;
    Port8Bit commandport;
    
    uint8_t buffer[3];//the three bytes of information where the mouse is
    //buffer[0] = x-axis
    //buffer[1] = y-axis
    //buffer[2]
    uint8_t offset;
    uint8_t buttons;
public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    
    
};




#endif
