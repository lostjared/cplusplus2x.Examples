#include "dimension.hpp"
#include "window.hpp"
#include "SDL_rect.h"

namespace mx {

    SystemBar::SystemBar(mxApp &app) {
        font = TTF_OpenFont(getPath("fonts/arial.ttf").c_str(), 14);
        if (!font) {
            std::cerr << "Error opening font: " << getPath("fonts/arial.ttf") << "\n";
            exit(EXIT_FAILURE);
        }
        objects.push_back(std::make_unique<MenuBar>(app));
        menu = dynamic_cast<MenuBar *>(objects[0].get());
        menu->menuOpen = false;
        animationComplete = false;
        yPos = 0;
    }

    SystemBar::~SystemBar() {
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

        SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);  // White bevel color
        SDL_RenderDrawLine(app.ren, startButton.x, startButton.y, startButton.x + startButton.w - 1, startButton.y);  
        SDL_RenderDrawLine(app.ren, startButton.x, startButton.y, startButton.x, startButton.y + startButton.h - 1);  
        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);  
        SDL_RenderDrawLine(app.ren, startButton.x, startButton.y + startButton.h - 1, startButton.x + startButton.w - 1, startButton.y + startButton.h - 1);  
        SDL_RenderDrawLine(app.ren, startButton.x + startButton.w - 1, startButton.y, startButton.x + startButton.w - 1, startButton.y + startButton.h - 1);  


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

        for (auto &i : objects) {
            i->draw(app);
        }
    }

    bool SystemBar::event(mxApp &app, SDL_Event &e) {
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
        } else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_LEAVE) {
            isHovering = false;
            return true;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                int barHeight = 50;
                int startButtonSize = 100;
                int windowWidth = app.width;
                int targetYPos = app.height - barHeight;

                SDL_Rect startButtonRect = {windowWidth - startButtonSize, targetYPos, startButtonSize, barHeight};

                if (mouseX >= startButtonRect.x && mouseX <= (startButtonRect.x + startButtonRect.w) &&
                    mouseY >= startButtonRect.y && mouseY <= (startButtonRect.y + startButtonRect.h)) {
                    menu->menuOpen = !menu->menuOpen;
                    menu->animating = true;
                if (menu->menuOpen) {
                    menu->currentY = targetYPos - barHeight;  
                    menu->targetY = targetYPos - (app.height / 2);  
                } else {
                    menu->currentY = targetYPos - (app.height / 2);  // Start at expanded hei
                    menu->targetY = targetYPos - barHeight;  // End just above the button
                }
                return true;
                }
            }

        for (auto &i : objects) {
            if(i->event(app, e))
                return true;
        }
        return false;
    }

    MenuBar::MenuBar(mxApp &app) {
        menuOpen = false;
        currentY = app.height;  
        targetY = app.height / 2;  
        font = TTF_OpenFont(getPath("fonts/arial.ttf").c_str(), 14);
        if (!font) {
            std::cerr << "Error opening font: " << getPath("fonts/arial.ttf") << "\n";
            exit(EXIT_FAILURE);
        }
    }

    MenuBar::~MenuBar() {
       // TTF_CloseFont(font);
    }

    void MenuBar::draw(mxApp &app) {
        if (!menuOpen && !animating) {
            return;
        }

        if (animating) {
            if (menuOpen) {
                currentY -= 30;
                if (currentY <= targetY) {
                    currentY = targetY;
                    animating = false;
                }
            } else {
                currentY += 30;
                if (currentY >= targetY) {
                    currentY = targetY;
                    animating = false;
                }
            }
        }

        int menuWidth = app.width / 6;
        int menuHeight = app.height / 2;
        int menuX = app.width - menuWidth - 20;

        SDL_SetRenderTarget(app.ren, app.tex);

        SDL_Color hoverColor = {0, 0, 128, 255};
        SDL_SetRenderDrawColor(app.ren, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
        SDL_Rect rightSideRect = {menuX + menuWidth, currentY, 20, menuHeight};
        SDL_RenderFillRect(app.ren, &rightSideRect);

        SDL_SetRenderDrawColor(app.ren, 192, 192, 192, 255);
        SDL_Rect menuRect = {menuX, currentY, menuWidth, menuHeight};
        SDL_RenderFillRect(app.ren, &menuRect);

        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);
        SDL_RenderDrawRect(app.ren, &menuRect);

        SDL_Color black = {0, 0, 0, 255};
        SDL_Color white = {255, 255, 255, 255};

        const char* items[] = {"Programs", "Documents", "Settings", "Find", "Help", "Run"};
        int numItems = sizeof(items) / sizeof(items[0]);
        int itemHeight = 30;
        SDL_Rect itemRect;

        for (int i = 0; i < numItems; i++) {
            itemRect = {menuX + 10, currentY + 10 + i * (itemHeight + 5), menuWidth - 20, itemHeight};

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Point cursor_pos = {mouseX, mouseY};
            bool isHovering = SDL_PointInRect(&cursor_pos, &itemRect);

            if (isHovering) {
                SDL_SetRenderDrawColor(app.ren, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
                SDL_RenderFillRect(app.ren, &itemRect);
            } else {
                SDL_SetRenderDrawColor(app.ren, 192, 192, 192, 255);
                SDL_RenderFillRect(app.ren, &itemRect);
            }

            SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);
            SDL_RenderDrawRect(app.ren, &itemRect);

            SDL_Color textColor = isHovering ? white : black;
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, items[i], textColor);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app.ren, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = {itemRect.x + (itemRect.w - textWidth) / 2, itemRect.y + (itemRect.h - textHeight) / 2, textWidth, textHeight};
            SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }

        SDL_SetRenderTarget(app.ren, nullptr);
 
    }   
    
    bool MenuBar::event(mxApp &app, SDL_Event &e) {
        if (!menuOpen && !animating) {
            return false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            int menuX = app.width / 4;
            int menuY = app.height / 4;

            SDL_Rect menuItem1 = {menuX + 20, menuY + 20, 200, 40};
            SDL_Rect menuItem2 = {menuX + 20, menuY + 80, 200, 40};
            SDL_Rect menuItem3 = {menuX + 20, menuY + 140, 200, 40};

            SDL_Point cursor_pos = {mouseX, mouseY};

            if (SDL_PointInRect(&cursor_pos, &menuItem1)) {
                std::cout << "Menu Item 1 clicked\n";
                return true;
            } else if (SDL_PointInRect(&cursor_pos, &menuItem2)) {
                std::cout << "Menu Item 2 clicked\n";
                return true;
            } else if (SDL_PointInRect(&cursor_pos, &menuItem3)) {
                std::cout << "Menu Item 3 clicked\n";
                return true;
            }
        }
        return false;
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

        for (auto &i : objects) {
            i->draw(app);
        }
        SDL_SetRenderTarget(app.ren, nullptr);
    }

    bool Dimension::event(mxApp &app, SDL_Event &e) {
        for (auto &i : objects) {
            if(i->event(app, e))
                return true;
        }
        return false;
    }

}