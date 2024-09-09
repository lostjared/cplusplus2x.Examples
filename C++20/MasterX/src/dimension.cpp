#include "dimension.hpp"
#include "terminal.hpp"
#include "mx_controls.hpp"
#include "window.hpp"
#include "SDL_rect.h"
#include<algorithm>
#include<unordered_set>

namespace mx {

    bool cursor_shown = false;

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

    void SystemBar::addMinimizedWindow(Window *window) {
          DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
          if(con != nullptr) {
            con->mini_win.push_back(window);
          }
    }

    void SystemBar::restoreWindow(Window *window) {
        if (window->minimized) {
            window->isMinimizing = false;  
            window->restoreTargetX = window->originalX; 
            window->restoreTargetY = window->originalY; 
            window->restoreTargetW = window->originalWidth; 
            window->restoreTargetH = window->originalHeight; 
            window->restoreAnimationStep = 10; 
            window->isRestoring = true;
            window->minimize(false);
            DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
            if(con != nullptr) {
                auto w = std::find(con->mini_win.begin(), con->mini_win.end(), window);
                if(w != con->mini_win.end()) {
                    con->mini_win.erase(w);
                }
            }
        }
    }
    
    void SystemBar::drawMinimizedMenu(mxApp &app, int button_x, int button_y, int button_width) {
        if (!showMinimizedMenu) {
            return;
        }

        DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
        if (!con) return;

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color gray = {200, 200, 200, 255};

        int menu_width = 150;
        int menu_height = 20 + con->mini_win.size() * 30;  

        int menu_x = button_x + (button_width - menu_width) / 2;
        int menu_y = button_y - menu_height - 5;  

        SDL_Rect menuRect = {menu_x, menu_y, menu_width, menu_height};

        SDL_SetRenderDrawColor(app.ren, gray.r, gray.g, gray.b, 255);
        SDL_RenderFillRect(app.ren, &menuRect);

        SDL_SetRenderDrawColor(app.ren, white.r, white.g, white.b, 255);
        SDL_RenderDrawRect(app.ren, &menuRect);

        int yOffset = 20;  
        for (size_t i = 0; i < con->mini_win.size(); ++i) {
            Window *win = con->mini_win[i];

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, win->title.c_str(), white);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(app.ren, textSurface);
            int text_width = textSurface->w;
            int text_height = textSurface->h;
            SDL_FreeSurface(textSurface);
            SDL_Rect textRect = {menu_x + 10, menu_y + yOffset, text_width, text_height};
            SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
            yOffset += 30;  
        }
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
            int activeIndex = 0;  
            
            bool tcursor = false;
                        
            std::unordered_set<int> uniqueDimensions;
            for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                int i = activeDimensionsStack[j];
                if (uniqueDimensions.count(i) == 0) {
                    uniqueDimensions.insert(i);
                    DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
                
                    if (dim != nullptr && dim->isActive()) { 
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
                        
                        
                        int button_x = 10 + activeIndex * (button_width + 10);  
                        activeIndex++;  

                        
                        if (static_cast<int>(i) == cur_dim) {
                            SDL_SetRenderDrawColor(app.ren, 0, 0, 255, 255);  
                        } else {
                            SDL_SetRenderDrawColor(app.ren, 150, 150, 150, 255); 
                        }

                        SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                        SDL_RenderFillRect(app.ren, &buttonRect);

                        SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
                        SDL_RenderDrawRect(app.ren, &buttonRect);

                        
                        SDL_Rect textRect = {button_x + 10, button_y + (button_height - text_height) / 2, text_width, text_height};
                        SDL_RenderCopy(app.ren, textTexture, nullptr, &textRect);
                        SDL_DestroyTexture(textTexture);

                        if (i == cur_dim && dim->mini_win.size()>0 && showMinimizedMenu) {
                                drawMinimizedMenu(app, button_x, button_y, button_width);
                        }

                        if(dim->name != "Dashboard") {
                            int square_size = 20;
                            int square_x = button_x + button_width - square_size - 5;
                            int square_y = button_y + (button_height - square_size) / 2;

                            SDL_SetRenderDrawColor(app.ren, 200, 200, 200, 255);
                            SDL_Rect closeButtonRect = {square_x, square_y, square_size, square_size};
                            if (dim->hoveringX) {
                                SDL_SetRenderDrawColor(app.ren, 0xBD, 0, 0, 255);  
                                tcursor = true;
                            } 
                            SDL_RenderFillRect(app.ren, &closeButtonRect);

                            SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
                            SDL_RenderDrawRect(app.ren, &closeButtonRect);

                            
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
            if(tcursor == true) {
                cursor_shown = true;
            } 
        }
    }
    void SystemBar::setCurrentDimension(int dim) {
        if(dim != cur_dim) {
            DimensionContainer *old, *setv;
            old = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
            setv = dynamic_cast<DimensionContainer *>(dimensions->operator[](dim).get());
            prev_dim = cur_dim;
            cur_dim = dim;
            if (setv) {
                setv->setActive(true);  
                setv->setVisible(true);
                std::cout << "MasterX System: Switching from: " << old->name << " to " << setv->name << "\n";
                setv->startTransition(old ? old->wallpaper : nullptr);  
            }
        }    
    }

    int  SystemBar::getCurrentDimension() const {
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
                DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
                con->setVisible(true);
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
            cursor_shown = true;
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

        DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](cur_dim).get());
        if (!con) {
            return false;  
        }
        
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        int button_width = 150;  
        int button_y = app.height - 50;  
        int button_x = 10 + cur_dim * (button_width + 10);  
        int menu_width = 150;
        int menu_height = 20 + con->mini_win.size() * 30;  

