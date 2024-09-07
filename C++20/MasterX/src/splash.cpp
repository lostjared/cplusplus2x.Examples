#include"splash.hpp"

namespace mx {

    Splash::Splash(mxApp &app) {
        SDL_Surface *temp = SDL_LoadBMP(getPath("images/startup.bmp").c_str());
        if(!temp) {
            std::cerr << "Error loading bitmap: " << getPath("images/startup.bmp") << "\n";
            exit(EXIT_FAILURE);
        }
        bg = SDL_CreateTextureFromSurface(app.ren, temp);
        if(!bg) {
            std::cerr << "Error creating texture...\n";
            exit(EXIT_FAILURE);
        }
        font = TTF_OpenFont(getPath("fonts/arial.ttf").c_str(), 120);
        if(!font) {
            std::cerr << "Error could not load font: " << getPath("fonts/arial.ttf") << "\n";
            exit(EXIT_FAILURE);
        }
    }

    Splash::~Splash() {
        std::cout << "Releasing Splash..\n";
        SDL_DestroyTexture(bg);
        //TTF_CloseFont(font);
    }

   
    void Splash::draw(mxApp &app) {
        SDL_SetRenderTarget(app.ren, app.tex);
        Uint32 elapsed = SDL_GetTicks();
        float progress = (elapsed % 5000) / 5000.0f;
        SDL_Color textColor = {0xBD, 0, 0, 255};
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, "MasterX", textColor);
        if (!textSurface) {
            exit(EXIT_FAILURE);
        }
        int tw = textSurface->w;
        int th = textSurface->h;
        SDL_Texture *tex1 = SDL_CreateTextureFromSurface(app.ren, textSurface);
        if(!tex1) {
            std::cerr << "Error creating texture..\n";
            exit(EXIT_FAILURE);
        }
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 0);
        SDL_RenderClear(app.ren);
        SDL_RenderCopy(app.ren, bg, nullptr, nullptr);
        int padding = 50;
        int window_width = app.width;
        int available_width = window_width - 2 * padding;
        int bar_width = static_cast<int>(available_width * progress);
        int bar_height = 50;
        int bar_x = padding;
        int bar_y = (app.height - bar_height) / 2;
        SDL_SetRenderDrawColor(app.ren, 200, 200, 200, 255);
        SDL_Rect background = { bar_x, bar_y, available_width, bar_height };
        SDL_RenderFillRect(app.ren, &background);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 255, 255);
        SDL_Rect progressBar = { bar_x, bar_y, bar_width, bar_height };
        SDL_RenderFillRect(app.ren, &progressBar);
        int text_x = (app.width - tw) / 2;
        int text_y = bar_y - th - 20;  
        SDL_Rect textRect = {text_x, text_y, tw, th};
        SDL_RenderCopy(app.ren, tex1, nullptr, &textRect);
        SDL_SetRenderTarget(app.ren, nullptr);
        SDL_DestroyTexture(tex1);
        if (progress >= 0.95f) {
            setScreen(ID_DIM);
            std::cout << "MasterX: Screen Dimension\n";
        }
        SDL_SetRenderTarget(app.ren, nullptr);
    }

    bool Splash::event(mxApp &app, SDL_Event &e) { 
        if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
            setScreen(ID_DIM);
            return true;
        } 
        return false;
    }
}