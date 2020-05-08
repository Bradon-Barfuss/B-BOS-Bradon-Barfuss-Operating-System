#include <gdt.h>
#include <common/types.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <hardwarecommunication/interrupt.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/pci.h>
#include <gui/desktop.h>
#include <gui/widget.h>
#include <gui/window.h>


using namespace OS;
using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;
using namespace OS::gui;



void printf(char* str){ // This is a printf function, taking the pointer or position of the current char 
    uint16_t* VideoMemory = (uint16_t*)0xb8000;//the memory location of where the computer will start printing char to the screen is 0xb8000
    static uint8_t x = 0, y = 0;

    for(int i = 0; str[i] != '\0'; ++i){ //Going to loop through the string until it finds a '\0'
        VideoMemory[(80*y)+x] = (VideoMemory[(80*y)+x] & 0xFF00) | str[i]; //it will set the videoMemory var to current char of the string
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
    MouseToConsole(){
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


//the normal start of kernal
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber){//start of the kernel and the "extern C" is used for c++ to work with gcc 

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);//happens here

    Desktop desktop(320,200,0x00,0x00,0xA8);

    DriverManager drvManager;
        
    //PrintfKeyboardEventHandler kbhandler;
    //KeyboardDriver keyboard(&interrupts, &kbhandler);
    KeyboardDriver keyboard(&interrupts, &desktop);
    drvManager.AddDrivers(&keyboard);
    
    
    //MouseToConsole mousehandler;
    //MouseDriver mouse(&interrupts, &mousehandler);
    MouseDriver mouse(&interrupts, &desktop);
    drvManager.AddDrivers(&mouse);
    
    PeripheralComponentInterconnectController PCIcontroller;
    PCIcontroller.SelectDrivers(&drvManager, &interrupts);
    
    VideoGraphicsArray vga;
    
    
    printf("Stage 2\n");
    drvManager.ActivateAll();

    vga.SetMode(320,200,8);
    
    Window win1(&desktop, 150,10,150,125,0xFF,0xFF,0xFF);
    desktop.AddChild(&win1);
    

    
    Window win2(&desktop, 290,13,7,7,0xA8,0x00,0x00);
    desktop.AddChild(&win2);
        

    printf("Stage 3\n");
    interrupts.Activate();

       // desktop.Draw(&vga);

    //while(1){
    desktop.Draw(&vga);

    //}
}
