#include "dimension.hpp"
#include "window.hpp"
#include "SDL_rect.h"

namespace mx {

    SystemBar::SystemBar(mxApp &app) {
        dimensions = nullptr;
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

    void SystemBar::setDimensions(std::vector<std::unique_ptr<Screen>> *dim) {
        dimensions = dim;
    }

    void SystemBar::drawDimensions(mxApp &app) {
        if (dimensions != nullptr && animationComplete == true) {
            for (size_t i = 0; i < dimensions->size(); ++i) {
                DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
                if (dim != nullptr) {
                    const std::string &name = dim->name;
                    SDL_Color white = {255, 255, 255, 255};
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, name.c_str(), white);
                    if (textSurface == nullptr) {
                        continue;
                    }

                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app.ren, textSurface);
                    int text_width = textSurface->w;
                    int text_height = textSurface->h;
                    SDL_FreeSurface(textSurface);
                    if (textTexture == nullptr) {
                        continue;
                    }

                    int button_width = 150;  
                    int button_height = text_height + 20;  

                    
                    int bar_height = 50;  
                    int button_y = (bar_height - button_height) / 2 + app.height - bar_height;

                    int button_x = 10 + i * (button_width + 10);
                    if (static_cast<int>(i) == cur_dim) {
                        SDL_SetRenderDrawColor(app.ren, 0, 0, 255, 255);  
                    } else {
                        SDL_SetRenderDrawColor(app.ren,150, 150, 150, 255);  
                    }
                    
                    SDL_Rect buttonRect = { button_x, button_y, button_width, button_height };
                    SDL_RenderFillRect(app.ren, &buttonRect);

                    SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
                    SDL_RenderDrawRect(app.ren, &buttonRect);

                    SDL_Rect textRect = { button_x + 10, 
                                        button_y + (button_height - text_height) / 2, 
                                        text_width, text_height };

                    SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
            }
        }
    }

    void SystemBar::setCurrentDimension(int dim) {
        cur_dim = dim;
    }
    int  SystemBar::getCurrentDimesnion() const {
        return cur_dim;
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
        SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);  
        SDL_RenderDrawLine(app.ren, barRect.x, barRect.y, barRect.x + barRect.w - 1, barRect.y);  
        SDL_RenderDrawLine(app.ren, barRect.x, barRect.y, barRect.x, barRect.y + barRect.h - 1);  
        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);  
        SDL_RenderDrawLine(app.ren, barRect.x, barRect.y + barRect.h - 1, barRect.x + barRect.w - 1, barRect.y + barRect.h - 1); 
        SDL_RenderDrawLine(app.ren, barRect.x + barRect.w - 1, barRect.y, barRect.x + barRect.w - 1, barRect.y + barRect.h - 1); 
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
        drawDimensions(app);
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

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
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
                    menu->currentY = targetYPos - (app.height / 2);
                    menu->targetY = targetYPos - barHeight;
                }
                return true;
            }

            if (dimensions != nullptr) {
                int button_width = 150;
                int button_height = 30; 
                int button_y = app.height - barHeight;

                for (size_t i = 0; i < dimensions->size(); ++i) {
                    int button_x = 10 + i * (button_width + 10);

                    SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};

                    
                    if (mouseX >= buttonRect.x && mouseX <= (buttonRect.x + buttonRect.w) &&
                        mouseY >= buttonRect.y && mouseY <= (buttonRect.y + buttonRect.h)) {
                        setCurrentDimension(i);  
                        return true;
                    }
                }
            }
        }

        for (auto &i : objects) {
            if (i->event(app, e))
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

    Window::Window(mxApp &app) : x{0}, y{0}, w{320}, h{240}, title{"windwow"}, shown{false} {
   
    }

    Window::~Window() {
        std::cout << "MasterX: Releasing Window: " << title << "\n";
    }


    void Window::create(const std::string &n, const int xx, const int yy, const int ww, const int hh) {
        title = n;
        x = xx;
        y = yy;
        w = ww;
        h = hh;
    }

    void Window::draw(mxApp &app) {
    }

    bool Window::event(mxApp &app, SDL_Event &e) {
        return false;
    }

    DimensionContainer::DimensionContainer(mxApp &app) : wallpaper{nullptr} {
    
    }


    DimensionContainer::~DimensionContainer() {
        std::cout << "MasterX: Releasing DImension: " << name << "\n";
        if(wallpaper) {
            SDL_DestroyTexture(wallpaper);
        }
    }

    void DimensionContainer::init(const std::string &name_, SDL_Texture *wallpaperx) {
        name = name_;
        wallpaper = wallpaperx;
    }

    void DimensionContainer::draw(mxApp &app) {

        if(wallpaper != nullptr) {
            SDL_SetRenderTarget(app.ren, app.tex);
            SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
        }

        for(auto &i : objects) {
            i->draw(app);
        }
    }

    bool DimensionContainer::event(mxApp &app, SDL_Event &e) {
        for(auto &i : objects) {
            if(i->event(app, e) == true)
                return true;
        }
        return false;
    }

    Dimension::Dimension(mxApp &app) {
        objects.push_back(std::make_unique<SystemBar>(app));
        system_bar = dynamic_cast<SystemBar *>(objects[0].get());
        dimensions.push_back(std::make_unique<DimensionContainer>(app));

        welcome = dynamic_cast<DimensionContainer *>(dimensions[0].get());
        welcome->init("Welcome", loadTexture(app, "images/wallpaper.bmp"));
        
        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        about = dynamic_cast<DimensionContainer *>(dimensions[1].get());
        about->init("About", loadTexture(app, "images/about.bmp"));
        
        system_bar->setDimensions(&dimensions);
        setCurrentDimension(0);

    }

    void Dimension::setCurrentDimension(int dim) {
        system_bar->setCurrentDimension(dim);
    }
        
    int  Dimension::getCurrentDimension() const {
        return system_bar->getCurrentDimesnion();
    }
        

    Dimension::~Dimension() {
        std::cout << "MasterX: Releasing Dimensions\n";
    }

    void Dimension::draw(mxApp &app) {
        
        int cur = getCurrentDimension();
        if(cur >= 0 && cur < static_cast<int>(dimensions.size())) {
            dimensions[cur]->draw(app);
        }
        
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