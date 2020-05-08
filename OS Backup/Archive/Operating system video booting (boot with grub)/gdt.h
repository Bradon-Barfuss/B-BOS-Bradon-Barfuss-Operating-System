#ifndef __GDT_H //if the ram already has this info, don't add it I THINK
#define __GDT_H

#include "types.h";

//the 8, 16, and 32 could all mean bytes Need more Info
class GlobalDescriptorTable{
    public:
        class SegmentDescriptor{//This is where we will descripe each segment
        private:
            uint16_t limit_lo; //unsigned 16-bit int, 2 low bytes
            uint16_t base_lo; //The 2 low bytes of the pointer 
            uint8_t base_hi; //1 byte extention for the pointer
            uint8_t type; //Access byte
            uint8_t flags_limit_hi; //flags high byte
            uint8_t base_vhi;
            
        public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);//take in 32bit input and seperates them into the descripter table
            uint32_t Base();
            uint32_t Limit();
            
        } __attribute__((packed));//this is used to tell the compiler not to mess with the call segment descriptor, because everything needs to be byte perfect
        
    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;
    
    public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();
        
    uint16_t CodeSegmentSelector();//Method used to get the offset of the code segment descriptor
    uint16_t DataSegmentSelector();//Method used to get the offset of the data segment descriptor
};



#endif
