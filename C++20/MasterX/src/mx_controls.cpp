#include "mx_controls.hpp"

namespace mx {

       Label::Label(mxApp &app)
        : font_(nullptr), name_(""), text_(""), x(0), y(0), size_(0), wx(0), wy(0), w(0), h(0) {
    }

    Label::~Label() {
    }

    void Label::setWindowPos(int xx, int yy) {
        wx = xx;
        wy = yy;
    }

    void Label::draw(mxApp &app) {
        if(font_ != nullptr && text_.length() > 0) {
            if(mode == false || under_ == false) {
                TTF_SetFontStyle(font_, TTF_STYLE_NORMAL);
            } else if(under_ == true) {
                cursor_shown = true;
                TTF_SetFontStyle(font_, TTF_STYLE_UNDERLINE);
            }
            SDL_Surface *surf = TTF_RenderText_Solid(font_, text_.c_str(), color_);
            if(surf == nullptr) {
                std::cerr << "MasterX System: Error creating surface.\n";
                exit(EXIT_FAILURE);
            }
            int sw = surf->w;
            int sh = surf->h;
            w = sw;
            h = sh;
            SDL_Texture *t = SDL_CreateTextureFromSurface(app.ren, surf);
            if(t == nullptr) {
                std::cerr << "MasterX System: Error creating texture.\n";
                exit(EXIT_FAILURE);
            }
            SDL_FreeSurface(surf);
            SDL_SetRenderTarget(app.ren, app.tex);
            SDL_Rect point = {x+wx,y+wy+25,sw,sh};
            SDL_RenderCopy(app.ren, t, nullptr, &point);
        }
    }

    void Label::linkMode(bool m) {
        mode = m;
    }

    void Label::create(const std::string &text, SDL_Color col, int xx, int yy) {
        setText(text, col);
        setGeometry(xx, yy);
    }

    bool Label::event(mxApp &app, SDL_Event &e) {

        if(e.type == SDL_MOUSEMOTION) {
            int mx = e.motion.x;
            int my = e.motion.y;
            SDL_Rect rc = {x+wx, y+wy+25, w, h };
            SDL_Point cp = { mx, my };
            if(SDL_PointInRect(&cp, &rc)) {
                cursor_shown = true;
                under_ = true;
                return true;
            } else {
                under_ = false;
            }
        }


        return false;
    }

    void Label::loadFont(const std::string &name, int sizex) {
        name_ = name;
        size_ = sizex;
        font_ = TTF_OpenFont(getPath(name).c_str(), size_);
        if(font_ == nullptr) {
            std::cerr << "Error opening font: " << getPath(name) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void Label::setText(const std::string &text, SDL_Color col) {
        text_ = text;
        color_ = col;
    }

    void Label::setGeometry(int xx, int yy) {
        x = xx;
        y = yy;
    }

    Button::Button(mxApp &app) 
        : x(0), y(0), w(0), h(0), visible(true), hover(false), pressed(false) {
        fgColor = { 200, 200, 200, 255 };  
        bgColor = { 240, 240, 240, 255 };  
        hover_fg = { 180, 180, 180, 255 };  
        hover_bg = { 220, 220, 220, 255 };  
        pressed_fg = { 120, 120, 120, 255 };  
        pressed_bg = { 200, 200, 200, 255 };  
        textColor = { 0, 0, 0, 255 };        
    }

    Button::~Button() {}

    void Button::draw(mxApp &app) {
        if (!visible) return;

        SDL_Renderer* renderer = app.ren;
        SDL_Rect rect = { x, y, w, h };
        
        SDL_Color topLeftColor = fgColor;
        SDL_Color bottomRightColor = { 100, 100, 100, 255 };  

        if (pressed) {
            SDL_SetRenderDrawColor(renderer, pressed_bg.r, pressed_bg.g, pressed_bg.b, pressed_bg.a);
            topLeftColor = pressed_fg;
            bottomRightColor = { 150, 150, 150, 255 }; 
        } else if (hover) {
            SDL_SetRenderDrawColor(renderer, hover_bg.r, hover_bg.g, hover_bg.b, hover_bg.a);
        } else {
            SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        }
        
        SDL_RenderFillRect(renderer, &rect);
        
        
        SDL_SetRenderDrawColor(renderer, topLeftColor.r, topLeftColor.g, topLeftColor.b, topLeftColor.a);
        SDL_RenderDrawLine(renderer, x, y, x + w - 1, y);  
        SDL_RenderDrawLine(renderer, x, y, x, y + h - 1);  
        
        SDL_SetRenderDrawColor(renderer, bottomRightColor.r, bottomRightColor.g, bottomRightColor.b, bottomRightColor.a);
        SDL_RenderDrawLine(renderer, x + w - 1, y, x + w - 1, y + h - 1);  
        SDL_RenderDrawLine(renderer, x, y + h - 1, x + w - 1, y + h - 1);  

        if (app.font) {
            SDL_Surface* surface = TTF_RenderText_Blended(app.font, text.c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;
            SDL_Rect dstrect = { x + (w - text_width) / 2, y + (h - text_height) / 2, text_width, text_height };

            SDL_RenderCopy(renderer, texture, nullptr, &dstrect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }

    void Button::setWindowPos(int xx, int yy) {
        x = xx;
        y = yy;
    }

    bool Button::event(mxApp &app, SDL_Event &e) {
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;
            hover = (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) {
                pressed = true;
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (pressed && mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h) {
                pressed = false;
                return true;  
            }
            pressed = false;
        }
        
        return false;
    }

    void Button::create(const std::string &text, int x, int y, int w, int h) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void Button::setText(const std::string &t) {
        text = t;
    }

    void Button::setGeometry(int xx, int yy, int ww, int hh) {
        x = xx;
        y = yy;
        w = ww;
        h = hh;
    }
    void Button::show(bool v) {
        visible = v;
    }
}