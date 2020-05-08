#ifndef __OS__GUI__WINDOW_H
#define __OS__GUI__WINDOW_H

#include <common/types.h>
#include <gui/widget.h>
#include <drivers/mouse.h>
namespace OS{
    namespace gui{

        
        
class Window : public CompositeWidget{
protected:
    bool Dragging;
public:
    Window(Widget* parent, OS::common::int32_t x, OS::common::int32_t y, OS::common::int32_t w, OS::common::int32_t h, //the location/size of the widget
            OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);
    ~Window();
    void OnMouseDown(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    void OnMouseUp(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    void OnMouseMove(OS::common::int32_t oldx, OS::common::int32_t oldy, OS::common::int32_t newx, OS::common::int32_t newy);

};
        
        
        
        
        
        
        
        
        
        
        
        
    }
}








#endif
