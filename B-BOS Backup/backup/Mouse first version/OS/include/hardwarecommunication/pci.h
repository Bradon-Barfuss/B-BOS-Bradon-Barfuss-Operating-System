#ifndef __OS__HARDWARECOMMUNICATION__PCI_H
#define __OS__HARDWARECOMMUNICATION__PCI_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupt.h>

namespace OS{
    namespace hardwarecommunication{
    
enum BaseAddressRegisterType{//the first byte in the base address determins if it a IO or MemoryMapping
    MemoryMapping = 0,
    InputOutput = 1
};

class BaseAddressRegister{
public:
    bool prefetchable;//a bool for the 4-bit only in the MemoryMapping section
    OS::common::uint8_t* address;
    OS::common::uint32_t size;
    BaseAddressRegisterType type;
    
    
};

        
//this class will store all the data we get from the PCI 
class PeripheralComponentInterconnectDeviceDescriptor{
public:
    OS::common::uint32_t portBase;//the number for the port from the PCI controller
    OS::common::uint32_t interrupt;//the number for the interrupt
    
    OS::common::uint16_t bus;//store the bus information
    OS::common::uint16_t device;//store the device information
    OS::common::uint16_t function;//store the function information
    
    //store the information we get from the driver
    OS::common::uint16_t vendor_id;
    OS::common::uint16_t device_id;
    
    OS::common::uint8_t class_id;
    OS::common::uint8_t subclass_id;
    OS::common::uint8_t interface_id;
    
    OS::common::uint8_t revision;

    void GetDeviceDescriptor(OS::common::uint16_t, OS::common::uint16_t, OS::common::uint16_t);
    PeripheralComponentInterconnectDeviceDescriptor();
    ~PeripheralComponentInterconnectDeviceDescriptor();
};
        
class PeripheralComponentInterconnectController{
    Port32Bit dataPort;
    Port32Bit commandPort;
    
public:
    PeripheralComponentInterconnectController();
    ~PeripheralComponentInterconnectController();
    
    //The read function will get the bus, device, and function information, and the offset of the information we want to access
    OS::common::uint32_t Read(OS::common::uint16_t bus, OS::common::uint16_t device, OS::common::uint16_t function, OS::common::uint32_t registeroffset);
    //the write function similar to the read function, but you are pushing the value you want to write
    void Write(OS::common::uint16_t bus, OS::common::uint16_t device, OS::common::uint16_t function, OS::common::uint32_t registeroffset, OS::common::uint32_t value);
    
    //check if a device has a function
    bool DeviceHadFunction(OS::common::uint16_t bus, OS::common::uint16_t device);

    void SelectDrivers(OS::drivers::DriverManager* driverManager, OS::hardwarecommunication::InterruptManager* interrupts);//a method to get the drivers by going through all the buses, drivers, and functions. and look for the class, subclass, vendor, device ID's. And put these drivers in the Driver manager
    
    OS::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, OS::hardwarecommunication::InterruptManager* interrupts); //create a function to get a driver with the paramters of the device descriptor and the interrupt manager
    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(OS::common::uint16_t bus, OS::common::uint16_t device, OS::common::uint16_t function);//get the description of a device, depending on the bus, device and function
    
    //gives us a instanct of the base register class
    //bar is the base address register
    BaseAddressRegister GetBaseAddressRegister(OS::common::uint16_t bus, OS::common::uint16_t device, OS::common::uint16_t function, OS::common::uint16_t bar);
    
    
    
};
    
    
    
    
    
    }
}

#endif
