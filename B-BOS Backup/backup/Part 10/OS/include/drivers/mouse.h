#ifndef __OS__DRIVERS__MOUSE_H
#define __OS__DRIVERS__MOUSE_H


#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupt.h>
#include <hardwarecommunication/port.h>

namespace OS{
    namespace drivers{
        
        
        
class MouseEventHandler{
public:
    MouseEventHandler();
    
    virtual void OnActivate();
    virtual void OnMouseDown(OS::common::uint8_t button);
    virtual void OnMouseUp(OS::common::uint8_t button);
    virtual void OnMouseMove(int xoffset, int yoffset);
};

class MouseDriver : public OS::hardwarecommunication::InterruptHandler, public Driver{
    OS::hardwarecommunication::Port8Bit dataport;
    OS::hardwarecommunication::Port8Bit commandport;
    
    OS::common::uint8_t buffer[3];//the three bytes of information where the mouse is
    //buffer[0] = x-axis
    //buffer[1] = y-axis
    //buffer[2]
    OS::common::uint8_t offset;
    OS::common::uint8_t buttons;
    MouseEventHandler* handler;
    OS::common::int8_t x, y;
public:
    MouseDriver(OS::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
    ~MouseDriver();
    virtual OS::common::uint32_t HandleInterrupt(OS::common::uint32_t esp);
    virtual void Activate();
};
    


    }
}


#endif
