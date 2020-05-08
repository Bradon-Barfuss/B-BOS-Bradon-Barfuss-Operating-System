#include "interrupt.h"
#include "/home/bradon/OS/OSV/std/print.h"


void printf(char* str);

InterruptHandler::InterruptHandler(uint8_t InterruptNumber, InterruptManager* interruptManager){
    this->InterruptNumber = InterruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[InterruptNumber] = this;
}

InterruptHandler::~InterruptHandler()
{
    if(interruptManager->handlers[InterruptNumber] == this){
        interruptManager->handlers[InterruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}



InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];//set 256 gate Descriptors

InterruptManager* InterruptManager::ActiveInterruptManager = 0;



void InterruptManager::SetInterruptDescriptorTableEntry(//this is the set the entry in the table
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilageLevel,
    uint8_t DescriptorType
){
    
    
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;  //set the handler address low bits,
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;//shift 16 bits (or 2 bytes) to the left and then set it
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
        const uint8_t IDT_DESC_PRESENT = 0x80;//look up this

    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilageLevel&3) << 5);//look up info about why we set the access byte like this
    interruptDescriptorTable[interruptNumber].reserved = 0; //teh reserved byte is normally always zero
}
    
InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
: picMasterCommand(0x20), //set port to 0x20
picMasterData(0x21), //set PIC data to port 0x21
picSlaveCommand(0xA0), //set PIC slave to port A0
picSlaveData(0xA1){ //set slave data to port A1
    
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    
    for(uint16_t i = 0; i < 256; i++){ //loop through to set all 256 entrys
        handlers[i] = 0;//set the handlers to zero
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);//(interruptNumber, codesegment offset, handler pointer, privilage 0 or kernel space, Descriptor types is interupt gate)
    }
    handlers[0] = 0;
    //when we call interupt 0x20, it will jump into the &HandleInterruptRequest0x00 in interrupt.h. It will then go into a macro with the IQR_Base(20) offset.
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);//is interupt number 0x00, + the IQR base(20) offset making it interupt number 0x20 HandleInterruptRequest0x00
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

    //before we use the interruptDescriptorTable we tell the processor to communicate with the picMasterCommand

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);
    
    picMasterData.Write(0x20);//if the processor get a interrupt. add 0x20 to master pic
    picSlaveData.Write(0x28);//if the processor get a interrupt. add 0x28 to slave pic

    picMasterData.Write(0x04);//look up why we need these
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    
    //we are now telling the processor to use this table
    interruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;//set the size of the IDT
    idt.base = (uint32_t)interruptDescriptorTable;//a pointer to the IDT (one of the very top lines of interrupt.cpp)
    asm volatile("lidt %0" : : "m" (idt));//load interupt discriptor tbale
    
    //all of the code is used to set the interrupt table

}


InterruptManager::~InterruptManager(){
}

void InterruptManager::Deactivate(){
    if(ActiveInterruptManager == this){
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

void InterruptManager::Activate(){
    if(ActiveInterruptManager != 0){
        ActiveInterruptManager->Deactivate();//decativate the activeinterruptmanager if it already been set
    }
    ActiveInterruptManager = this;
    asm("sti");//start interupt
    
        
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp){
    if(ActiveInterruptManager != 0){
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp){//this is for stack switching. When we call a handler, we will get the interupt and put the program informaiton in the esp pointer
    
    
    if(handlers[interruptNumber] != 0){//if we have a handler
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if(interruptNumber != 0x20){
        char* foo = "Unhandled Interrupt 0x00\0";
        char* hex = "0123456789ABCDEF";
        foo[22] = hex[(interruptNumber >> 4) & 0x0F];
        foo[23] = hex[(interruptNumber) & 0x0F];
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
        printf(foo);
    }
    
    
    if(0x20 <= interruptNumber && interruptNumber < 0x30){//we only need to answers to the hard ware interupts
        //we have remaped the hardware interrupts to the pic to 0x00 to 0x20
        picMasterCommand.Write(0x20);//this is the answer the PIC wants
        if(0x28 <= interruptNumber){//only send a pic answer if it came from the slave
            picSlaveCommand.Write(0x20);
        }
    }
        
        
    return esp; //just return the stack pointer for now
};
