#ifndef __MX_CONTROLS__H_
#define __MX_CONTROLS__H_

#include "dimension.hpp"

namespace mx {

    class Label : public Control {
    public:
        Label(mxApp &app);
        virtual ~Label();
        virtual void draw(mxApp &app) override;
        virtual void setWindowPos(int xx, int yy) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void create(Window *parent, const std::string &text, SDL_Color col, int x, int y);
        void loadFont(const std::string &name, int size);
        void setText(const std::string  &text, SDL_Color color);
        void setGeometry(int xx, int yy);
        void linkMode(bool m);
    private:
        TTF_Font *font_;
        std::string name_;
        std::string text_;
        int x,y,size_;
        SDL_Color color_;
        int wx, wy;
        int w, h;
        bool mode = false;
        bool under_ = false;
    };

    

    class Button : public Control {
    public:
        Button(mxApp &app);
        virtual ~Button();
        virtual void draw(mxApp &app) override;
        virtual void setWindowPos(int xx, int yy) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void create(Window *parent, const std::string &text, int x, int y, int w, int h);
        void setText(const std::string &t);
        void setGeometry(int xx, int yy, int ww, int hh);
        void show(bool v);
     private:
        std::string text;
        int x, y, w, h;
        int wx, wy;
        bool visible;
        bool hover;
        bool pressed;
        SDL_Color fgColor;
        SDL_Color bgColor;
        SDL_Color hover_fg;
        SDL_Color hover_bg;
        SDL_Color pressed_fg;
        SDL_Color pressed_bg;
        SDL_Color textColor;
    };
}


#endif