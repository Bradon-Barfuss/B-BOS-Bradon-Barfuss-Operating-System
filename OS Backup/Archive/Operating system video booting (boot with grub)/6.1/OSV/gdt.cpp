#include "gdt.h"
#include "/home/bradon/OS/OSV/std/printf.h"

/*the compiler is expecting 32 bit or 8 bytes of code, but we have to sepearte them into low and high bytes
 * to fit in the GDT, we will first set the GlobalDescriptorTable with the following functions
 * 
 * terms: the base  address is where in the CPU-addressable space the segment starts
 * terms a limit is the last segment-reltive address that can be accessed offset from the base
 * 
 * Info about limits
 * https://wiki.osdev.org/Segment_Limits
 */

/*The GDT sections
 * byte 0 to 15 or the first 2 bytes are for the limit
 * the 16 to 39 bits or the next 3 bytes are for the base
 * The 40 to 47 bites or next byte is for the access byte
 * The bit 48 to 51 is 4 bits or half a byte added for the limit
 * The bits 52 to 55 is a half bytes for flags
 * and the last 56 to 63 bits or 1 bytes is for the base.
 * 
 * This can also be seen as setting a variable called uint8_t target, (uint8_t allows for 8 bytes to be set)
 * Target[0] = limit
 * Target[1] = limit
 * Target[2] = base
 * Target[3] = base
 * Target[4] = base
 * Target[5] = Flags (techicly access bytes, but we are not doing that)
 * Target[6 first 4 bits] = flags
 * Target[6 last 4 bits] = extra limit
 * Target[7] = base
 * https://wiki.osdev.org/Global_Descriptor_Table
 * 
 * Other things that you should know
 * 
 * if VAR = uint_8: then it can hold 8 bits at a time
 * If VAR = uint_32: then it can hold 32 bits at a time
 * If uint_8 VAR = 0x##: We would set all 8 bits or a byte
 * if Uint_8 VAR |= 0x#: we would add a half byte or 4 bits
 * If Uint_32 VAR = 0x###: we would set 12 bits or 1.5 bytes
 */
GlobalDescriptorTable::GlobalDescriptorTable()://setting the global GlobalDescriptorTable permaerters
nullSegmentSelector(0,0,0),//this is never reference by the processor, but emulators my complain not having this.
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024, 0x9A),//starts at byte zero, size is 64mb, flag 0x9A saying that this is the code segment (I think)
dataSegmentSelector(0,64*1024*1024, 0x92){//sets data seggment size and you can't write to a data segment so you use the 0x92 flag
//Because of the 
    uint32_t i[2];//Because the process is expecting 8 bytes, we need to speartate them into 2 so we can define them
    i[1] = (uint32_t)this;//the address of the table
    i[0] = sizeof(GlobalDescriptorTable) << 16;//the last four bytes are the high bytes of the section, moved by 4 bytes to be the high bytes
    
    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));//tell the processor to use the GDT
    
    
}

GlobalDescriptorTable::~GlobalDescriptorTable(){
    
}

uint16_t GlobalDescriptorTable::DataSegmentSelector(){//gives us the offset
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;

}

uint16_t GlobalDescriptorTable::CodeSegmentSelector(){//gives us the offset of the code segment
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t flags){
    uint8_t* target = (uint8_t*)this;
    
    if(limit <= 65536){//if limit is 16 bit
        target[6] = 0x40; //set the sixth byte to tell the process that we are in 16-bit code; The 6th bytes is where the extra limit is
    /*We are currently in 32 bit mode, so there is 4 bytes, this will make more sence why i added this later
     * the limit in the first part of the GDT and 16 bits or 2 bytes for the limit, which is not enought for the large memory sections
     * To add more space, that added a limit after the access sector. The added a half byte limit must be multiplied by the 2^12, so 
     * the 2^12 virtale bits. ||This means that there are 12 extra bits that we can set|| But this only works if the last bits of the 32-bits 
     * were only 1's. This pretty much means that when we set 2 1/2 bytes of the limit, the last 1 1/2 bytes (12 bits) must be one.
     * We do this to fill up the 32bit limit to be full
     * 
     * The next code will check if the last 1 1/2 bytes are one, if not, we will decrease the limit section by one and increase the last 1 1/2
     * bits by 1, which is some how a legal way of setting all the last bytes to 1
     * 
     * see Write you own oeprating system 3: Memory segments and GDT. starts at 30:00 https://www.youtube.com/watch?v=pfWjteMpcxE&t=1947s
     */
    } else{//this is used to set the last 12 bits to 1
        if((limit & 0xFFF) != 0xFFF){//if the last 12 bits of the limit not all 1
            limit = (limit >> 12)-1;//we will shift the limit by 12 and subtract 1. 
        } else {
            limit = (limit >> 12);
        }
        
        target[6] = 0xC0; //We will set this to 0xC0 to see if we have added the extra 1's at the end of the limit segment
    }
    
    //we will now set distribute the limit into the correct places
    //the last byte is going to be the lest signlift bits part of the limit
    target[0] = limit & 0xFF;//this is setting the first 2 bytes of the limit
    target[1] = (limit >> 8) & 0xFF;//are the next 8 bits of the limit
    target[6] |= (limit >> 16) & 0xF;//shift the limit by 16 and set the half byte to 0xF
    //the |= is telling that half of the byte will be decleared
    
    target[2] = base & 0xFF;//This will set the pointer or the base of the GlobalDescriptorTable 
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF; 
    
    target[5] = flags;//this is also acting as the access bytes
    
}

/* Some times we need to look up the pointer while in the Global_Descriptor_Table
 * So we will do the same thing backwards, need to take corresponding bytes
 *  if we request the base and limit
 * 
 * If we have changed the limit, we can't keep it in a variable,
 * So we will have to compute it again 
 * 
 * The Base() and Limit() will take the base and limit information from the GDT and put them into results
 * It will set a target and add a (limit or base) byte in it. The it will shift the target pointer(where information will be 
 * put into target) to the left 8 bits. Then add the next byte into the newly created space.
*/

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base(){//get the base
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[7];//take the seveth byte of target (which has a base byte)
    result = (result << 8) + target[4];//move the results 8 bits to the left, and add target[4] (which is a base byte]
    result = (result << 8) + target[3];//move the results point 8 bits to the left again, and the imput the next base byte into the knewly added space
    result = (result << 8) + target[2];//add base byte to results
    return result;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit(){//get the limit
    uint8_t* target = (uint8_t*)this;
    uint32_t result = target[6] & 0xF;//take the low 4 bits of byte 6
    result = (result << 8) + target[1];//take the 
    result = (result << 8) + target[0];
    
    //we need to add the extra 1's at the end of the results to make it legal
    if((target[6] & 0xC0) == 0xC0){ //look up why we need the extra '& 0xC0'
        result = (result << 12) | 0xFFF;
    }
    return result;
}
 
