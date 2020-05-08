#ifndef __OS__HARDWARECOMMUNICATION__INTERRUPT_H
#define __OS__HARDWARECOMMUNICATION__INTERRUPT_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>

namespace OS{
    namespace hardwarecommunication{
        
        
        
class InterruptManager;

class InterruptHandler
{
    //we don't have virtual methods so we can't have abstract methods, so we will make the constructor protected
protected:
    OS::common::uint8_t InterruptNumber;
    InterruptManager* interruptManager;
    InterruptHandler(OS::common::uint8_t InterruptNumber, InterruptManager* interruptManager);
    ~InterruptHandler();
public:
    virtual OS::common::uint32_t HandleInterrupt(OS::common::uint32_t esp);
};




class InterruptManager{//created class called interrupt manager
        friend class InterruptHandler;
protected:
    static InterruptManager* ActiveInterruptManager; //pointer to the interrupt manager class, so we can access it after the interruptstubes
    InterruptHandler* handlers[256]; //handler is a pointer to the interrupthandle class
    
    
    struct GateDescriptor{//are entrys to the interrupt table
        OS::common::uint16_t handlerAddressLowBits;//the handler address low bit
        OS::common::uint16_t gdt_codeSegmentSelector;//the gdt code segemnt offset, to read the info, the handler get the offset of the code segment
        OS::common::uint8_t reserved;//extra reserved byte
        OS::common::uint8_t access;//access byte
        OS::common::uint16_t handlerAddressHighBits;//the handler address high bit
            } __attribute__((packed));
    
    static GateDescriptor interruptDescriptorTable[256];//set 256 possible interrupts
    
    struct interruptDescriptorTablePointer{//is to tell the processor to use the IDT
        OS::common::uint16_t size;
        OS::common::uint32_t base; 
            } __attribute__((packed));
    
    static void SetInterruptDescriptorTableEntry(//set the interrupt descriptor table
        OS::common::uint8_t interruptNumber,//the number
        OS::common::uint16_t codeSegmentSelectorOffset,//the code offset
        void (*handler)(),//the pointer to the handler
        OS::common::uint8_t DescriptorPrivilageLevel, //the accesss rights
        OS::common::uint8_t DescriptorType //flags
    );
    
    //we need to tell the PIC to send us the interrupts we do this by
    //there a two pic's, a master PIC and a slave PIC
    //The master PIC is for
    //The slave PIC is for
    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

    
    
    
public:
    InterruptManager(GlobalDescriptorTable* gdt);//get DlobalDescriptotr class info
    ~InterruptManager();//deconstruct interruptmanager

    void Activate();
    void Deactivate();
    
    static OS::common::uint32_t handleInterrupt(OS::common::uint8_t interruptNumber, OS::common::uint32_t esp);//get the interrupt number and the stack pointer
    OS::common::uint32_t DoHandleInterrupt(OS::common::uint8_t interruptNumber, OS::common::uint32_t esp);//a non static version of handler manager interrupt method
        
    static void IgnoreInterruptRequest();
//     static void HandleInterruptRequest0x00();//timer interrupt
//     static void HandleInterruptRequest0x01();//timer interrupt
//     static void HandleInterruptRequest0x0C();
    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x02();
    static void HandleInterruptRequest0x03();
    static void HandleInterruptRequest0x04();
    static void HandleInterruptRequest0x05();
    static void HandleInterruptRequest0x06();
    static void HandleInterruptRequest0x07();
    static void HandleInterruptRequest0x08();
    static void HandleInterruptRequest0x09();
    static void HandleInterruptRequest0x0A();
    static void HandleInterruptRequest0x0B();
    static void HandleInterruptRequest0x0C();
    static void HandleInterruptRequest0x0D();
    static void HandleInterruptRequest0x0E();
    static void HandleInterruptRequest0x0F();
    static void HandleInterruptRequest0x31();

    static void HandleException0x00();
    static void HandleException0x01();
    static void HandleException0x02();
    static void HandleException0x03();
    static void HandleException0x04();
    static void HandleException0x05();
    static void HandleException0x06();
    static void HandleException0x07();
    static void HandleException0x08();
    static void HandleException0x09();
    static void HandleException0x0A();
    static void HandleException0x0B();
    static void HandleException0x0C();
    static void HandleException0x0D();
    static void HandleException0x0E();
    static void HandleException0x0F();
    static void HandleException0x10();
    static void HandleException0x11();
    static void HandleException0x12();
    static void HandleException0x13();
    
    
};



    }
}
#endif
