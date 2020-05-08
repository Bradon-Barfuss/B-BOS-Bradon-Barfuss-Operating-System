#ifndef __OS__DRIVERS__KEYBOARD_H
#define __OS__DRIVERS__KEYBOARD_H


#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupt.h>
#include <hardwarecommunication/port.h>

namespace OS{
    namespace drivers{

        
        
class KeyboardEventHandler{
public:
    KeyboardEventHandler();
    
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};

class KeyboardDriver : public OS::hardwarecommunication::InterruptHandler, public Driver{
    
    OS::hardwarecommunication::Port8Bit dataport;
    OS::hardwarecommunication::Port8Bit commandport;
    
    KeyboardEventHandler* handler;
    
public:
    KeyboardDriver(OS::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual OS::common::uint32_t HandleInterrupt(OS::common::uint32_t esp);
    virtual void Activate();
    
    
};



    }
}


#endif
