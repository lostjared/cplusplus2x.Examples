#include"mx_event.hpp"
#include<vector>
#include"mx_window.hpp"

namespace mx {

    bool EventHandler::pumpEvent(SDL_Event &e) {
        for (auto it = window_stack.rbegin(); it != window_stack.rend(); ++it) {
            Window *window = *it;
            if (window->event(app_, e)) {
                return true;
            }
        }
        return false;
    }

    void EventHandler::sendDrawMessage() {
        for (auto &window : window_stack) {
            window->draw(app_);
        }
    }

    void EventHandler::setFocus(int index) {
        if (index >= 0 && index < window_stack.size()) {
            cur_focus = index;
            Window *focused_window = window_stack[index];
            window_stack.erase(window_stack.begin() + index);
            window_stack.push_back(focused_window);
        }
    }

    Window *EventHandler::currentWindow() {
        if (!window_stack.empty()) {
            return window_stack.back();
        }
        return nullptr;
    }

    void EventHandler::addWindow(Window *window) {
        window_stack.push_back(window);
    }
}