#ifndef __OS__HARDWARECOMMUNICATION__PORT_H
#define __OS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace OS{
    namespace hardwarecommunication{

        
        
class Port{
protected:
    OS::common::uint16_t portnumber; //every port needs a port number which is a 16 bit interger
    Port(OS::common::uint16_t portnumber); //go back to part 4 8:40 for needed info
    ~Port();
};


//8 bit port class
class Port8Bit : public Port{ //create a class that takes 8 bits and haves the information from class port
public:
    Port8Bit(OS::common::uint16_t portnumber);
    ~Port8Bit();
    virtual void Write(OS::common::uint8_t data);//have a function that takes a 8 bit variable called data
    virtual OS::common::uint8_t Read(); //have a 8 bit function called read
};


//this is a slower version of the 8 bit port, which needs it own write version
class Port8BitSlow : public Port8Bit { //create a class that takes 8 bits and haves the information from class port
public:
    Port8BitSlow(OS::common::uint16_t portnumber);
    ~Port8BitSlow();
    virtual void Write(OS::common::uint8_t data);//have a function that takes a 8 bit variable called data
    //it will inherit a read method
};


//16 bit port class
class Port16Bit : public Port { //create a class that takes 8 bits and haves the information from class port
public:
    Port16Bit(OS::common::uint16_t portnumber);
    ~Port16Bit();
    virtual void Write(OS::common::uint16_t data);//have a function that takes a 16 bit variable called data
    virtual OS::common::uint16_t Read(); //have a 16 bit function called read
};


//32 bit port class
class Port32Bit : public Port { //create a class that takes 8 bits and haves the information from class port
public: 
    Port32Bit(OS::common::uint16_t portnumber);
    ~Port32Bit();
    virtual void Write(OS::common::uint32_t data);//have a function that takes a 32 bit variable called data
    virtual OS::common::uint32_t Read(); //have a 32 bit function called read
};



    }
}
#endif
