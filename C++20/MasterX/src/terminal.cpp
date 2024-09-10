#include"terminal.hpp"
#include<sstream>
#include<algorithm>
#include<iostream>

namespace mx {

    Terminal::Terminal(mxApp  &app) : Window(app) {
        print("MasterX System - Logged in...");
        text_color = { 255, 255, 255 };
        font = app.font;
        Window::setCanResize(true);
    }

     void Terminal::setWallpaper(SDL_Texture *tex) {
        wallpaper = tex;
     }

    void Terminal::draw(mxApp &app) {        

        if(!isVisible())
            return;

        Window::draw(app);

        if(isDraw() == false)
            return;

        SDL_Rect rc;
        Window::getRect(rc);
        rc.x += 5;  
        rc.y += 28; 
        rc.w -= 10; 
        rc.h -= 33; 

        SDL_SetRenderDrawBlendMode(app.ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 128);
        SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
        SDL_RenderFillRect(app.ren, &rc);
        SDL_SetRenderDrawBlendMode(app.ren, SDL_BLENDMODE_NONE);
        Window::drawMenubar(app);
        
        int lineHeight = TTF_FontHeight(app.font);
        maxVisibleLines = ((rc.h - 1) / lineHeight) - 1;
        if (scrollOffset > static_cast<int>(outputLines.size()) - maxVisibleLines) {
            scrollOffset = std::max(0, static_cast<int>(outputLines.size()) - maxVisibleLines);
        }
        int y = rc.y + 4;
        int maxWidth = rc.w - 10;

        for (int i = scrollOffset; i < static_cast<int>(outputLines.size()) && i < scrollOffset + maxVisibleLines; ++i) {
            renderTextWrapped(app, outputLines[i], rc.x + 5, y, maxWidth);
        }
        renderTextWrapped(app, "$ " + inputText, rc.x + 5, y, maxWidth);

        
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - cursorTimer >= cursorBlinkInterval) {
            showCursor = !showCursor;
            cursorTimer = currentTime;
        }

        
        if (showCursor) {
            int textWidth, textHeight;
            TTF_SizeText(app.font, ("$ " + inputText).c_str(), &textWidth, &textHeight);
            int cursorY = y - lineHeight;  
           SDL_SetRenderDrawColor(app.ren, text_color.r, text_color.g, text_color.b, 255);  
           SDL_RenderDrawLine(app.ren, rc.x + 5 + textWidth, cursorY + textHeight - 2, rc.x + 5 + textWidth + 10, cursorY + textHeight - 2);
        }
    }

    void Terminal::renderText(mxApp &app, const std::string &text, int x, int y) {
        SDL_Surface* surface = TTF_RenderText_Solid(app.font, text.c_str(), text_color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app.ren, surface);

        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(app.ren, texture, nullptr, &dstRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    std::vector<std::string> Terminal::splitText(const std::string &text) {
        std::vector<std::string> words;
        std::istringstream ss(text);
        std::string word;

        while (ss >> word) {
            words.push_back(word);
        }

        return words;
    }

    void Terminal::renderTextWrapped(mxApp &app, const std::string &text, int x, int &y, int maxWidth) {
        std::vector<std::string> words = splitText(text);
        std::string currentLine;

        int lineHeight = TTF_FontHeight(app.font);

        for (const auto &word : words) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;

            int w, h;
            TTF_SizeText(app.font, testLine.c_str(), &w, &h);

            if (w > maxWidth) {
                if (!currentLine.empty()) {
                    renderText(app, currentLine, x, y);
                    y += lineHeight;
                    currentLine.clear();
                }

                std::string remainingWord = word;
                while (!remainingWord.empty()) {
                    std::string part;
                    int partW;

                    for (size_t i = 0; i < remainingWord.length(); ++i) {
                        std::string tmp = part + remainingWord[i];
                        TTF_SizeText(app.font, tmp.c_str(), &partW, &h);
                        if (partW > maxWidth) {
                            break; 
                        }
                        part = tmp;
                    }

                    renderText(app, part, x, y);
                    y += lineHeight;

                    remainingWord = remainingWord.substr(part.length());
                }
            } else {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty()) {
            renderText(app, currentLine, x, y);
            y += lineHeight;
        }
    } 
    void Terminal::handleScrolling(int direction) {
        scrollOffset -= direction;
        
        if (scrollOffset < 0) {
            scrollOffset = 0;
        }

        int totalLines = static_cast<int>(outputLines.size());
        if (scrollOffset > totalLines - maxVisibleLines) {
            scrollOffset = std::max(0, totalLines - maxVisibleLines);
        }
    }

    bool Terminal::event(mxApp &app, SDL_Event &e) {
        
        if(!Window::isVisible())
            return false;

        if (e.type == SDL_TEXTINPUT) {
            inputText += e.text.text;
            return true;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_BACKSPACE:
                    if (!inputText.empty()) {
                        inputText.pop_back(); 
                    }
                    break;

                case SDLK_RETURN:
                    processCommand(app, inputText);
                    inputText.clear(); 
                    break;
            }
            return true;
        }

        if (e.type == SDL_MOUSEWHEEL) {
            handleScrolling(e.wheel.y);
            return true;
        }
        
        if(Window::event(app, e))
            return true;

        return false; 
    }

    void Terminal::processCommand(mxApp &app, const std::string &command) {
        outputLines.push_back("$ " + command);
        std::vector<std::string> words;
        words = splitText(command);

        if (command == "clear") {
            outputLines.clear();
            scrollOffset = 0; 
        } else if(words.size()>0 && words[0] == "settextcolor") {
            if(words.size()==4) {
                int r = atoi(words[1].c_str());
                int g = atoi(words[2].c_str());
                int b = atoi(words[3].c_str());
                text_color.r = r;
                text_color.g = g;
                text_color.b = b;
                text_color.a = 255;
                print("MasterX System: Terminal color set.");
            }
        } else if(command == "about") {
            print("MasterX System - written by Jared Bruni\n(c) 2024 LostSideDead Software\nsite: lostsidedead.biz");
        } 
        scroll();
    }

    void Terminal::print(const std::string &s) {
        std::istringstream stream(s);
        std::string line;
        while(std::getline(stream, line)) {
            outputLines.push_back(line);
        }
    }

    void Terminal::scroll() {
        int totalLines = static_cast<int>(outputLines.size());
        SDL_Rect rc;
        Window::getRect(rc);
        rc.h -= 33;
        int lineHeight = TTF_FontHeight(font);
        maxVisibleLines = ((rc.h - 1) / lineHeight) - 1;
        if (totalLines > maxVisibleLines) {
            scrollOffset = totalLines - maxVisibleLines;
        } else {
                scrollOffset = 0;  
        }
    }

    void Terminal::stateChanged(bool min, bool max, bool closed) {
        isMaximized = max;
        scroll();

    }
}
 