        int menu_x = button_x + (button_width - menu_width) / 2;
        int menu_y = button_y - menu_height - 5;  

        if (showMinimizedMenu && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;  
            int mouseY = e.button.y;  
            SDL_Rect menuRect = {menu_x, menu_y, menu_width, menu_height};
            SDL_Point rc = {mouseX, mouseY};
            if (SDL_PointInRect(&rc, &menuRect)) {            
                int yOffset = 20;
                for (size_t i = 0; i < con->mini_win.size(); ++i) {
                    SDL_Rect textRect = {menu_x + 10, menu_y + yOffset, 180, 20};  
                    if (SDL_PointInRect(&rc, &textRect)) {
                        restoreWindow(con->mini_win[i]);  
                        showMinimizedMenu = false;
                        return true;
                    }
                    yOffset += 30;  
                }
            } else {
                showMinimizedMenu = false;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            int button_width = 150;
            int button_height = 30;
            int button_y = app.height - 50;

            for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                if(static_cast<int>(j) != cur_dim)
                    continue;
                int button_x = 10 + j * (button_width + 10);


                SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                SDL_Point mousePoint = {mouseX, mouseY};

                if (SDL_PointInRect(&mousePoint, &buttonRect)) {
                    if (holdingDimension) {
                        Uint32 holdTime = SDL_GetTicks() - holdStartTime;
                        if (holdTime > 500) {
                            showMinimizedMenu = true;
                            holdingDimension = false;
                        }
                    } else {
                        showMinimizedMenu = !showMinimizedMenu;
                    }
                    break;
                }
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            bool clickedOutside = true;

            int button_width = 150;
            int button_height = 30;
            int button_y = app.height - 50;

            for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                int button_x = 10 + j * (button_width + 10);
                SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                SDL_Point mousePoint = {mouseX, mouseY};

                if (SDL_PointInRect(&mousePoint, &buttonRect)) {
                    clickedOutside = false;
                    break;
                }
            }

            SDL_Rect menuRect = {menu_x, menu_y, menu_width, menu_height};
            if (mouseX >= menuRect.x && mouseX <= menuRect.x + menuRect.w &&
                mouseY >= menuRect.y && mouseY <= menuRect.y + menuRect.h) {
                clickedOutside = false;
            }

            if (clickedOutside) {
                showMinimizedMenu = false;
            }
        }
        
