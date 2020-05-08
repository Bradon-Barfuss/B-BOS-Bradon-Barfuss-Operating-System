#ifndef __OS__DRIVERS__VGA_H
#define __OS__DRIVERS__VGA_H


#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupt.h>
#include <hardwarecommunication/port.h>

//the VGA has 11 ports

namespace OS{
    namespace drivers{

        
     
class VideoGraphicsArray{
protected:
    //intialize all 11 ports
    hardwarecommunication::Port8Bit miscPort;
    hardwarecommunication::Port8Bit crtcIndexPort;
    hardwarecommunication::Port8Bit crtcDataPort;
    hardwarecommunication::Port8Bit sequencerIndexPort;
    hardwarecommunication::Port8Bit sequencerDataPort;
    hardwarecommunication::Port8Bit graphicsControllerIndexPort;
    hardwarecommunication::Port8Bit graphicsControllerDataPort;
    hardwarecommunication::Port8Bit attributeControllerIndexPort;
    hardwarecommunication::Port8Bit attributeControllerReadPort;
    hardwarecommunication::Port8Bit attributeControllerWritePort;
    hardwarecommunication::Port8Bit attributeControllerResetPort;


    void WriteRegisters(OS::common::uint8_t* registers);//write teh registers
    OS::common::uint8_t* GetFrameBufferSegment();//get the segment where the additional memory can be access
    
    virtual OS::common::uint8_t GetColorIndex(OS::common::uint8_t r, OS::common::uint8_t g,OS::common::uint8_t b);//get the color index for a rgb color. It will be called by putpixel


public:
    VideoGraphicsArray();
    ~VideoGraphicsArray();
    
    virtual bool SetMode(OS::common::uint32_t width, OS::common::uint32_t height, OS::common::uint32_t colordepth);//set the mode
    virtual bool SupportsMode(OS::common::uint32_t width, OS::common::uint32_t height, OS::common::uint32_t colordepth);//check if the mode is supported
    virtual void PutPixel(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);//put the 24 bit colors (r,g,b) to the x and y positions
    virtual void PutPixel(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t colorTable);//we are using a 8-bit version right now, so we will use a color table, that has 256 different entrys that we can pull from and use.
    
    virtual void FillRectangle(OS::common::uint32_t x, OS::common::uint32_t y, OS::common::uint32_t h, OS::common::uint32_t w,  OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);
    virtual void Line(OS::common::uint32_t x, OS::common::uint32_t y, OS::common::uint32_t h, OS::common::uint32_t w, OS::common::uint32_t br, OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);
    void PutText();
    
};




        
        
    }
}

#endif
