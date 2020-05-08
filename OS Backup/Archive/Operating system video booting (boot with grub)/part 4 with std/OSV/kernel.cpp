#include "types.h"
#include "gdt.h"
#include "std/printf.h"
#include "std/len.h"


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){//start of the kernel and the "extern C" is used for c++ to work with gcc 
    clearScreen();
    len l;
    printf("abcdefghijklmnopqrstuvwxyz\0");//the space, '\f' and '\0' count as one char
    char len = l.length("abcdefghijklmnopqrstuvwxyz\0");
    GlobalDescriptorTable gdt;

    while(1);//infinit loop
}
