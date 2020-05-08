#ifndef __OS__GUI__WIDGET_H
#define __OS__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

namespace OS{
    namespace gui{
        
        
        
class Widget : public OS::drivers::KeyboardEventHandler{
protected:
    Widget* parent;
    OS::common::int32_t x;
    OS::common::int32_t y;
    OS::common::int32_t w;
    OS::common::int32_t h;
    
    OS::common::uint8_t r;
    OS::common::uint8_t g;
    OS::common::uint8_t b;
    bool forcusable;
public:
    //Widget();
    Widget(Widget* parent, 
           OS::common::int32_t x, OS::common::int32_t y, OS::common::int32_t w, OS::common::int32_t h,
           OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);//parms for the rbg, or color
    
    ~Widget();

    virtual void GetFocus(Widget* widget);
    virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);

    virtual void ModelToScreen(OS::common::int32_t &x, OS::common::int32_t &y);
    virtual void Draw(OS::common::GraphicsContext* gc);
    
    
    virtual void OnMouseDown(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    virtual void OnMouseUp(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    virtual void OnMouseMove(OS::common::int32_t oldx, OS::common::int32_t oldy, OS::common::int32_t newx, OS::common::int32_t newy);
};



class CompositeWidget : public Widget{
private:
    Widget* children[100];
    int numChildren;
    Widget* forcussedChild;
public:
    CompositeWidget(Widget* parent, OS::common::int32_t x, OS::common::int32_t y, OS::common::int32_t w, OS::common::int32_t h, //the location/size of the widget
            OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b);

    ~CompositeWidget();

    virtual void GetFocus(Widget* widget);
    virtual bool AddChild(Widget* child);

    virtual void Draw(OS::common::GraphicsContext* gc);
    virtual void OnMouseDown(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    virtual void OnMouseUp(OS::common::int32_t x, OS::common::int32_t y, OS::common::uint8_t button);
    virtual void OnMouseMove(OS::common::int32_t oldx, OS::common::int32_t oldy, OS::common::int32_t newx, OS::common::int32_t newy);
    
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};

        
        
    }
}








#endif
