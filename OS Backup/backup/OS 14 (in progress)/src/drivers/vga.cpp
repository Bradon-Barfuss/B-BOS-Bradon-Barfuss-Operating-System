#include <drivers/vga.h>

using namespace OS::common;
using namespace OS::drivers;
using namespace OS::hardwarecommunication;


VideoGraphicsArray::VideoGraphicsArray() :

    miscPort(0x3c2),//the mics port is 0x3c2
    crtcIndexPort(0x3d4),
    crtcDataPort(0x3d5),
    sequencerIndexPort(0x3c4),
    sequencerDataPort(0x3c5),
    graphicsControllerIndexPort(0x3ce),
    graphicsControllerDataPort(0x3cf),
    attributeControllerIndexPort(0x3c0),
    attributeControllerReadPort(0x3c1),
    attributeControllerWritePort(0x3c0),
    attributeControllerResetPort(0x3da){
        
        
        
}


    
VideoGraphicsArray::~VideoGraphicsArray(){
    
}


void VideoGraphicsArray::WriteRegisters(uint8_t* registers){//write the registers
    
    //MICS
    miscPort.Write(*(registers++));//write the first register, Which is the misc port. and move to the next register
    
    //SEQ
    for(uint8_t i = 0; i < 5; i++){
        sequencerIndexPort.Write(i);//where do we want to write the data
        sequencerDataPort.Write(*(registers++));//the data we want to write
    }
    
    //Cathode Ray Tube Controller 
    
    //because the crtc can be damage by sending some data there, you need to unlock it the lock it again
    crtcIndexPort.Write(0x03);//unlock it
    crtcDataPort.Write(crtcDataPort.Read() | 0x80);//we get the first old value and set it to 1, and write it back to the index
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);//for the 11th (17th really) we need to set the first bit to zero
    
    
    //to make sure we don't over ride the CRTC, we do this LOOK UP WHY
    registers[0x03] = registers[0x03] | 0x80; //we set the first bit in register at 0x03 to one
    registers[0x11] = registers[0x11] | ~0x80; //we set the first bit in register at 0x11 to zero
    
    
    for(uint8_t i = 0; i < 25; i++){
        crtcIndexPort.Write(i);//where do we want to write the data
        crtcDataPort.Write(*(registers++));//the data we want to write
    }
    
    //graphics controller
    for(uint8_t i = 0; i < 9; i++){
        graphicsControllerIndexPort.Write(i);//where do we want to write the data
        graphicsControllerDataPort.Write(*(registers++));//the data we want to write
    }
    
    //attribute controller
    for(uint8_t i = 0; i < 21; i++){
        
        //we will reset the attribute controller before we send data
        attributeControllerResetPort.Read();
        
        
        attributeControllerIndexPort.Write(i);//where do we want to write the data
        attributeControllerWritePort.Write(*(registers++));//the data we want to write
    }
    
    attributeControllerResetPort.Read();//we reset the attribute controller;
    attributeControllerIndexPort.Write(0x20);
    
    
}



bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth){//set the mode
    if(!SupportedMode(width, height, colordepth)){
        return false;
    }
    unsigned char g_320x200x256[] = {
    /* MISC */
        0x63,
    /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
    /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
    /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };
        
    WriteRegisters(g_320x200x256);
    return true;
}

bool VideoGraphicsArray::SupportedMode(uint32_t width, uint32_t height, uint32_t colordepth){//check if the mode is supported
    return width == 320 && height == 200 && colordepth == 8;
}



uint8_t* VideoGraphicsArray::GetFrameBufferSegment(){//get the segment where the additional memory can be access
    graphicsControllerIndexPort.Write(0x06);
    
    //we shift it by two because we are only intrested in bits 3 and 4 
    //we set all the other bits killed (0)
    uint8_t segmentNumber = ((graphicsControllerDataPort.Read() & (3<<2));
    
    //look up why we need this
    switch(segmentNumber){
        case 0<<2: return (uint8_t*)0x00000;
        case 1<<2: return (uint8_t*)0xA0000;
        case 2<<2: return (uint8_t*)0xB0000;
        case 3<<2: return (uint8_t*)0xB8000;
        default: break;
            
    }
}
void VideoGraphicsArray::PutPixel(int32_t x, int32_t y, uint8_t colorIndex){//we are using a 8-bit version right now, so we will use a color table, that has 256 different entrys that we can pull from and use.
    
    uint8_t* pixelAddress = GetFrameBufferSegment() + 320*y + x;
    *pixelAddress = colorIndex;
}
uint8_t VideoGraphicsArray::GetColorIndex(uint8_t r, uint8_t g,uint8_t b){//get the color index for a rgb color. It will be called by putpixel
    if(r == 0x00 && g == 0x00 && b==0x00){ return 0x00; } //black
    if(r == 0x00 && g == 0x00 && b==0xA8){ return 0x01; }//blue
    if(r == 0x00 && g == 0xA8 && b==0x00){ return 0x02; }//green
    if(r == 0x00 && g == 0x00 && b==0xA8){ return 0x04; }//red
    if(r == 0xFF && g == 0xFF && b==0xFF){ return 0x3F; }//white

    return 0x00;
}


void VideoGraphicsArray::PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b){//put the 24 bit colors (r,g,b) to the x and y positions
    PutPixel(x, y, GetColorIndex(r,g,b));
}

void VideoGraphicsArray::FillRectangle(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b){
    for(uint32_t Y = y; Y < y+h; Y++){
        for(uint32_t X = x; X < x+w; X++){
            PutPixel(X, Y, r, g, b);
        }
    }
}
    
