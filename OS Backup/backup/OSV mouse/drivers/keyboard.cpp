#include "keyboard.h"

void printf(char*);

KeyboardDrive::KeyboardDrive(InterruptManager* manager)
: InterruptHandler(0x21, manager), //0x21 is the keyboard interupt
dataport(0x60),
commandport(0x64){
    while(commandport.Read() & 0x1){//if you hold down a key, it will just wait for you to stop pressing it down
        dataport.Read();
    }
    commandport.Write(0xAE);//tell the pic to start sending keyboard interupts
    
    //command 0x20 will get the current state, and the we changes it current state with status
    commandport.Write(0x20);// ask the pic for its current state
    uint8_t status = (dataport.Read() | 1) & ~0x10; //set status rightmost bit to 1, becuase that will be the newest state and clear the 5th bit
    commandport.Write(0x60); //tell the keyboard to changes it state
    dataport.Write(status);
    
    
    dataport.Write(0xF4);//activate the keyboard
}


KeyboardDrive::~KeyboardDrive(){
    
}


uint32_t KeyboardDrive::HandleInterrupt(uint32_t esp){
    
    uint8_t key = dataport.Read();//if there is a key stike we will need to fetch it
    
    //when you press a key, a interrupt will be sent when you press down and when you realease, causing two interrupt to appear, so if the interrupt is less than 80, it is a press down interrupt
    if(key < 0x80){
        switch(key){
            

            case 0x02: printf("1"); break;
            case 0x03: printf("2"); break;
            case 0x04: printf("3"); break;
            case 0x05: printf("4"); break;
            case 0x06: printf("5"); break;
            case 0x07: printf("6"); break;
            case 0x08: printf("7"); break;
            case 0x09: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;
            case 0x1E: printf("a"); break;
            case 0x30: printf("b"); break;
            case 0x2E: printf("c"); break;
            case 0x20: printf("d"); break;
            case 0x12: printf("e"); break;
            case 0x21: printf("f"); break;
            case 0x22: printf("g"); break;
            case 0x23: printf("h"); break;
            case 0x17: printf("i"); break;
            case 0x24: printf("j"); break;
            case 0x25: printf("k"); break;
            case 0x26: printf("l"); break;
            case 0x31: printf("n"); break;
            case 0x32: printf("m"); break;
            case 0x18: printf("o"); break;
            case 0x19: printf("p"); break;
            case 0x10: printf("q"); break;
            case 0x13: printf("r"); break;
            case 0x1F: printf("s"); break;            
            case 0x14: printf("t"); break;
            case 0x16: printf("u"); break;
            case 0x11: printf("w"); break;
            case 0x2D: printf("x"); break;
            case 0x2F: printf("v"); break;
            case 0x15: printf("y"); break;
            case 0x2C: printf("z"); break;
            case 0x33: printf(","); break;
            case 0x34: printf("."); break;
            case 0x35: printf("-"); break;

            case 0x1C: printf("\n"); break;
            case 0x39: printf(" "); break;

                
            default:
                char* foo = " KEYBOARD 0x21\0";
                char* hex = "0123456789ABCDEF";
                foo[11] = hex[(key >> 4) & 0xF];
                foo[12] = hex[(key) & 0x0F];
                printf(foo);
                break;
        }
    }


    
    
    return esp;
}
