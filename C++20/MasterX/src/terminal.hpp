#ifndef __TERM_H_
#define __TERM_H_

#include"dimension.hpp"

namespace mx {

  class Terminal :  public Window {
    public:
        Terminal(mxApp  &app);
        void draw(mxApp  &app);
        bool event(mxApp &app, SDL_Event  &e);
      private:
        std::string inputText;
        std::vector<std::string> outputLines;
        void renderText(mxApp &app, const std::string &text, int x, int y);
        void renderTextWrapped(mxApp &app, const std::string &text, int x, int &y, int maxWidth);
        void processCommand(mxApp &app, const std::string &cmd);
        void handleScrolling(int);
        std::vector<std::string> splitText(const std::string &text);
        int scrollOffset = 0; 
        int maxVisibleLines = 10; 
    };


}

#endif