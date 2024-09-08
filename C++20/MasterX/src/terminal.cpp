#include"terminal.hpp"

namespace mx {


    Terminal::Terminal(mxApp  &app) : Window(app) {

    }
    
    void Terminal::draw(mxApp  &app) {
        Window::draw(app);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 255);
        SDL_Rect rc;
        Window::getRect(rc);
        rc.x += 5;  
        rc.y += 28; 
        rc.w -= 10; 
        rc.h -= 33; 

        SDL_RenderFillRect(app.ren, &rc);
    }
        
    bool Terminal::event(mxApp &app, SDL_Event  &e) {

        if(Window::event(app, e))
            return true;

        return false;
    }



}