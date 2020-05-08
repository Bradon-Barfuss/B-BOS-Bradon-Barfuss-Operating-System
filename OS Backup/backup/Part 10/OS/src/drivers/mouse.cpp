//need to move the mouse and keyboard out of the driver, see end of video 7


#include <drivers/mouse.h>

using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;

void printf(char*);


MouseEventHandler::MouseEventHandler(){}

void MouseEventHandler::OnActivate(){}
void MouseEventHandler::OnMouseDown(uint8_t button){}
void MouseEventHandler::OnMouseUp(uint8_t button){}
void MouseEventHandler::OnMouseMove(int x, int y){}

//THE WHOLE PROBLEM WAS THE UPPERCASE H on MouseEventHandler* (H)andler, where it should of been (h)andler. The mouse would not load, and the OS would crash after I interact with the mouse. I couldn't use ctrl alt to escape the mouse, I had to wait for it to crash
MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager), //0x2C is the mouse interupt
dataport(0x60),
commandport(0x64){
    this->handler = handler;
}


MouseDriver::~MouseDriver(){
    
}

void MouseDriver::Activate(){
    offset = 0;
    buttons = 0;
    
      if(handler != 0){
            handler->OnActivate();
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

uint32_t MouseDriver::HandleInterrupt(uint32_t esp){//GOOD

    uint8_t status = commandport.Read();
    if(!(status & 0x20)){//on if the 6 bit  is one, there is data
        return esp;
    }

    buffer[offset] = dataport.Read();

    if(handler == 0){//on if the 6 bit  is one, there is data
        return esp;
    }
    
    offset = (offset+1) % 3;

    if(offset == 0){
        //when the mouse moves, add to the x-axis, subtract to the y (y is inverted)
        if(buffer[1] != 0 || buffer[2] != 0){
            handler->OnMouseMove(buffer[1], -buffer[2]);//the negitive in the buffer[2] is the y, we want to subtract, not add in Mouse Handler
        }
        
        for(uint8_t i = 0; i < 3; i++)
        {
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
            {
                if(buttons & (0x1<<i))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
        }
        buttons = buffer[0];
            
        }
        return esp;
}


    
