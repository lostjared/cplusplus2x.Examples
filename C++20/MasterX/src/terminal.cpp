#include"terminal.hpp"
#include<sstream>
#include<algorithm>
  
namespace mx {

    Terminal::Terminal(mxApp  &app) : Window(app) {}

    void Terminal::draw(mxApp &app) {
        Window::draw(app);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 255);
        SDL_Rect rc;
        Window::getRect(rc);
        rc.x += 5;  
        rc.y += 28; 
        rc.w -= 10; 
        rc.h -= 33; 
        SDL_RenderFillRect(app.ren, &rc);


        int lineHeight = TTF_FontHeight(app.font);
        maxVisibleLines = ((rc.h -1) / lineHeight)-1;  // Calculate maxVisibleLines once here


        if (scrollOffset > static_cast<int>(outputLines.size()) - maxVisibleLines) {
            scrollOffset = std::max(0, static_cast<int>(outputLines.size()) - maxVisibleLines);
        }
        int y = rc.y + 4;
        int maxWidth = rc.w - 10;

        for (int i = scrollOffset; i < static_cast<int>(outputLines.size()) && i < scrollOffset + maxVisibleLines; ++i) {
            renderTextWrapped(app, outputLines[i], rc.x + 5, y, maxWidth);
        }
        renderTextWrapped(app, "=)> " + inputText, rc.x + 5, y, maxWidth);
    }

    void Terminal::renderText(mxApp &app, const std::string &text, int x, int y) {
        SDL_Color color = {255, 255, 255}; 
        SDL_Surface* surface = TTF_RenderText_Solid(app.font, text.c_str(), color);
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

        if(Window::event(app, e))
            return true;

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

        return false; 
    }

    void Terminal::processCommand(mxApp &app, const std::string &command) {
        outputLines.push_back("=)> " + command);

        if (command == "clear") {
            outputLines.clear();
            scrollOffset = 0; 
        } else if(command == "about") {
            outputLines.push_back("MasterX written by Jared Bruni");
            outputLines.push_back("(C) 2024 LostSideDead Software");
            outputLines.push_back("site: lostsidedead.biz");
        } else {
            int totalLines = static_cast<int>(outputLines.size());
            if (totalLines > maxVisibleLines) {
                scrollOffset = totalLines - maxVisibleLines;
            }
        }
    }
}
 