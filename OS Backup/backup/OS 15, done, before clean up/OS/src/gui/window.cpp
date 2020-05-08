#include <gui/window.h>
#include <common/graphicscontext.h>
#include <drivers/vga.h>

using namespace OS;
using namespace OS::common;
using namespace OS::gui;

Window::Window(Widget* parent, OS::common::int32_t x, OS::common::int32_t y, OS::common::int32_t w, OS::common::int32_t h, //the location/size of the widget
            OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b) : CompositeWidget(parent, x,y,w,h,r,g,b){
    Dragging = false;
}

Window::~Window(){
    
}

void Window::OnMouseDown(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button){
    Dragging = (button == 1);
    CompositeWidget::OnMouseDown(x,y,button);
}

void Window::OnMouseUp(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button){
    Dragging = false;
    CompositeWidget::OnMouseUp(x,y,button);

}

void Window::OnMouseMove(OS::common::int32_t oldx, OS::common::int32_t oldy, OS::common::int32_t newx, OS::common::int32_t newy){
    if(Dragging){
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
        CompositeWidget::OnMouseMove(oldx,oldy,newx,newy);

}
