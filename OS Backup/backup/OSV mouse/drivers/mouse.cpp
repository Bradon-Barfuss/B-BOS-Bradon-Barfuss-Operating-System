#include "mouse.h"

void printf(char*);

MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager), //0x2C is the mouse interupt
dataport(0x60),
commandport(0x64){
    
    offset = 0;
    buttons = 0;
    
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;


    VideoMemory[80*12+40] = ((VideoMemory[80*12+40] & 0xF000) >> 4)
        | ((VideoMemory[80*12+40] & 0x0F00) << 4)
        | ((VideoMemory[80*12+40] & 0x00FF));
    
    while(commandport.Read() & 0x1){//if you hold down a key, it will just wait for you to stop pressing it down
        dataport.Read();
    }
    commandport.Write(0xA8);//tell the pic to start sending mouse interupts
    //command 0x20 will get the current state, and the we changes it current state with status
    commandport.Write(0x20);// ask the pic for its current state
    uint8_t status = dataport.Read() | 2; //set status rightmost bit to 1, becuase that will be the newest state and clear the 5th bit
    commandport.Write(0x60); //tell the keyboard to changes it state
    dataport.Write(status);
    
    
    commandport.Write(0xD4);
    dataport.Write(0xF4);//activate the mouse
    dataport.Read();
    
    
}


MouseDriver::~MouseDriver(){
    
}


uint32_t MouseDriver::HandleInterrupt(uint32_t esp){

    uint8_t status = commandport.Read();
    if(!(status & 0x20)){//on if the 6 bit  is one, there is data
        return esp;
    }

    
    static int8_t x=40, y=12;
    
    buffer[offset] = dataport.Read();
    offset = (offset+1) % 3;
    
    
    if(offset == 0){
        
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        //when the mouse moves, add to the x-axis, subtract to the y (y is inverted)
        
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
            | ((VideoMemory[80*y+x] & 0x0F00));
                
        x += buffer[1];
        if(x < 0){x = 0;}//keeps the mouce leaving the screen
        if(x > 80){x=79;}
        
        y -= buffer[2];
        
        if(y < 0){y = 0;}//keeps the mouce leaving the screen
        if(y >= 25){y=24;}
        
                
        VideoMemory[80*y+x] = ((VideoMemory[80*y+x] & 0xF000) >> 4)
                            | ((VideoMemory[80*y+x] & 0x0F00) << 4)
                            | ((VideoMemory[80*y+x] & 0x00FF));
    }
    uint8_t key = dataport.Read();//if there is a key stike we will need to fetch it
    
    



    
    
    return esp;
}
