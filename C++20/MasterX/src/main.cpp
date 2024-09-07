#include"SDL.h"
#include<iostream>
#include"window.hpp"
#include"splash.hpp"
#include"dimension.hpp"
#include<vector>
#include<memory>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

bool active = true;
std::vector<std::unique_ptr<mx::Screen>> *screens;
int cur_screen = 0;
mx::mxApp *p_app = nullptr;

void setScreen(int scr) {
    if(scr >= 0 && scr < static_cast<int>(screens->size()))
        cur_screen = scr;
    else {
        std::cerr << "Error screen out of bounds\n";
    }
}

void draw(mx::mxApp &app) {
    SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 255);
    SDL_RenderClear(app.ren);

    SDL_SetRenderTarget(app.ren, app.tex);
    SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 255);
    SDL_RenderClear(app.ren);

    (*screens)[cur_screen]->draw(app);

    SDL_RenderCopy(app.ren, app.tex, nullptr, nullptr);
    SDL_RenderPresent(app.ren);
}

 void eventProc() {
    static SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            active = false;
        }
        (*screens)[cur_screen]->event(*p_app, e);
    }
    draw(*p_app);
}

void init(mx::mxApp &app) {
    screens->push_back(std::make_unique<mx::Splash>(app));
    screens->push_back(std::make_unique<mx::Dimension>(app));
}

#ifdef FOR_WASM
std::string cur_path = "/assets";
#else
std::string cur_path = "assets";
#endif

std::string getPath(const std::string &name) {
    return cur_path + "/" + name;
}

SDL_Texture *loadTexture(mx::mxApp &app, const std::string &name) {
    SDL_Surface *surf = SDL_LoadBMP(getPath(name).c_str());
    if(!surf) {
        std::cerr << "Error loading surface; " << getPath(name) << "\n";
        exit(EXIT_FAILURE);
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(app.ren, surf);
    if(!tex) {
        std::cerr << "Error creating texture from surface: " << name << "\n";
        exit(EXIT_FAILURE);
    }
    return tex;
}

int main(int argc, char **argv) {

    mx::mxApp app;  
    p_app = &app;

    if(argc == 2) {
        cur_path = argv[1];
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    
    if(TTF_Init() < 0) {
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << "\n";
        return 1;
    }
  
    std::vector<std::unique_ptr<mx::Screen>> screen_obj;
    screens = &screen_obj;
    if(!app.init("MasterX", 1280, 720)) {
        exit(EXIT_FAILURE);
        return 1;
    }
    init(app);
#ifndef FOR_WASM
    while(active == true) {
        eventProc();
    }
#else
    emscripten_set_main_loop(eventProc, 0, 1);
#endif

    TTF_Quit();
    SDL_Quit();
    return 0;

 
}