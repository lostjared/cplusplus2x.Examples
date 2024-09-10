 #ifndef __MX_EVENT_H_X
#define __MX_EVENT_H_X

#include"window.hpp"
#include"mx_window.hpp"

namespace mx {


    class EventHandler {
    public:
        EventHandler(mxApp &app) : app_{app} {}
        bool pumpEvent(SDL_Event &e);
        void sendDrawMessage();
        void setFocus(int index);
        Window *currentWindow();
        void addWindow(Window *window);
        std::vector<Window *> window_stack;
    private:
        mxApp &app_;
        int cur_focus = 0;
    };
}

#endif