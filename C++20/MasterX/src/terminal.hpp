#ifndef __TERM_H_
#define __TERM_H_

#include"dimension.hpp"

namespace mx {

  class Terminal :  public Window {
    public:
        Terminal(mxApp  &app);
        void draw(mxApp  &app);
        bool event(mxApp &app, SDL_Event  &e);
        void print(const std::string &s);
        void scroll();
        void stateChanged(bool min, bool max, bool closed);
        void setWallpaper(SDL_Texture *tex);
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
        bool showCursor = true;
        Uint32 cursorTimer = 0, cursorBlinkInterval = 100;  
        SDL_Color text_color;
        bool isMaximized = false;
        TTF_Font *font;
        SDL_Texture *wallpaper;
        bool isScrolling = false;
        int scrollBarWidth = 10;
        int scrollBarHeight = 0;
        int scrollBarPosY = 0;
        int scrollBarDragOffset = 0;
    };


}

#endif