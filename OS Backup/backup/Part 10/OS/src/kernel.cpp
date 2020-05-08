//#include "std/print.h"
//#include "std/len.h"

#include <gdt.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <hardwarecommunication/interrupt.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/pci.h>

using namespace OS;
using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;


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
         //   clearScreen();
            x = 0;
            y = 0;
        }
    }
}

void printfposition(char* str, uint8_t x, uint8_t y){ // This is a printf function, taking the pointer or position of the current char 
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000

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
//            clearScreen();
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key){
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[(key) & 0x0F];
    printf(foo);
}




//hard ware


//keyboard

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
public:
    void OnKeyDown(char c){
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};


//mouse

class MouseToConsole : public MouseEventHandler{
    int8_t x, y;
public:
    
    MouseToConsole(){}
    
    virtual void OnMouseDown(int button){
    }
    
    virtual void OnActivate(){
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
            | ((VideoMemory[80*y+x] & 0x00FF));
    }
    
    
    
    
    virtual void OnMouseMove(int xoffset, int yoffset){        
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));
                
        x += xoffset;
        if(x < 0){x = 0;}//keeps the mouce leaving the screen
        if(x > 80){x=79;}
        
        y += yoffset;//y offset is negitive, so you would add it.
        
        if(y < 0){y = 0;}//keeps the mouce leaving the screen
        if(y >= 25){y=24;}
        
                
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));
                        
    }
};



void startUp(){
    
    char* welcome = "Welcome to Bradons OS";
    
    printfposition(welcome, 25, 4);
    
    char* copyright = "This is the property of bradon barfuss";
    printfposition(copyright, 15, 7);
    char* instructions  = "Please Wait";
    printfposition(instructions, 30, 12);
}




//the normal start of kernal
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){//start of the kernel and the "extern C" is used for c++ to work with gcc 

    printf("1\0");//the space, '\f' and '\0' count as one char
    //char len = l.length("abcdefghijklmnopqrstuvwxyz\0");


    
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);//happens here
    
//    clearScreen();
    printf("Stage 1\n");

    DriverManager drvManager;

        MouseToConsole mousehandler;
        MouseDriver mouse(&interrupts, &mousehandler);
        drvManager.AddDrivers(&mouse);


        PrintfKeyboardEventHandler kbhandler;
        KeyboardDriver keyboard(&interrupts, &kbhandler);
        drvManager.AddDrivers(&keyboard);

        PeripheralComponentInterconnectController PCIcontroller;
        PCIcontroller.SelectDrivers(&drvManager);
        //Solved old bug
        //The keyboard board section was intiazlized first, before the mouse sections. Causing for some reason for the os to register the mouse, but pausing all interupts after pressing anything on the screen. I think this is because when you activate the CLI, we immedially do the STI command, which causes for memory loss and interrupts to fail see: https://en.wikipedia.org/wiki/Interrupt_flag, cli and sti section
    printf("Stage 2\n");
        drvManager.ActivateAll();

    printf("Stage 3\n");
    interrupts.Activate();

//    startUp();
    while(1);//infinit loop
}
