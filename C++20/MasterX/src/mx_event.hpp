#ifndef __MX_EVENT_H_X
#define __MX_EVENT_H_X

#include"window.hpp"
#include"mx_window.hpp"

namespace mx {


    class EventHandler {
    public:
        EventHandler(mxApp &app) : app_{app} {}
        void pumpEvent(SDL_Event &e);
        void setFocus(int index);
        Window *currentWindow();
        void addWindow(Window *window);
    private:
        mxApp &app_;
        std::vector<Window *> window_stack;
        int cur_focus = 0;














































































    };




}


#endif