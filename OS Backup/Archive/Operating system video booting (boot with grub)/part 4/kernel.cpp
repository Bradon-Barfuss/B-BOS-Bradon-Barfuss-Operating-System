#include "types.h"
#include "gdt.h"
#include "printf.h"


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){//start of the kernel and the "extern C" is used for c++ to work with gcc 
    clearScreen();
    printf("hello, \0");//the space, '\f' and '\0' count as one char
    GlobalDescriptorTable gdt;

    while(1);//infinit loop
}
