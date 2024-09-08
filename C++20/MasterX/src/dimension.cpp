#include "dimension.hpp"
#include "window.hpp"
#include "SDL_rect.h"
#include<algorithm>

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
        
        for (size_t j = 0; j < activeDimensionsStack.size(); ++j) {
            int i = activeDimensionsStack[j]; 
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
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        
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
        return false;
    }


    Label::Label(mxApp &app)
        : font_(nullptr), name_(""), text_(""), x(0), y(0), size_(0), wx(0), wy(0) {
    }

    Label::~Label() {
    }

    void Label::setWindowPos(int xx, int yy) {
        wx = xx;
        wy = yy;
    }

    void Label::draw(mxApp &app) {

        if(font_ != nullptr && text_.length() > 0) {
            SDL_Surface *surf = TTF_RenderText_Solid(font_, text_.c_str(), color_);
            if(surf == nullptr) {
                std::cerr << "MasterX System: Error creating surface.\n";
                exit(EXIT_FAILURE);
            }


            int sw = surf->w;
            int sh = surf->h;

            SDL_Texture *t = SDL_CreateTextureFromSurface(app.ren, surf);
            if(t == nullptr) {
                std::cerr << "MasterX System: Error creating texture.\n";
                exit(EXIT_FAILURE);
            }

            SDL_FreeSurface(surf);
            SDL_SetRenderTarget(app.ren, app.tex);
            SDL_Rect point = {x+wx,y+wy+25,sw,sh};
            SDL_RenderCopy(app.ren, t, nullptr, &point);
        }

    }

    void Label::create(const std::string &text, SDL_Color col, int xx, int yy) {
        setText(text, col);
        setGeometry(xx, yy);
    }

    bool Label::event(mxApp &app, SDL_Event &e) {
        return false;
    }

    void Label::loadFont(const std::string &name, int sizex) {
        name_ = name;
        size_ = sizex;
        font_ = TTF_OpenFont(getPath(name).c_str(), size_);
        if(font_ == nullptr) {
            std::cerr << "Error opening font: " << getPath(name) << "\n";
            exit(EXIT_FAILURE);
        }
    }

    void Label::setText(const std::string &text, SDL_Color col) {
        text_ = text;
        color_ = col;
    }

    void Label::setGeometry(int xx, int yy) {
        x = xx;
        y = yy;
    }

    Window::Window(mxApp &app) : x{0}, y{0}, w{320}, h{240}, title{"windwow"}, shown{false}, minimizeHovered(SDL_FALSE), closeHovered(SDL_FALSE) {
   
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

        if(shown == false)
            return;

        SDL_Rect rc = {x, y, w, h};
        SDL_SetRenderDrawColor(app.ren, 205, 205, 205, 255);
        SDL_RenderFillRect(app.ren, &rc);

        int titleBarHeight = 30;
        for (int i = 0; i < titleBarHeight; ++i) {
            int gradientColor = 128 - (128 * i / titleBarHeight);
            SDL_SetRenderDrawColor(app.ren, 0, 0, gradientColor, 255);
            SDL_RenderDrawLine(app.ren, x, y + i, x + w, y + i);
        }

        int buttonSize = 20;
        int buttonPadding = 5;

        minimizeButton = {x + w - 2 * (buttonSize + buttonPadding), y + 5, buttonSize, buttonSize};
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
        SDL_Rect textRect = {x + 5, y + 5, textW, textH};
        SDL_RenderCopy(app.ren, texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

        for (auto &c : children) {
            c->setWindowPos(x, y);
            c->draw(app);
        }
    }

    void Window::show(bool b) {
        shown = b;
    }

    bool Window::event(mxApp &app, SDL_Event &e) {

        if(shown == false) return false;

        if (e.type == SDL_MOUSEMOTION) {
            SDL_Point mousePoint = {e.motion.x, e.motion.y};
            minimizeHovered = SDL_PointInRect(&mousePoint, &minimizeButton);
            closeHovered = SDL_PointInRect(&mousePoint, &closeButton);
            if (dragging) {
                 x = e.motion.x - dragOffsetX;
                y = e.motion.y - dragOffsetY;
            }
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
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
                minimize(true);  
            }
        } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            dragging = false;
        }
        for(auto &c : children) {
            if(c->event(app, e))
                return true;
        }
        return false;
    }

    void Window::minimize(bool m) {
        minimized = m;
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
        welcome = dynamic_cast<DimensionContainer *>(dimensions[0].get());
        welcome->init("Welcome", loadTexture(app, "images/wallpaper.bmp"));
        welcome->setActive(true);
        welcome->setVisible(false);
        welcome->objects.push_back(std::make_unique<Window>(app));
        welcome_window = dynamic_cast<Window *>(welcome->objects[0].get());
        welcome_window->create("Welcome", 45, 25, 640, 480);
        welcome_window->children.push_back(std::make_unique<Label>(app));
        welcome_window->show(true);
        welcome_label = dynamic_cast<Label *>(welcome_window->children[0].get());
        welcome_label->create("Hello World!", {0,0,255}, 25, 25);
        welcome_label->loadFont("fonts/arial.ttf", 14);
        dimensions.push_back(std::make_unique<DimensionContainer>(app));
        about = dynamic_cast<DimensionContainer *>(dimensions[1].get());
        about->init("About", loadTexture(app, "images/about.bmp"));
        about->setActive(false);
        about->setVisible(false);
        system_bar->setDimensions(&dimensions);
        setCurrentDimension(0);
        system_bar->activateDimension(0);
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