#ifndef __TERM_H_
#define __TERM_H_

#include"dimension.hpp"

namespace mx {

  class Terminal :  public Window {
    public:
        Terminal(mxApp  &app);
        void draw(mxApp  &app);
        bool event(mxApp &app, SDL_Event  &e);
    };


}

#endif