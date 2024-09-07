#ifndef _DIMENSION_H
#define _DIMENSION_H

#include "window.hpp"
#include<memory>
#include<vector>

namespace mx {

    class SystemBar : public Screen {
    public:
        SystemBar(mxApp &app);
        virtual ~SystemBar();
        virtual void draw(mxApp &app) override;
        virtual void event(mxApp &app, SDL_Event &e) override;
    private:
        int yPos;
        bool animationComplete;
        TTF_Font  *font;
        SDL_Color start_bg;
        bool isHovering = false;
    };

    class Dimension : public Screen {
    public:
        Dimension(mxApp &app);
        virtual ~Dimension();

        virtual void draw(mxApp &app) override;
        virtual void event(mxApp &app, SDL_Event &e) override;
    private:
        SDL_Texture *wallpaper;
        std::vector<std::unique_ptr<Screen>> objects;
    };


}

#endif