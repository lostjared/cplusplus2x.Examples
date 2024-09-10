#include"mx_event.hpp"
#include<vector>

namespace mx {

        void EventHandler::pumpEvent(SDL_Event &e) {

            if(window_stack.size()>0 && cur_focus >= 0 && cur_focus< static_cast<int>(window_stack.size())) {
                window_stack[cur_focus]->event(app_, e);
            }    
        
        }

        void  EventHandler::setFocus(int index){       
        }
        
        Window *EventHandler::currentWindow() {
            if(cur_focus >= 0 && cur_focus < static_cast<int>(window_stack.size())) {
                 return window_stack[cur_focus];
            }
            return nullptr;
        }
        void EventHandler::addWindow(Window *window) {
            window_stack.push_back(window);
        }
























}
