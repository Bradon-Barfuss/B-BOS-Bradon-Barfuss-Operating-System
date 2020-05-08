#ifndef __OS__GUI__DESKTOP_H
#define __OS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace OS{
    namespace gui{
        
        
class Desktop : public CompositeWidget, public OS::drivers::MouseEventHandler{
protected:
    OS::common::uint32_t MouseX;
    OS::common::uint32_t MouseY;
    
public:
    Desktop(OS::common::int32_t w, OS::common::int32_t h, OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);
    ~Desktop();
    
    void Draw(OS::common::GraphicsContext* gc);
    void OnMouseDown(OS::common::uint8_t button);
    void OnMouseUp(OS::common::uint8_t button);
    void OnMouseMove(int x, int y);
    
    
};
    
        
        
        
    }
}








#endif
