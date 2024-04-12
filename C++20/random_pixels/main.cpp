
#include"SDL.h"
#include<iostream>
#include"argz.hpp"


void rand_pixels(SDL_Texture *tex, SDL_Surface *surface) {
    SDL_LockTexture(tex, 0, &surface->pixels, &surface->pitch);
    for(unsigned int i = 0; i < surface->w; ++i) {
        for(unsigned int z = 0; z < surface->h; ++z) {
            unsigned int *ptr = reinterpret_cast<unsigned int *>(surface->pixels) + (i+z*surface->w);
            *ptr = SDL_MapRGB(surface->format, rand()%255, rand()%255, rand()%255);
        }
    }
    SDL_UnlockTexture(tex);
}

class Texture {
public:
    Texture(SDL_Renderer *renderer, int width, int height) : ren{renderer} {
        tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        if(!tex) {
            std::cerr << "Fatal Error could not create texture....\n";
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
    }

    SDL_Texture *texture() const { return tex; }

    ~Texture() {
        if(tex != nullptr)
            SDL_DestroyTexture(tex);
    }

protected:
    SDL_Renderer *ren;
    SDL_Texture *tex = nullptr;
};

class ScopedSurface {
public:
    ScopedSurface(SDL_Surface *s) : surf{s} {}
    ~ScopedSurface() {
        if(surf != nullptr) {
            SDL_FreeSurface(surf);
            surf = nullptr;
        }
    }
    SDL_Surface *surface() { return surf; }
protected:
    SDL_Surface *surf = nullptr;
};

int main(int argc, char **argv) {
    int width=1080, height=1080;
    try {
        Argz<std::string> args(argc, argv);
        args.addOptionSingleValue('w', "width").addOptionSingleValue('h', "height");
        int value{};
        Argument<std::string> arg;

        while((value = args.proc(arg)) != -1) {
            switch(value) { 
                case 'w':
                width = atoi(arg.arg_value.c_str());
                break;
                case 'h':
                height = atoi(arg.arg_value.c_str());
                break;
            }
        }
    } catch(ArgException<std::string> &e) {
        std::cerr << e.text() << "\n";
    }
    
    if(width <= 0 || height <= 0) {
        std::cerr << "Invalid width/height";
        return EXIT_FAILURE;
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }
    SDL_Window *window = SDL_CreateWindow("Hello , World!", 0, 0, width, height, SDL_WINDOW_SHOWN); 
    SDL_Renderer *ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == 0) {
        std::cout << SDL_GetError() << "\n";
        SDL_Quit();
        return EXIT_FAILURE;
    }    
    Texture tex(ren, width, height);
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(NULL, width, height, 32, 0, 0x00FF0000, 0x0000FF00,0x000000FF,0xFF000000);
    ScopedSurface surface1{surface};
    bool active = true;
    SDL_Event e;
    while(active) {
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE) active = false;
                    break;
                case SDL_QUIT:
                    active = false;
                    break;
            }
        }       
        SDL_RenderClear(ren);
        rand_pixels(tex.texture(), surface1.surface());
        SDL_RenderCopy(ren, tex.texture(), 0, 0);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}