        if (e.type == SDL_MOUSEMOTION) {  
            int barHeight = 50;
            int button_height = 30;  
            int button_y = app.height - barHeight;
            int button_width = 150;
            int square_size = 20;  
            int activeIndex = 0;  

            if (dimensions != nullptr) {
                    for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
                        int i = activeDimensionsStack[j];
                        DimensionContainer *con = dynamic_cast<DimensionContainer *>(dimensions->operator[](i).get());
                        if (con->isActive()) {
                            int button_x = 10 + activeIndex * (button_width + 10);
                            activeIndex++;
                            SDL_Rect closeButtonRect = {button_x + button_width - square_size - 5, button_y + (button_height - square_size) / 2, square_size, square_size};
                            SDL_Point cur_point { mouseX, mouseY };
                        if (con->name != "Dashboard" && SDL_PointInRect(&cur_point, &closeButtonRect)) {
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


        if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            if (holdingDimension) {
                Uint32 holdTime = SDL_GetTicks() - holdStartTime;
                
                if (holdTime > 500) { 
                    showMinimizedMenu = true;  
                }

                holdingDimension = false;  
                return true;
            }
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
                        if (con->name != "Dashboard" && SDL_PointInRect(&cur_point, &closeButtonRect)) {
                            con->setActive(false);  
                            if(i == cur_dim) {
                                deactivateDimension(i);
                                prev_dim = 0;
                                setCurrentDimension(0); 
                            } else {
                                deactivateDimension(i);
                            }
                            return true;
                        } else {
                            SDL_Rect buttonRect = {button_x, button_y, button_width, button_height};
                            if (mouseX >= buttonRect.x && mouseX <= (buttonRect.x + buttonRect.w) &&
                                mouseY >= buttonRect.y && mouseY <= (buttonRect.y + buttonRect.h)) {
                                if(getCurrentDimension() != i) {
                                    setCurrentDimension(i);  
                                    return true;
                                }
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
                    setCurrentDimension(1);
                    activateDimension(1);
                }
                break;
            case 2: {
                    DimensionContainer *dim = dynamic_cast<DimensionContainer *>(dimensions->operator[](2).get());
                    dim->setActive(true);
                    setCurrentDimension(3);
                    activateDimension(3);
            }
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
                setCurrentDimension(2);
                activateDimension(2);
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

        const char* items[] = {"Welcome", "Terminal", "Settings", "Find", "About", "Run"};
        int numItems = sizeof(items) / sizeof(items[0]);
        int itemHeight = 30;
        SDL_Rect itemRect;

        bool tcursor = false;

        for (int i = 0; i < numItems; i++) {
            itemRect = {menuX + 10, currentY + 10 + i * (itemHeight + 5), menuWidth - 20, itemHeight};

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            SDL_Point cursor_pos = {mouseX, mouseY};
            bool isHovering = SDL_PointInRect(&cursor_pos, &itemRect);

            if (isHovering) {
                SDL_SetRenderDrawColor(app.ren, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
                SDL_RenderFillRect(app.ren, &itemRect);
                tcursor = true;
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

        if(tcursor == true) {
            cursor_shown = true;
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

        if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_LEAVE) {
                this->animating = false;
                this->menuOpen = true;
                return false;
            }
        }

        return false;
    }


 

    Window::Window(mxApp &app) : x{0}, y{0}, w{320}, h{240}, shown{false}, minimizeHovered(SDL_FALSE), closeHovered(SDL_FALSE), maximizeHovered(SDL_FALSE), title{"windwow"} {
        dim_w = app.width;
        dim_h = app.height;
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
        orig_x = x;
        orig_y = y;
    }


    void Window::draw(mxApp &app) {
      
        if(shown == false || (minimized == true && isMinimizing == false)) return;

        if (isMinimizing) {
            x += (minTargetX - x) / minAnimationStep;
            y += (minTargetY - y) / minAnimationStep;
            w += (minTargetW - w) / minAnimationStep;
            h += (minTargetH - h) / minAnimationStep;

            if (abs(x - minTargetX) < 1 && abs(y - minTargetY) < 1 && 
                abs(w - minTargetW) < 1 && abs(h - minTargetH) < 1) {
                isMinimizing = false;
                minimized = true;
                minimizeHovered = SDL_FALSE;
                maximizeHovered = SDL_FALSE;
                closeHovered = SDL_FALSE;                                       
            }
        }

        if (isRestoring) {
            
            x += (restoreTargetX - x) / restoreAnimationStep;
            y += (restoreTargetY - y) / restoreAnimationStep;
            w += (restoreTargetW - w) / restoreAnimationStep;
            h += (restoreTargetH - h) / restoreAnimationStep;

            if (abs(x - restoreTargetX) < 1 && abs(y - restoreTargetY) < 1 && 
                abs(w - restoreTargetW) < 1 && abs(h - restoreTargetH) < 1) {
                x = restoreTargetX;
                y = restoreTargetY;
                w = restoreTargetW;
                h = restoreTargetH;
                isRestoring = false;
                minimized = false;
                minimizeHovered = SDL_FALSE;
                maximizeHovered = SDL_FALSE;
                closeHovered = SDL_FALSE;

                for(auto &c : children) {
                    c->setWindowPos(x, y);
                    c->resizeWindow(w, h);
                }
            }
                x = restoreTargetX;
                y = restoreTargetY;
                w = restoreTargetW;
                h = restoreTargetH;
                isRestoring = false; 
                dragging = false;
        }

        SDL_Rect rc = {x, y, w, h};
        SDL_SetRenderDrawColor(app.ren, 205, 205, 205, 255);
        SDL_RenderFillRect(app.ren, &rc);
        SDL_Color lightBlue = {173, 216, 230}; 
        SDL_Color darkBlue = {0, 0, 139};     
        int titleBarHeight = 30;
        for (int i = 0; i < titleBarHeight; ++i) {
            int red = lightBlue.r + (darkBlue.r - lightBlue.r) * i / titleBarHeight;
            int green = lightBlue.g + (darkBlue.g - lightBlue.g) * i / titleBarHeight;
            int blue = lightBlue.b + (darkBlue.b - lightBlue.b) * i / titleBarHeight;
            SDL_SetRenderDrawColor(app.ren, red, green, blue, 255);
            SDL_RenderDrawLine(app.ren, x, y + i, x + w, y + i);
        }

        int buttonSize = 20;
        int buttonPadding = 5;

        minimizeButton = {x + w - 3 * (buttonSize + buttonPadding), y + 5, buttonSize, buttonSize};
        maximizeButton = {x + w - 2 * (buttonSize + buttonPadding), y + 5, buttonSize, buttonSize};
        closeButton = {x + w - (buttonSize + buttonPadding), y + 5, buttonSize, buttonSize};

        SDL_RenderDrawLine(app.ren, minimizeButton.x, minimizeButton.y, minimizeButton.x + buttonSize, minimizeButton.y);
        SDL_RenderDrawLine(app.ren, minimizeButton.x, minimizeButton.y, minimizeButton.x, minimizeButton.y + buttonSize);
        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);
        SDL_RenderDrawLine(app.ren, minimizeButton.x + buttonSize, minimizeButton.y, minimizeButton.x + buttonSize, minimizeButton.y + buttonSize);
        SDL_RenderDrawLine(app.ren, minimizeButton.x, minimizeButton.y + buttonSize, minimizeButton.x + buttonSize, minimizeButton.y + buttonSize);
        if (minimizeHovered) {
            SDL_SetRenderDrawColor(app.ren, 255, 0, 0, 255);  
            cursor_shown = true;
        } else {
            SDL_SetRenderDrawColor(app.ren, 150, 150, 150, 255); 
        }
        SDL_RenderFillRect(app.ren, &minimizeButton);
        if (closeHovered) {
            cursor_shown = true;
            SDL_SetRenderDrawColor(app.ren, 255, 0, 0, 255);  
        } else {
            SDL_SetRenderDrawColor(app.ren, 150, 150, 150, 255);  
        }
        SDL_RenderFillRect(app.ren, &closeButton);
        if (maximizeHovered) {
            cursor_shown = true;
            SDL_SetRenderDrawColor(app.ren, 255, 0, 0, 255);  
        } else {
            SDL_SetRenderDrawColor(app.ren, 150, 150, 150, 255);  
        }
        SDL_RenderFillRect(app.ren, &maximizeButton);

        SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(app.ren, maximizeButton.x + 2, maximizeButton.y + 2, maximizeButton.x + buttonSize - 2, maximizeButton.y + 2);
        SDL_RenderDrawLine(app.ren, maximizeButton.x + 2, maximizeButton.y + 2, maximizeButton.x + 2, maximizeButton.y + buttonSize - 2);
        SDL_RenderDrawLine(app.ren, maximizeButton.x + 2, maximizeButton.y + buttonSize - 2, maximizeButton.x + buttonSize - 2, maximizeButton.y + buttonSize - 2);
        SDL_RenderDrawLine(app.ren, maximizeButton.x + buttonSize - 2, maximizeButton.y + 2, maximizeButton.x + buttonSize - 2, maximizeButton.y + buttonSize - 2);
 
        SDL_RenderDrawLine(app.ren, closeButton.x, closeButton.y, closeButton.x + buttonSize, closeButton.y);
        SDL_RenderDrawLine(app.ren, closeButton.x, closeButton.y, closeButton.x, closeButton.y + buttonSize);
        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);
        SDL_RenderDrawLine(app.ren, closeButton.x + buttonSize, closeButton.y, closeButton.x + buttonSize, closeButton.y + buttonSize);
        SDL_RenderDrawLine(app.ren, closeButton.x, closeButton.y + buttonSize, closeButton.x + buttonSize, closeButton.y + buttonSize);
        SDL_Surface* closeSurface = TTF_RenderText_Solid(app.font, "X", {255, 255, 255});
        SDL_Texture* closeTexture = SDL_CreateTextureFromSurface(app.ren, closeSurface);
        int closeTextW = 0, closeTextH = 0;
        SDL_QueryTexture(closeTexture, nullptr, nullptr, &closeTextW, &closeTextH);
        SDL_Rect closeTextRect = {closeButton.x + (buttonSize - closeTextW) / 2, closeButton.y + (buttonSize - closeTextH) / 2, closeTextW, closeTextH};
        SDL_RenderCopy(app.ren, closeTexture, nullptr, &closeTextRect);
        SDL_DestroyTexture(closeTexture);
        SDL_FreeSurface(closeSurface);

        SDL_Surface* minimizeSurface = TTF_RenderText_Solid(app.font, "_", {255, 255, 255});
        SDL_Texture* minimizeTexture = SDL_CreateTextureFromSurface(app.ren, minimizeSurface);
        int minimizeTextW = 0, minimizeTextH = 0;
        SDL_QueryTexture(minimizeTexture, nullptr, nullptr, &minimizeTextW, &minimizeTextH);
        SDL_Rect minimizeTextRect = {minimizeButton.x + (buttonSize - minimizeTextW) / 2, minimizeButton.y + (buttonSize - minimizeTextH) / 2, minimizeTextW, minimizeTextH};
        SDL_RenderCopy(app.ren, minimizeTexture, nullptr, &minimizeTextRect);
        SDL_DestroyTexture(minimizeTexture);
        SDL_FreeSurface(minimizeSurface);

        SDL_SetRenderDrawColor(app.ren, 255, 255, 255, 255);
        SDL_RenderDrawLine(app.ren, x, y + titleBarHeight, x + w - 1, y + titleBarHeight);
        SDL_RenderDrawLine(app.ren, x, y, x + w - 1, y);
        SDL_RenderDrawLine(app.ren, x, y, x, y + h - 1);

        SDL_SetRenderDrawColor(app.ren, 128, 128, 128, 255);
        SDL_RenderDrawLine(app.ren, x + w - 1, y, x + w - 1, y + h - 1);
        SDL_RenderDrawLine(app.ren, x, y + h - 1, x + w - 1, y + h - 1);

        SDL_SetRenderDrawColor(app.ren, 192, 192, 192, 255);
        SDL_RenderDrawLine(app.ren, x + 1, y + titleBarHeight + 1, x + w - 2, y + titleBarHeight + 1);
        SDL_RenderDrawLine(app.ren, x + 1, y + titleBarHeight + 1, x + 1, y + h - 2);

        SDL_SetRenderDrawColor(app.ren, 64, 64, 64, 255);
        SDL_RenderDrawLine(app.ren, x + w - 2, y + titleBarHeight + 1, x + w - 2, y + h - 2);
        SDL_RenderDrawLine(app.ren, x + 1, y + h - 2, x + w - 2, y + h - 2);

        SDL_Surface* surface = TTF_RenderText_Solid(app.font, title.c_str(), {255, 255, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app.ren, surface);
        int textW = 0, textH = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {x + 25, y + 5, textW, textH};
        SDL_RenderCopy(app.ren, texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        int iconWidth = 16;
        int iconHeight = 16;
        int iconX = x + 5;  
        int iconY = y + (titleBarHeight - iconHeight) / 2;  
        SDL_Rect iconRect = { iconX, iconY, iconWidth, iconHeight };
        SDL_RenderCopy(app.ren, app.icon, nullptr, &iconRect);
        for (auto &c : children) {
            c->setWindowPos(x, y);
            c->draw(app);
        }
    }

    bool Window::isVisible() const {
        return shown;
    }

    void Window::show(bool b) {
        shown = b;
        stateChanged(false, false, shown);
    }

    void Window::getRect(SDL_Rect &rc) {
        rc.x = x;
        rc.y = y;
        rc.w = w;
        rc.h = h;
    }

    void Window::setRect(const SDL_Rect &rc) {
        x = rc.x;
        y = rc.y;
        w = rc.w;
        h = rc.h;
    }
   bool Window::event(mxApp &app, SDL_Event &e) {
        
        static Uint32 lastClickTime = 0;   
        if (e.type == SDL_MOUSEMOTION) {
            SDL_Point mousePoint = {e.motion.x, e.motion.y};
            minimizeHovered = SDL_PointInRect(&mousePoint, &minimizeButton);
            closeHovered = SDL_PointInRect(&mousePoint, &closeButton);
            maximizeHovered = SDL_PointInRect(&mousePoint, &maximizeButton);
            if (dragging) {
                    if (e.motion.y > 0) {
                        x = e.motion.x - dragOffsetX;
                        y = e.motion.y - dragOffsetY;
                        isMinimizing = false;
                        isRestoring = false;
                        return true;
                    }
            }          
        } 
        
        if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            SDL_Point mousePoint = {e.button.x, e.button.y};
            SDL_Rect titleBarRect = {x, y, w, 30};

            if (SDL_PointInRect(&mousePoint, &titleBarRect) &&
                !SDL_PointInRect(&mousePoint, &minimizeButton) &&
                !SDL_PointInRect(&mousePoint, &closeButton)) {
                dragging = true;
                dragOffsetX = e.button.x - x;
                dragOffsetY = e.button.y - y;
            }

            if (SDL_PointInRect(&mousePoint, &closeButton)) {
                show(false);
            }

            if (SDL_PointInRect(&mousePoint, &minimizeButton)) {
                if (!minimized) {
                    minimize(true);
                    if (systemBar) systemBar->addMinimizedWindow(this);
                    minimizeHovered = SDL_FALSE;
                } else {
                    systemBar->restoreWindow(this);
                    if (systemBar) systemBar->restoreWindow(this);
                    minimizeHovered = SDL_FALSE;
                }
            }
                
            if (canResize() == true && minimized == false && SDL_PointInRect(&mousePoint, &maximizeButton)) {

                maximize(!maximized);
                shown = true;
                if (can_resize) {
                    for (auto &c : children) {
                        if(c->show) {
                            c->setWindowPos(x,y);
                            c->resizeWindow(w, h);
                        }
                    }
                }
            }
        } 
        
        
        if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            dragging = false;
            isMinimizing = false;
            isRestoring = false;
        }

        if (!shown || minimized) {
            
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                Uint32 currentTime = SDL_GetTicks();  
                SDL_Point mousePoint = {e.button.x, e.button.y};
                SDL_Rect titleBarRect = {x, y, w, 30};  
                if (SDL_PointInRect(&mousePoint, &titleBarRect)) {
                    if (currentTime - lastClickTime < 500) {  
                        if (minimized) {
                            minimize(false);  
                            if (systemBar) systemBar->restoreWindow(this);
                            return true;
                        }
                    }       
                    lastClickTime = currentTime;
                }
            }
            return false;
        }

        if (e.type == SDL_WINDOWEVENT) {
            if (e.window.event == SDL_WINDOWEVENT_LEAVE) {
                minimizeHovered = SDL_FALSE;
                closeHovered = SDL_FALSE;
                maximizeHovered = SDL_FALSE;
            }
        }

        for (auto &c : children) {
            if (c->event(app, e)) return true;
        }
     
      
        return false;
    }

    void Window::setSystemBar(SystemBar *s) {
        systemBar = s;
    }

    Control *Window::getControl() {
        if(children.size()>0)
            return children[children.size()-1].get();
        std::cerr << "MasterX System: Trying to acess out of bounds control.\n";
        return nullptr;
    }
    Control *Window::getControl(int index) {
        if(index >= 0 && index < static_cast<int>(children.size()))
            return children[index].get();
        std::cerr << "MasterX System: Trying to acess out of bounds control.\n";
        return nullptr;
    }

    void Window::minimize(bool m) {
        if (m) {
           if (!minimized) {  
                originalX = x;
                originalY = y;
                originalWidth = w;
                originalHeight = h;
            }
            isMinimizing = true;
            minimized = true;
            minTargetX = 5;  
            minTargetY = systemBar->yPos - 50;
            minTargetW = 200;  
            minTargetH = 5; 
            originalX = x;
            originalY = y;  
            originalWidth = w;
            originalHeight = h;
            if(isVisible()) {  
                for (auto &c : children) {
                    c->setShow(false);  
                }
            }
            w = 200;
            h = 25;
            
        } else if (!m && minimized) {
            minimized = false;
            isRestoring = true;
            restoreTargetX = originalX;
            restoreTargetY = originalY;
            restoreTargetW = originalWidth;
            restoreTargetH = originalHeight;
            shown = true;
            dragging = false;
            if(isVisible()) {
                for (auto &c : children) {
                    c->setShow(true);  
                }
            }
        }
        stateChanged(m, false, false);
    }

    void Window::maximize(bool m) {
        if (m && !maximized) {
            oldX = x;
            oldY = y;
            oldW = w;
            oldH = h;
            x = 0;
            y = 0;
            w = dim_w;
            h = dim_h-50;
        } else if (!m && maximized) {
            x = oldX;
            y = oldY;
            w = oldW;
            h = oldH;
        }
        maximized = m;
        stateChanged(false, maximized, false);
    }

    bool Window::reload() const {
        return reload_window;
    }
    
    void Window::setReload(bool r) {
        reload_window = r;
    }

   void Window::setCanResize(bool r) {
        can_resize = r;
   }
    
    bool Window::canResize() const {
        return can_resize;
    }
  
    DimensionContainer::DimensionContainer(mxApp &app) : wallpaper{nullptr} , active{false} {}

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
        if(active) {
            for (auto &i : objects) {
                if(auto win = dynamic_cast<Window *>(i.get())) {
                    if(win->reload()) {
                        win->show(true);
                    }
                }
            }
        }
    }

    bool DimensionContainer::isActive() const {
        return active;
    }

    void DimensionContainer::draw(mxApp &app) {
        if (!active) return;

        SDL_SetRenderDrawBlendMode(app.ren, SDL_BLENDMODE_BLEND);

        if (wallpaper != nullptr) {
            if (transitioning && nextWallpaper != nullptr) {
                SDL_SetTextureBlendMode(wallpaper, SDL_BLENDMODE_BLEND);
                SDL_SetTextureBlendMode(nextWallpaper, SDL_BLENDMODE_BLEND);
                SDL_SetTextureAlphaMod(wallpaper, 255 - transitionAlpha);
                SDL_SetTextureAlphaMod(nextWallpaper, transitionAlpha);
                SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
                SDL_RenderCopy(app.ren, nextWallpaper, nullptr, nullptr);
                transitionAlpha -= 5;
                if (transitionAlpha <= 0) {
                    transitioning = false;
                    SDL_SetTextureBlendMode(wallpaper, SDL_BLENDMODE_NONE);
                }
            } else {
                SDL_SetTextureBlendMode(wallpaper, SDL_BLENDMODE_NONE);
                SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
            }
        }

        if (visible) {
            for (auto &i : objects) {
                i->draw(app);
            }
        }
    }
    
    void DimensionContainer::startTransition(SDL_Texture* nextWp) {
        if (!transitioning) {
            nextWallpaper = nextWp;
            transitionAlpha = 255;
            transitioning = true;
        }
    }
    void DimensionContainer::updateTransition() {
        transitionAlpha -= transitionSpeed;  
        if (transitionAlpha <= 0) {
            transitionAlpha = 255;
            transitioning = false;
        }
    }

    bool DimensionContainer::event(mxApp &app, SDL_Event &e) {

        if(active == false || visible == false) return false;

        for(auto &i : objects) {
            if(i->event(app, e) == true)
                return true;
        }
        return false;
    }

    void DimensionContainer::setVisible(bool v) {
        visible = v;
    }

    Dimension::Dimension(mxApp &app) {
        wallpaper = loadTexture(app, "images/desktop.bmp");
        objects.push_back(std::make_unique<SystemBar>(app));
        system_bar = dynamic_cast<SystemBar *>(objects[0].get());

        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        dash = dynamic_cast<DimensionContainer *>(getDimension());
        dash->init("Dashboard", loadTexture(app, "images/desktop.bmp"));
        dash->setActive(true);
        dash->setVisible(false);

        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        welcome = dynamic_cast<DimensionContainer *>(getDimension());
        welcome->init("Welcome", loadTexture(app, "images/wallpaper.bmp"));
        welcome->setActive(false);
        welcome->setVisible(false);
        welcome->objects.push_back(std::make_unique<Window>(app));
        welcome_window = dynamic_cast<Window *>(welcome->objects[0].get());
        welcome_window->create("Welcome", 45, 25, 640, 480);
        welcome_window->show(true);
        welcome_window->setReload(false);
        welcome_window->setCanResize(true);
        welcome_window->children.push_back(std::make_unique<Image>(app));
        welcome_image = dynamic_cast<Image *>(welcome_window->getControl());
        welcome_image->create(app, welcome_window, "images/welcome_logo.bmp", 45, 45);
        welcome_image->setGeometry(5, 5, 640-10, 480-35);
        welcome_window->children.push_back(std::make_unique<Button>(app));
        welcome_ok = dynamic_cast<Button *>(welcome_window->getControl());
        SDL_Rect i_rc;
        welcome_window->getRect(i_rc);
        welcome_ok->create(welcome_window, "Dismiss", i_rc.w-110, i_rc.h-35, 100, 25);
        welcome_ok->setShow(true);
        welcome_ok->setCallback([](mxApp &app, Window *parent, SDL_Event &e) -> bool {
            parent->show(false);
            return true;
        });
        welcome_ok->setResizeCallback([&](Window *parent, int x, int y) -> void {
            SDL_Rect rc,src;
            parent->getRect(rc);
            welcome_ok->setGeometry(rc.w - 110, rc.h - 40, 100, 30);
            welcome_image->getRect(src);
            welcome_image->setGeometry(src.x, src.y, rc.w-10, rc.h-35);    
            welcome_image->setWindowPos(rc.x, rc.y);
        });
        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        about = dynamic_cast<DimensionContainer *>(getDimension());
        about->init("About", loadTexture(app, "images/about.bmp"));
        about->setActive(false);
        about->setVisible(false);
        about->objects.push_back(std::make_unique<Window>(app));
        about_window = dynamic_cast<Window *>(about->objects[0].get());
        int centered_x = (app.width - 800) / 2;
        int centered_y = (app.height - 600) / 2;
        about_window->create("About", centered_x, centered_y-35, 800, 600);
        about_window->show(true);
        about_window->setReload(false);

        about_window->children.push_back(std::make_unique<Image>(app));
        Image *image = dynamic_cast<Image *>(about_window->getControl());
        image->create(app, about_window, "images/logo.bmp", 0, 0);
        image->setGeometry(5, 5, 800-10, 600-35);

        about_window->children.push_back(std::make_unique<Button>(app));
        about_window_ok = dynamic_cast<Button *>(about_window->getControl());

        about_window_ok->create(about_window, "Ok", 800-110, 600-35, 100, 25);
        about_window_ok->setShow(true);
        about_window_ok->setCallback([](mxApp &app, Window *parent, SDL_Event &e) -> bool {
            parent->show(false);
            return false;
        });
        about_window->children.push_back(std::make_unique<Label>(app));
        about_window_info = dynamic_cast<Label *>(about_window->getControl());
        std::vector<std::string> info_text {"MasterX System", "written by Jared Bruni", "(C) 2024 LostSideDead Software", "https://lostsidedead.biz", "\"Open Source, Open Mind\""};
        about_window_info->create_multi(about_window, info_text, { 255,255,255,255}, 25, 25 );
        about_window_info->loadFont("fonts/arial.ttf", 36);
        about_window_info->linkMode(false);
        about_window->setCanResize(false);
        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        term = dynamic_cast<DimensionContainer *>(getDimension());
        term->init("Terminal", loadTexture(app, "images/terminal.bmp"));
        term->setActive(false);
        term->setVisible(false);
        system_bar->activateDimension(1);
        system_bar->activateDimension(0);
        term->objects.push_back(std::make_unique<Terminal>(app)); 
        termx = dynamic_cast<Terminal*>(term->objects[0].get());
        termx->create("mXTerm", 25, 25, 640, 480);
        termx->show(true);
        termx->setReload(true);
        system_bar->setDimensions(&dimensions);
        welcome_window->setSystemBar(system_bar);
        about_window->setSystemBar(system_bar);
        termx->setSystemBar(system_bar);
        setCurrentDimension(1);
        system_bar->activateDimension(1);
        SDL_Surface *hand_cursor_surf = SDL_LoadBMP(getPath("images/hand.bmp").c_str());
        if(hand_cursor_surf == nullptr) {
            std::cerr << "MasterX: Error loading cursor..\n";
            exit(EXIT_FAILURE);
        }
        SDL_SetColorKey(hand_cursor_surf, SDL_TRUE, SDL_MapRGB(hand_cursor_surf->format, 0, 0, 0));
        hand_cursor = SDL_CreateTextureFromSurface(app.ren, hand_cursor_surf);
        SDL_FreeSurface(hand_cursor_surf);
        if(hand_cursor == nullptr) {
            std::cerr << "MasterX System: Error creating texture from surface...\n";
            exit(EXIT_FAILURE);
        }
        SDL_Surface *cursor_surf = SDL_LoadBMP(getPath("images/cursor.bmp").c_str());
        if(cursor_surf == nullptr) {
            std::cerr << "MasterX: Error loading cursor..\n";
            exit(EXIT_FAILURE);
        }
        SDL_SetColorKey(cursor_surf, SDL_TRUE, SDL_MapRGB(cursor_surf->format, 0, 0, 0));
        reg_cursor = SDL_CreateTextureFromSurface(app.ren, cursor_surf);
        SDL_FreeSurface(cursor_surf);
        if(reg_cursor == nullptr) {
            std::cerr << "MasterX System: Error creating texture from surface...\n";
            exit(EXIT_FAILURE);
        }
    }

    void Dimension::setCurrentDimension(int dim) {
        system_bar->setCurrentDimension(dim);
    }
        
    int  Dimension::getCurrentDimension() const {
        return system_bar->getCurrentDimension();
    }
        

    Dimension::~Dimension() {
        if(wallpaper != nullptr) {
            SDL_DestroyTexture(wallpaper);
        }
        if(hand_cursor != nullptr) {
            SDL_DestroyTexture(hand_cursor);
        }
        if(reg_cursor != nullptr) {
            SDL_DestroyTexture(reg_cursor);
        }
        std::cout << "MasterX: Releasing Dimensions\n";
    }

    void Dimension::drawDash(mxApp &app) {
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
    }

    void Dimension::draw(mxApp &app) {
        cursor_shown = false;
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
        SDL_SetRenderTarget(app.ren, app.tex);
        SDL_Rect rc = { cursor_x, cursor_y, 32, 32 };
        if(cursor_shown) { 
            SDL_RenderCopy(app.ren, hand_cursor, nullptr, &rc);
        } else {
            SDL_RenderCopy(app.ren, reg_cursor, nullptr, &rc);
        }
        SDL_SetRenderTarget(app.ren, nullptr);
    }

   Screen *Dimension::getDimension() {
        if(dimensions.size()>0)
            return dimensions[dimensions.size()-1].get();
        std::cerr << "MasterX System: Trying to access out of bounds dimension.\n";
        return nullptr;
   }
    
    Screen *Dimension::getDimension(int index) {
        if(index >= 0 && index < static_cast<int>(dimensions.size()))
            return dimensions[index].get();

        std::cerr << "MasterX System: Trying to access out of bounds dimension.\n";
        return nullptr;
    }

    bool Dimension::event(mxApp &app, SDL_Event &e) {
        for (auto &i : objects) {
            if(i->event(app, e))
                return true;
        }
        int cur = getCurrentDimension();
        if(cur >= 0 && cur < static_cast<int>(dimensions.size())) {
            dimensions[cur]->event(app, e);
        }
        if(e.type == SDL_MOUSEMOTION) {
            cursor_x = e.motion.x;
            cursor_y = e.motion.y;
        }
        return false;
    }
}