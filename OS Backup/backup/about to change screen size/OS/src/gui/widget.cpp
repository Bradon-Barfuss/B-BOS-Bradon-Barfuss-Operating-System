#include <gui/widget.h>
#include <common/graphicscontext.h>
#include <drivers/vga.h>

using namespace OS;
using namespace OS::common;
using namespace OS::gui;
using namespace OS::drivers;
    

    
Widget::Widget(Widget* parent, OS::common::int32_t x, OS::common::int32_t y, OS::common::int32_t w, OS::common::int32_t h, OS::common::uint8_t r, OS::common::uint8_t g, OS::common::uint8_t b)
: KeyboardEventHandler()
{//parms for the rbg, or color
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->b = b;
    this->g = g;
    this->forcusable = true;//defalut forcusable is true
                
}
Widget::~Widget(){
    
}
    
void Widget::GetFocus(Widget* widget){
    if(parent != 0 ){//go to the next parent unless there isn't a next one
        parent->GetFocus(widget);
    }
}

void Widget::ModelToScreen(int32_t &x, int32_t &y){
    if(parent != 0){//if this isn't the main parent
        parent->ModelToScreen(x, y);//use recerisve and move to the next screen, to get the offset of x and y
    }
    //add to the absoult corrdenates of the x and y
    x += this->x;
    y += this->y;
}
void Widget::Draw(GraphicsContext* gc){
    int X = 0;//absoult x cordinate
    int Y = 0;//absolute y coordinate
    ModelToScreen(X,Y);//add the the absoult X and Y
    gc->FillRectangle(X,Y, w,h,  r,g,b);
    gc->Line(X,Y,w,h,2,r,g,b);
    gc->PrintString(X,Y,r,g,b,"THIS IS TEXT");

}

void Widget::OnMouseDown(int32_t x, int32_t y, uint8_t button){
    if(forcusable){
        GetFocus(this);
    }
}

void Widget::OnMouseUp(int32_t x, int32_t y, uint8_t button){}
void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy){}

//this->x/y is a realative coordiate, but it is fine because when we call this function, we are wanting a relative coordinate
bool Widget::ContainsCoordinate(int32_t x, int32_t y){

 return this->x <= x && x < this->x + this->w
        && this->y <= y && y < this->y + this->h;
}


CompositeWidget::CompositeWidget(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
: Widget(parent, x,y, w,h, r,g,b)
{
    forcussedChild = 0;
    numChildren = 0;
}

CompositeWidget::~CompositeWidget(){}

void CompositeWidget::GetFocus(Widget* widget){
    this->forcussedChild = widget;
    if(parent != 0){
        parent->GetFocus(this);//get the main parent
    }
}

void CompositeWidget::Draw(GraphicsContext* gc){
    Widget::Draw(gc);//draw its own background
    //the reason we start at numChildren is because that the numChildren is that first on that will be drawn, and all the ones after it will be drawn over it. so the orginal child will be displayed
    for(int i = 0; i < numChildren; ++i){//********put this in the wrong order, i was drawing the last one first, it should of been the other way around**********
        children[i]->Draw(gc);//this will draw all the children
    }
}

bool CompositeWidget::AddChild(Widget* child){
    if(numChildren >= 100){
        return false;
    }
    children[numChildren++] = child;
    
    return true;
}


void CompositeWidget::OnMouseDown(int32_t x, int32_t y, uint8_t button){//check if the mouse key is pressed in which child
    for(int i = 0; i < numChildren; ++i){//loop through all the widgets
        //the reason we start at 0, and not at numChildren, is because the 0 is the child that would first be displayed, so it will be the first to check. and just keep on moveing back until we find one.
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y)){//if the children/widget contains mouse coordiate, it is the correct widget
            children[i]->OnMouseDown(x - this->x, y - this->y, button);
            break;
        }
    }
}

//just like mouse down, but just using MouseUp
void CompositeWidget::OnMouseUp(int32_t x, int32_t y, uint8_t button){
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y)){//if the children/widget contains mouse coordiate, it is the correct widget
            children[i]->OnMouseUp(x - this->x, y - this->y, button);
            break;
        }
    }
}


void CompositeWidget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy){
    int firstchild = -1;
    //we will do the same thing twice, but one is for the old coordinate and one for the new coordinate
    for(int i = 0; i < numChildren; ++i){//loop through all the widgets
        if(children[i]->ContainsCoordinate(oldx - this->x, oldy - this->y)){
            children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);//this->x/y makes it into relative coordinates
            firstchild = i;//set what current child we are on
            break;
        }
    }

    //if we move into a new widget, run it Mouse Move twice
    for(int i = 0; i < numChildren; ++i){
        if(children[i]->ContainsCoordinate(newx - this->x, newy - this->y)){
            if(firstchild != i){//if we moved out of the widget, run mouse move again for the new widget
                children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);//this->x/y makes it into relative coordinates
                break;
            }
        }
    }
}

//has not been tested
Widget* CompositeWidget::findscreen(int32_t x, int32_t y){
    for(int i = 0; i < numChildren; ++i){//loop through all the widgets
        if(children[i]->ContainsCoordinate(x - this->x, y - this->y)){
            return children[i];
        }
    }
}

//we have these functinos in the keyboard driver, but we will override them, so we pass them into the composite child 
void CompositeWidget::OnKeyDown(char str){
    if(forcussedChild != 0){
        forcussedChild->OnKeyDown(str);
    }
}
void CompositeWidget::OnKeyUp(char str){
     if(forcussedChild != 0){
        forcussedChild->OnKeyUp(str);
    }
}
    
    
    
