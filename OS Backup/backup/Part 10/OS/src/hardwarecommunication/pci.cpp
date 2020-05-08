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
void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager){//a method to get the drivers by going through all the buses, drivers, and functions. and look for the class, subclass, vendor, device ID's. And put these drivers in the Driver manager
    
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
                        break;
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










