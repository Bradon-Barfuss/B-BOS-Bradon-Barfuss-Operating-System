#include "types.h"
#include "gdt.h"
#include "std/print.h"
#include "std/len.h"
#include "Interrupts/interrupt.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"


void printf(char* str){ // This is a printf function, taking the pointer or position of the current char 
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000
    static uint8_t x = 0, y = 0;

    for(int i = 0; str[i] != '\0'; ++i){ //Going to loop through the string until it finds a '\0'
        VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | str[i]; //it will set the videoMemory var to current char of the string
                                                                /*If we just set it to str[i], it will override the high byte where the color information is stored
                                                                So we need to combine the current High byte (VideoMemory[i] & 0xBB00) This work as comparing VideoMemory[i] with the bitwise symbole & (AND bitwise symbole) So 0xBB-- will stay the same and ox--00 will change by str[i] text colors */
        switch (str[i]){
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        if(x >= 80){
            y++;
            x = 0;            
        }
        if(y >= 25){
            clearScreen();
            x = 0;
            y = 0;
        }
    }
}


extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){//start of the kernel and the "extern C" is used for c++ to work with gcc 

    clearScreen();
    len l;
    printf("1\0");//the space, '\f' and '\0' count as one char
    //char len = l.length("abcdefghijklmnopqrstuvwxyz\0");


    
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);//happens here
    //works here
    
    KeyboardDrive keyboard(&interrupts);//start the keyboard interupts
    MouseDriver mouse(&interrupts);//start the keyboard interupts

    
    
 
    interrupts.Activate();
    
    //for(int i = 0; i < 10000000; i++){//put a wait between the interrupt
//    };
  //  printf("33\0");//the space, '\f' and '\0' count as one char
    


    //printf("a\0");
    while(1);//infinit loop
}
