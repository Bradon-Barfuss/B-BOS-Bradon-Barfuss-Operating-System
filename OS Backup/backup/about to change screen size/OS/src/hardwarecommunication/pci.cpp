#include <hardwarecommunication/pci.h>

using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;

void printf(char*);
void printfHex(uint8_t);

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor(){}
PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor(){}
PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
: dataPort(0xCFC), //set the data port for the PCI PeripheralComponentInterconnectController
commandPort(0xCF8) //set the command port for the PCI PeripheralComponentInterconnectController
{
    
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController(){}


// We will combine the port, device, and function number into one identifier to send to the PCI
// An Identifier is a name that is assigned by the user for a program element such as variable, type, template, etc.
uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset){
    uint32_t id = 
    //B = bus, D = device, f = function r = registeroffset
    0x1 << 31 //set the first bit to 1: 1000 0000 0000 0000 0000 0000 0000 0000
    | ((bus & 0xFF) << 16) //it would look like 1000 0000 BBBB BBBB 0000 0000 0000 0000 
    //the & stands for AND, if both are 1, the result would be 1
    //the | stands for or, if anyone one is 1, the result would be 1
    | ((device & 0x1F) << 11) // it would look like 1000 0000 BBBB BBBB DDDD D000 0000 0000
    | ((function & 0x07) << 8) //it would look like 1000 0000 BBBB BBBB DDDD DFFF FFFF 0000 
    | (registeroffset & 0xFC); //it would look like 1000 0000 BBBB BBBB DDDD DFFF FFRR RRRR we lost information at FFRR, when it was FFFF
    commandPort.Write(id);//write to the commandPort, which will run the assemply port command
    uint32_t result = dataPort.Read();//read the output fromt the commandport
    return result >> (8* (registeroffset % 4));// the arithmic operator % stands for modulus, or the reminader
    //it will return the spefic section we want, depending on our current registeroffset
}


void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value){
    uint32_t id = 
    //B = bus, D = device, f = function R = registeroffset
    0x1 << 31 //set the first bit to 1: 1000 0000 0000 0000 0000 0000 0000 0000
    | ((bus & 0xFF) << 16) //it would look like 1000 0000 BBBB BBBB 0000 0000 0000 0000 
    //the & stands for AND, if both are 1, the result would be 1
    //the | stands for or, if anyone one is 1, the result would be 1
    | ((device & 0x1F) << 11) // it would look like 1000 0000 BBBB BBBB DDDD D000 0000 0000
    | ((function & 0x07) << 8) //it would look like 1000 0000 BBBB BBBB DDDD DFFF FFFF 0000 
    | (registeroffset & 0xFC); //it would look like 1000 0000 BBBB BBBB DDDD DFFF FFRR RRRR we lost information at FFRR, when it was FFFF
    commandPort.Write(id);//write to the commandPort, which will run the assemply port command
    dataPort.Write(value);
    //the difference between the read and write founctions is that we would read from the data port in the read function. 
    //Compared to the write function, where will ad write to the data port the new value
}

bool PeripheralComponentInterconnectController::DeviceHadFunction(uint16_t bus, uint16_t device){//check if a device has a function
    //to check if a device has a function, we would look at the memory address 0x0E, which is 0000 .... 0011 1000
    //and the 7th bit of that address would tell us if we have functions or not
    return Read(bus, device, 0, 0x0E) & (1<<7);//return the seveth bit on address 0x0E to see if we have functions
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(common::uint16_t bus, uint16_t device, uint16_t function){
    //get the description of a device, depending on the bus, device and function
    PeripheralComponentInterconnectDeviceDescriptor result;
    
    //copy the parampters to the result
    result.bus = bus;
    result.device = device;
    result.function = function;
    
    result.vendor_id = Read(bus, device, function, 0x00);//the vendor id is on the offset 00
    result.device_id = Read(bus, device, function, 0x02);//the device_id is on the offset 02
    
    result.class_id = Read(bus, device, function, 0x0b); //the class id is on the offset 0b
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);
    
    result.revision = Read(bus, device, function, 0x08);//the revision id is on the offset 08
    result.interrupt = Read(bus, device, function, 0x3c);
    return result;

}
void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager, OS::hardwarecommunication::InterruptManager* interrupts){//a method to get the drivers by going through all the buses, drivers, and functions. and look for the class, subclass, vendor, device ID's. And put these drivers in the Driver manager
    
    int drivercount = 0;
    for(int bus = 0; bus < 8; bus++){//loop through buses
        for(int device = 0; device < 32; device++){
            int numFunction = DeviceHadFunction(bus, device) ? 8 : 1;//if true return 8, if false return 1
            //the operator ? is a ternary operator. it's layout is like this <condition> ? <ture-case-code> : <False-case-code>
            for(int function = 0; function < numFunction; function++){
                //set the each device
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                
                //if the vendor id is all zero or 1, there is no vendor_id
                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF){
                        continue;//if there a function later on, we could skip them using break
                }
                
                
                for(int barNum = 0; barNum < 6; barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput)){//if the address is real or accuate
                        dev.portBase = (uint32_t)bar.address;
                    }
                    
                    Driver* driver = GetDriver(dev, interrupts);
                    if(driver != 0){//if we get a driver
                        driverManager->AddDrivers(driver);
                    }
                }
                
                printf("PCI BUS ");
                printfHex(bus & 0xFF);
                
                printf(", Device ");
                printfHex(device & 0xFF);
                
                printf(", Function ");
                printfHex(function & 0xFF);
                
                
                printf("= vendor ID:  ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                
                printf(", Device ID:  ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
                drivercount++;

            }
        }
    }
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    BaseAddressRegister result;

    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs){
        return result;//would be null because nothing been set
    }
    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar); //look at OS 11, 34:05
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;//ckeck the type of the base register
    uint32_t temp;
    
    if(result.type == MemoryMapping){
        switch((bar_value >> 1) & 0x3){
            case 0://32 bit mode
                break;
            case 1://20 bit mode
                break;
            case 2://64-bit mode
                break;
        }
    } else{//input output
        result.address = (uint8_t*)(bar_value & ~0x3);//cancel the last 2 bits
        result.prefetchable = false;
    }

    return result;
}

Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts){
    switch(dev.vendor_id){
        case 0x1022://AMD
            switch(dev.device_id){
                case 0x2000://driver for AM79c973
                    break;
            }
            break;
        case 0x8086: //intel
            break;
    }
    
    switch(dev.class_id){
        case 0x03://graphics
            switch(dev.subclass_id){
                case 0x00: //VGA
                    printf("VGA");
                    break;
            }
            break;
        }
    
    
    return 0;
}







