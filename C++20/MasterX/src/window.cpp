#include"window.hpp"

namespace mx {
    
    bool mxApp::init(const std::string &name, int w, int h) {
        win = SDL_CreateWindow(name.c_str(), 
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                        w, h, SDL_WINDOW_SHOWN);
        if (win == nullptr) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (ren == nullptr) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(win);
            SDL_Quit();
            return false;
        }

        tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        if (tex == nullptr) {
            std::cerr << "SDL_CreateTexture Error: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
            SDL_Quit();
            return false;
        }
        init_ = true;
        width = w;
        height = h;

        font = TTF_OpenFont(getPath("fonts/arial.ttf").c_str(), 14);
        if(!font) {
            std::cerr << "MasterX System: font: " << getPath("fonts/arial.ttf") << " Could not be loaded.\n";
            exit(EXIT_FAILURE);
        }

        std::cout << "MasterX: Initalized System Objects\n";
        return true;
    }

    void mxApp::release() {
        if(init_ == true) {
            std::cout << "MasterX: Releasing System Objects\n";
            SDL_DestroyTexture(tex);
            SDL_DestroyRenderer(ren);
            SDL_DestroyWindow(win);
        }
    }

    mxApp::~mxApp() {
        release();
    }
}