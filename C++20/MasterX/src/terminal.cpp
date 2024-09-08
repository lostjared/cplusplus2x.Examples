#include"terminal.hpp"

namespace mx {


    Terminal::Terminal(mxApp  &app) : Window(app) {

    }
    
    void Terminal::draw(mxApp  &app) {
        Window::draw(app);
    }
        
    bool Terminal::event(mxApp &app, SDL_Event  &e) {



        return false;
    }



}