#ifndef __WINDOW_H__
#define __WINDOW_H__

#include"SDL.h"
#include"SDL_ttf.h"

#include<string>
#include<iostream>

extern std::string getPath(const std::string &name);
enum { ID_LOAD=0, ID_DIM };

namespace mx {

    struct mxApp {
        SDL_Window *win;
        SDL_Renderer *ren;
        SDL_Texture *tex;
        bool init_ = false;
        mxApp() = default;
        ~mxApp();
        bool init(const std::string &name, int w, int h);
        void release();
        int width = 0, height = 0;
        TTF_Font *font;
        SDL_Texture* convertToStreamingTexture(SDL_Texture* originalTexture);
    };

    class Screen {
    public:
        Screen() = default;
        virtual ~Screen() = default;
        virtual void draw(mx::mxApp &app) = 0;
        virtual bool event(mxApp &app, SDL_Event &e) = 0;
    };
}

extern void setScreen(int screen);
extern SDL_Texture *loadTexture(mx::mxApp &app, const std::string &name);

#endif