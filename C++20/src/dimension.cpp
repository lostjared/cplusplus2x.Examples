#include"dimension.hpp"


namespace mx {


    SystemBar::SystemBar(mxApp &app) {
        font = TTF_OpenFont(getPath("fonts/arial.ttf").c_str(), 14);
        if(!font) {
            std::cerr << "Error opening font:" << getPath("fonts/arial.tf") << "\n";
            exit(EXIT_FAILURE);
        }
        animationComplete = false;
        yPos = 0;
    }

    SystemBar::~SystemBar() {
        //TTF_CloseFont(font);
    }

    void SystemBar::draw(mxApp &app) {
        int barHeight = 50;
        int windowWidth = app.width;
        int windowHeight = app.height;
        int targetYPos = windowHeight - barHeight;
        int startButtonSize = 100;
        if (!animationComplete) {
            yPos += 5;  
            if (yPos >= targetYPos) {
                yPos = targetYPos;
                animationComplete = true;
            }
        }
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_SetRenderDrawColor(app.ren, 192, 192, 192, 255);  
        SDL_Rect barRect = {0, yPos, windowWidth, barHeight}; 
        SDL_RenderFillRect(app.ren, &barRect);
        SDL_Color buttonColor = {169, 169, 169, 255};  
        SDL_Color textColor = {255, 255, 255};  

        if (isHovering) {
            buttonColor = {0, 0, 139, 255};  
            textColor = {255, 255, 255};  
        }

        SDL_SetRenderDrawColor(app.ren, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_Rect startButton = {windowWidth - startButtonSize, yPos, startButtonSize, barHeight};
        SDL_RenderFillRect(app.ren, &startButton);
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Launch", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app.ren, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;
        SDL_FreeSurface(textSurface);

        SDL_Rect textRect = {
            windowWidth - startButtonSize + (startButtonSize - textWidth) / 2,
            yPos + (barHeight - textHeight) / 2,
            textWidth,
            textHeight
        };

        SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_SetRenderTarget(app.ren, nullptr);
    }

    void SystemBar::event(mxApp &app, SDL_Event &e) {
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.x;
            int mouseY = e.motion.y;

            int barHeight = 50;
            int startButtonSize = 100;
            int windowWidth = app.width;
            int targetYPos = app.height - barHeight;

            SDL_Rect startButtonRect = {windowWidth - startButtonSize, targetYPos, startButtonSize, barHeight};

            if (mouseX >= startButtonRect.x && mouseX <= (startButtonRect.x + startButtonRect.w) &&
                mouseY >= startButtonRect.y && mouseY <= (startButtonRect.y + startButtonRect.h)) {
                isHovering = true;
            } else {
                isHovering = false;
            } 
        }  else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_LEAVE) {
         
          isHovering = false;
       }
    }

    Dimension::Dimension(mxApp &app) {
        wallpaper = loadTexture(app, "images/wallpaper.bmp");
        objects.push_back(std::make_unique<SystemBar>(app));
    }
    
    Dimension::~Dimension() {
        SDL_DestroyTexture(wallpaper);
        std::cout << "MasterX: Releasing Dimension\n";
    }

    void Dimension::draw(mxApp &app) {
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
    
        for(auto &i : objects) {
            i->draw(app);
        }
        SDL_SetRenderTarget(app.ren, nullptr);
    }
    
    void Dimension::event(mxApp &app, SDL_Event &e) {
        for(auto  &i : objects) {
            i->event(app, e);
        }
    }

}