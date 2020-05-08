#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "/home/bradon/OS/OSV/types.h"
#include "port.h"
#include "/home/bradon/OS/OSV/gdt.h"

class InterruptManager{//created class called interrupt manager
    
protected:
    static InterruptManager* ActiveInterruptManager; //pointer to the interrupt manager class
    struct GateDescriptor{//are entrys to the interrupt table
        uint16_t handlerAddressLowBits;//the handler address low bit
        uint16_t gdt_codeSegmentSelector;//the gdt code segemnt offset, to read the info, the handler get the offset of the code segment
        uint8_t reserved;//extra reserved byte
        uint8_t access;//access byte
        uint16_t handlerAddressHighBits;//the handler address high bit
            } __attribute__((packed));
    
    static GateDescriptor interruptDescriptorTable[256];//set 256 possible interrupts
    
    struct interruptDescriptorTablePointer{//is to tell the processor to use the IDT
        uint16_t size;
        uint32_t base; 
            } __attribute__((packed));
    
    static void SetInterruptDescriptorTableEntry(//set the interrupt descriptor table
        uint8_t interruptNumber,//the number
        uint16_t codeSegmentSelectorOffset,//the code offset
        void (*handler)(),//the pointer to the handler
        uint8_t DescriptorPrivilageLevel, //the accesss rights
        uint8_t DescriptorType //flags
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
    
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);//get the interrupt number and the stack pointer
    uint32_t DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp);//a non static version of handler manager interrupt method
        
    static void IgnoreInterruptRequest();
    static void HandleInterruptRequest0x00();//timer interrupt
};
#endif
