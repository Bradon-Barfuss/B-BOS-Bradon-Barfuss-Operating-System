#include <gui/desktop.h>
#include <common/graphicscontext.h>
#include <drivers/mouse.h>


using namespace OS;
using namespace OS::common;
using namespace OS::gui;
    

Desktop::Desktop(int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
: CompositeWidget(0,0,0, w,h,r,g,b),
  MouseEventHandler(){
    MouseX = w/2;//center
    MouseY = h/2;//center
    //set CurrentPixelMouse to (MouseX, MouseY)
}

Desktop::~Desktop(){
    
}

void Desktop::Draw(OS::common::GraphicsContext* gc){
    CompositeWidget::Draw(gc);
    //set intial oldmousepixels (replace putpixel with oldmousepixels
}

void Desktop::OnMouseDown(uint8_t button){
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
}

void Desktop::OnMouseUp(uint8_t button){
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);

}

void Desktop::OnMouseMove(int x, int y){
    x /= 8;
    y /= 8;
    
    //get graphics GraphicsContext
    
    //get new mouse pixels (for(x&y) < 4) : NewPixelMouse[x][y] = getPixel(MouseX+x, Mouse Y)..etc
    
    int32_t newMouseX = MouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= w) newMouseX = w - 1;    
    
    int32_t newMouseY = MouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= h) newMouseY = h - 1;
    

    //drawMouse(int NewPixelMouse[][]
    
    //oldmousepixels = newpixelmouse
    
    
    uint8_t pixelHoldArray[20][20];
    Widget* i = CompositeWidget::findscreen(MouseX, MouseY);
    for(int row = 0; row < 9; row++){
        for(int col = 0; col < 9; col++){
            if(localgc->getPixel(MouseX+row-3, MouseY+col-3) != 0x02)
                pixelHoldArray[row][col] = localgc->getPixel(newMouseX+row+3,newMouseY+col+3);
        }
    }
    

    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);

    for(uint32_t row = 0; row < 3; row++){
        for(uint32_t col = 0; col < 3; col++){
            localgc->PutPixel(MouseX+row, MouseY+col, pixelHoldArray[row][col]);
        }
    }
    
    MouseX = newMouseX;
    MouseY = newMouseY;
    DrawMouse(MouseX, MouseY);

}

void Desktop::DrawMouse(int32_t MouseX, int32_t MouseY){
    localgc->DisplayMouse(MouseX, MouseY);
}

void Desktop::setgc(GraphicsContext* hold){
    localgc = hold;
}
