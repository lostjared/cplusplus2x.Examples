#include "dimension.hpp"
#include "window.hpp"
#include "SDL_rect.h"
#include<algorithm>

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
        prev_dim = 0;
        cur_dim = 0;
    }

    SystemBar::~SystemBar() {
    }

    void SystemBar::setDimensions(std::vector<std::unique_ptr<Screen>> *dim) {
        dimensions = dim;
    }

    void SystemBar::activateDimension(int dim) {
        auto it = std::find(activeDimensionsStack.begin(), activeDimensionsStack.end(), dim);
        if (it != activeDimensionsStack.end()) {
            activeDimensionsStack.erase(it);
        }
        activeDimensionsStack.push_back(dim);  
    }

    void SystemBar::deactivateDimension(int dim) {
        auto it = std::find(activeDimensionsStack.begin(), activeDimensionsStack.end(), dim);
        if (it != activeDimensionsStack.end()) {
            activeDimensionsStack.erase(it);
        }
    }
void SystemBar::drawDimensions(mxApp &app) {
    if (dimensions != nullptr && animationComplete == true) {
        int activeIndex = 0;  // For calculating button position
        
        // Loop through active dimensions based on the stack
        for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
            int i = activeDimensionsStack[j];  // Get the dimension index from the stack
            DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
            
            if (dim != nullptr && dim->isActive()) {  // Only draw if the dimension is active
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

                int button_width = 150;  // Button dimensions
                int button_height = text_height + 20;
                int bar_height = 50;
                int button_y = (bar_height - button_height) / 2 + app.height - bar_height;  // Button Y position
                
                // Button X position calculated based on the activeIndex (stack order)
                int button_x = 10 + activeIndex * (button_width + 10);  
                activeIndex++;  // Move to next position

                // Render the button
                if (static_cast<int>(i) == cur_dim) {
                    SDL_SetRenderDrawColor(app.ren, 0, 0, 255, 255);  // Blue for current dimension
                } else {
                    SDL_SetRenderDrawColor(app.ren, 150, 150, 150, 255);  // Gray for others
                }

                SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                SDL_RenderFillRect(app.ren, &buttonRect);

                SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
                SDL_RenderDrawRect(app.ren, &buttonRect);

                // Render button text
                SDL_Rect textRect = {button_x + 10, button_y + (button_height - text_height) / 2, text_width, text_height};
                SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
                SDL_DestroyTexture(textTexture);

                // Draw the close "X" button
                int square_size = 20;
                int square_x = button_x + button_width - square_size - 5;
                int square_y = button_y + (button_height - square_size) / 2;

                SDL_SetRenderDrawColor(app.ren, 200, 200, 200, 255);
                SDL_Rect closeButtonRect = {square_x, square_y, square_size, square_size};
                if (dim->hoveringX) {
                    SDL_SetRenderDrawColor(app.ren, 0xBD, 0, 0, 255);  // Blue when hovered
                }
                SDL_RenderFillRect(app.ren, &closeButtonRect);

                SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
                SDL_RenderDrawRect(app.ren, &closeButtonRect);

                // Render the "X" text
                SDL_Color black = {0, 0, 0, 255};
                SDL_Surface* xSurface = TTF_RenderText_Solid(font, "X", black);
                if (xSurface != nullptr) {
                    SDL_Texture* xTexture = SDL_CreateTextureFromSurface(app.ren, xSurface);
                    int x_width = xSurface->w;
                    int x_height = xSurface->h;
                    SDL_FreeSurface(xSurface);

                    SDL_Rect xRect = {square_x + (square_size - x_width) / 2, square_y + (square_size - x_height) / 2, x_width, x_height};
                    SDL_RenderCopy(app.ren, xTexture, nullptr, &xRect);
                    SDL_DestroyTexture(xTexture);
                }
            }
        }
    }
}
    void SystemBar::setCurrentDimension(int dim) {
        prev_dim = cur_dim;
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

    bool SystemBar::empty() const {
        return activeDimensionsStack.empty();
    }

    bool SystemBar::event(mxApp &app, SDL_Event &e) {
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;

        if (e.type == SDL_MOUSEMOTION) {  // Track mouse motion for hovering
            int barHeight = 50;
            int button_height = 30;  // Assuming the height of the buttons
            int button_y = app.height - barHeight;
            int button_width = 150;
            int square_size = 20;  // Size of the close button square
            int activeIndex = 0;  // Track the position of active dimensions' buttons

            if (dimensions != nullptr) {
                    for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                        int i = activeDimensionsStack[j];
                        DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
                        if (con->isActive()) {
                            int button_x = 10 + activeIndex * (button_width + 10);
                            activeIndex++;
                            SDL_Rect closeButtonRect = {button_x + button_width - square_size - 5, button_y + (button_height - square_size) / 2, square_size, square_size};
                            SDL_Point cur_point { mouseX, mouseY };
                        if (SDL_PointInRect(&cur_point, &closeButtonRect)) {
                            con->hoveringX = true; 
                        } else {
                            con->hoveringX = false;
                        }
                    }
                }
            }
        }

        if (e.type == SDL_MOUSEMOTION) {
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
                int activeIndex = 0;
                for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                    int i = activeDimensionsStack[j];
                    DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
                    if(con->isActive()) {
                        int button_x = 10 + activeIndex * (button_width + 10);
                        activeIndex++;
                        int square_size = 20;
                        SDL_Rect closeButtonRect = {button_x + button_width - square_size - 5, button_y + (button_height - square_size) / 2, square_size, square_size};
                        SDL_Point cur_point = { mouseX, mouseY };
                        if (SDL_PointInRect(&cur_point, &closeButtonRect)) {
                            con->setActive(false);  
                            deactivateDimension(i);
                            setCurrentDimension(prev_dim);
                            return true;
                        } else {
                            SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                            if (mouseX >= buttonRect.x && mouseX <= (buttonRect.x + buttonRect.w) &&
                                mouseY >= buttonRect.y && mouseY <= (buttonRect.y + buttonRect.h)) {
                                setCurrentDimension(i);  
                                return true;
                            } 
                        }
                    }
                }
            }
            int action = menu->itemClicked(app, mouseX, mouseY);
            if(action != -1) {
                performAction(action);
                menu->menuOpen = false;
                menu->animating = true;
                menu->targetY = app.height;
            } else if (menu->menuOpen) {
                int menuX = app.width - (app.width / 6) - 20;
                int menuY = menu->currentY;
                int menuWidth = app.width / 6;
                int menuHeight = app.height / 2;

                SDL_Rect menuRect = {menuX, menuY, menuWidth, menuHeight};

                if (mouseX >= menuRect.x && mouseX <= (menuRect.x + menuRect.w) &&
                    mouseY >= menuRect.y && mouseY <= (menuRect.y + menuRect.h)) {
                    return false;
                }
                menu->menuOpen = false;
                menu->animating = true;
                menu->targetY = app.height; 
                return true;
            }
        }

        for (auto &i : objects) {
            if (i->event(app, e))
                return true;
        }

        return false;
    }

    void SystemBar::performAction(int action) {
        switch (action) {
            case 1: {
                    DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](0).get());
                    dim->setActive(true);
                    setCurrentDimension(0);
                    activateDimension(0);
                }
                break;
            case 2:
                std::cout << "Documents clicked\n";
                break;
            case 3:
                std::cout << "Settings clicked\n";
                break;
            case 4:
                std::cout << "Find clicked\n";
                break;
            case 5: {
                DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](1).get());
                dim->setActive(true);
                setCurrentDimension(1);
                activateDimension(1);
            }
                break;
            case 6:
                std::cout << "Run clicked\n";
                break;
            default:
                std::cout << "No valid action\n";
                break;
        }
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

        const char* items[] = {"Welcome", "Documents", "Settings", "Find", "About", "Run"};
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
    int MenuBar::itemClicked(mxApp &app, int x, int y) {
        int menuX = app.width - (app.width / 6) - 20; 
        int menuY = currentY;
        int itemHeight = 30;  
        int menuWidth = app.width / 6;  

        SDL_Rect menuItem1 = {menuX, menuY + 10, menuWidth, itemHeight};  
        SDL_Rect menuItem2 = {menuX, menuY + 10 + (itemHeight + 5), menuWidth, itemHeight};  
        SDL_Rect menuItem3 = {menuX, menuY + 10 + 2 * (itemHeight + 5), menuWidth, itemHeight};  
        SDL_Rect menuItem4 = {menuX, menuY + 10 + 3 * (itemHeight + 5), menuWidth, itemHeight};  
        SDL_Rect menuItem5 = {menuX, menuY + 10 + 4 * (itemHeight + 5), menuWidth, itemHeight};  
        SDL_Rect menuItem6 = {menuX, menuY + 10 + 5 * (itemHeight + 5), menuWidth, itemHeight};  

        SDL_Point cursor_pos = {x, y};

        if (SDL_PointInRect(&cursor_pos, &menuItem1)) {
            return 1;  // Action for "Programs"
        } else if (SDL_PointInRect(&cursor_pos, &menuItem2)) {
            return 2;  // Action for "Documents"
        } else if (SDL_PointInRect(&cursor_pos, &menuItem3)) {
            return 3;  // Action for "Settings"
        } else if (SDL_PointInRect(&cursor_pos, &menuItem4)) {
            return 4;  // Action for "Find"
        } else if (SDL_PointInRect(&cursor_pos, &menuItem5)) {
            return 5;  // Action for "Help"
        } else if (SDL_PointInRect(&cursor_pos, &menuItem6)) {
            return 6;  // Action for "Run"
        }

        return -1;  // No item clicked
    }
    
    bool MenuBar::event(mxApp &app, SDL_Event &e) {
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

    DimensionContainer::DimensionContainer(mxApp &app) : wallpaper{nullptr} , active{false} {
    
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
    
    void DimensionContainer::setActive(bool a) {
        active = a;
    }

    bool DimensionContainer::isActive() const {
        return active;
    }

    void DimensionContainer::draw(mxApp &app) {

        if(active == false) return;

        if(wallpaper != nullptr) {
            SDL_SetRenderTarget(app.ren, app.tex);
            SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
        }

        for(auto &i : objects) {
            i->draw(app);
        }
    }

    bool DimensionContainer::event(mxApp &app, SDL_Event &e) {

        if(active == false) return false;

        for(auto &i : objects) {
            if(i->event(app, e) == true)
                return true;
        }
        return false;
    }

    Dimension::Dimension(mxApp &app) {
        wallpaper = loadTexture(app, "images/desktop.bmp");
        objects.push_back(std::make_unique<SystemBar>(app));
        system_bar = dynamic_cast<SystemBar *>(objects[0].get());
        dimensions.push_back(std::make_unique<DimensionContainer>(app));

        welcome = dynamic_cast<DimensionContainer *>(dimensions[0].get());
        welcome->init("Welcome", loadTexture(app, "images/wallpaper.bmp"));
        welcome->setActive(true);
        
        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        about = dynamic_cast<DimensionContainer *>(dimensions[1].get());
        about->init("About", loadTexture(app, "images/about.bmp"));
        about->setActive(false);
        system_bar->setDimensions(&dimensions);
        setCurrentDimension(0);
        system_bar->activateDimension(0);

    }

    void Dimension::setCurrentDimension(int dim) {
        system_bar->setCurrentDimension(dim);
    }
        
    int  Dimension::getCurrentDimension() const {
        return system_bar->getCurrentDimesnion();
    }
        

    Dimension::~Dimension() {
        if(wallpaper != nullptr) {
            SDL_DestroyTexture(wallpaper);
        }
        std::cout << "MasterX: Releasing Dimensions\n";
    }

    void Dimension::drawDash(mxApp &app) {
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
    }

    void Dimension::draw(mxApp &app) {
        
        int cur = getCurrentDimension();
        if(cur >= 0 && cur < static_cast<int>(dimensions.size())) {
            if(system_bar->empty()) 
                drawDash(app);
            else
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