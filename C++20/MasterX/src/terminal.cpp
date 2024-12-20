#include"terminal.hpp"
#include<sstream>
#include<algorithm>
#include<iostream>
#include<thread>
#include<mutex>
#include"mx_window.hpp"
#include"mx_system_bar.hpp"

#ifdef FOR_WASM
#include"./apps/ats/ats.h"
#endif                                                                            

template<typename T>
T my_max(const T& a, const T& b) {
    return a > b ? a : b;
}

template<typename T>
T my_min(const T& a, const T& b) {
    return a < b ? a : b;
}

namespace mx {

 
    Terminal::Terminal(mxApp  &app) : Window(app) {
        active = true;
        text_color = { 255, 255, 255 };
        font = TTF_OpenFont(getPath("fonts/consolas.ttf").c_str(), 15);
        if(!font) {
            std::cerr << "MasterX System Error: could not load system font.\n";
            exit(EXIT_FAILURE);
        }

        Window::setCanResize(true);
        print("MasterX System - Logged in...\n");
        #ifdef FOR_WASM
        print("type help for instruction list\n");
        #endif
        SDL_Rect rc;
        Window::getRect(rc);
        scroll();  
        auto d = get_current_directory();
        if(d.has_value())
            currentDirectory = getLastDirectory(d.value());
#ifdef _WIN32
        SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
        if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
            print("Stdout pipe creation failed");
            return;
        }
        if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
            print("Stdin pipe creation failed");
            return;
        }
        STARTUPINFOA siStartInfo = {sizeof(STARTUPINFO)};
        siStartInfo.hStdError = hChildStdoutWr;
        siStartInfo.hStdOutput = hChildStdoutWr;
        siStartInfo.hStdInput = hChildStdinRd;
        siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

        std::string command  = "wsl.exe bash";
        
        if (!CreateProcessA(NULL, (LPSTR)command.data(), NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &procInfo)) {
            print("Process creation failed");
            return;
        }

        CloseHandle(hChildStdinRd);
        CloseHandle(hChildStdoutWr);
    bashThread = CreateThread(NULL, 0, bashReaderThread, this, 0, NULL);

 #elif !defined(FOR_WASM)
        if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
            std::cout << "Error creating pipes for bash\n ";
            return;
        }
        bashPID = fork();
        if (bashPID == -1) {
            std::cout << "Failed to fork bash process"; 
        } else if (bashPID == 0) {
            close(pipe_in[1]);  
            close(pipe_out[0]); 
            dup2(pipe_in[0], STDIN_FILENO);
            dup2(pipe_out[1], STDOUT_FILENO);
            dup2(pipe_out[1], STDERR_FILENO);

            execlp("/bin/bash", "bash", NULL);
            exit(1);  
        } else {
            close(pipe_in[0]);
            close(pipe_out[1]);
        }
             bashThread = SDL_CreateThread(bashReaderThread, "bashReaderThread", this);
    #endif
    }

     void Terminal::setWallpaper(SDL_Texture *tex) {
        wallpaper = tex;
     }

    Terminal::~Terminal() {
        active = false;
#ifdef _WIN32
        TerminateProcess(procInfo.hProcess, 0);
        CloseHandle(procInfo.hProcess);
        CloseHandle(procInfo.hThread);
        CloseHandle(hChildStdinWr);
        CloseHandle(hChildStdoutRd);
#elif !defined(FOR_WASM)
        if (bashPID > 0) {
            kill(bashPID, SIGTERM);
            waitpid(bashPID, nullptr, 0);
        }
        close(pipe_in[1]);
        close(pipe_out[0]);
        if (bashThread) {
            SDL_WaitThread(bashThread, nullptr);
        }
#endif
    }

    void Terminal::draw(mxApp &app) {        
        if(!isVisible())
            return;

        Window::draw(app);

        if(isDraw() == false)
            return;

        SDL_Rect rc;
        Window::getRect(rc);
        rc.y += 28;
        SDL_SetRenderDrawBlendMode(app.ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app.ren, 0, 0, 0, 128);
        SDL_RenderCopy(app.ren, wallpaper, nullptr, nullptr);
        SDL_RenderFillRect(app.ren, &rc);
        SDL_SetRenderDrawBlendMode(app.ren, SDL_BLENDMODE_NONE);
        Window::drawMenubar(app);
        int lineHeight = TTF_FontHeight(font);
        int maxWidth = rc.w - 10;
        int y = rc.y + 5;
        {
#ifndef FOR_WASM
            std::lock_guard<std::mutex> lock(outputMutex);
#endif
            int offsetLine = 0;
            if(scrollOffset > maxVisibleLines-static_cast<int>(outputLines.size()) ){
                if (!(scrollOffset >= total_Lines() - maxVisibleLines)) {
                    offsetLine = 1; 
                }
            }

            for (int i = scrollOffset; i < static_cast<int>(outputLines.size()); ++i) {
                if(y + lineHeight + lineHeight > rc.y+rc.h) {
                    break;
                } 
                renderText(app, outputLines[i], rc.x + 5, y);
                y += lineHeight;
            }

            if(atBottom()) {
                if (!outputLines.empty()) {
                    std::string lastLine = outputLines.back(); 
                    int textWidth = 0, textHeight = 0;
                    TTF_SizeText(font, lastLine.c_str(), &textWidth, &textHeight);

                    int cy = y - textHeight;  
                    int cx = rc.x + 5;  

                    if(offsetLine != 1) {
                        renderText(app, lastLine, cx, cy);
                    }         
                    cx -= 5;
                    cy += lineHeight;
                    renderTextWrapped(app, "$ ", inputText, cx, cy, maxWidth);
                } else {
                    std::string lastLine = " ";
                    int textWidth = 0, textHeight = 0;
                    TTF_SizeText(font, lastLine.c_str(), &textWidth, &textHeight);
                    int cy = y - textHeight;  
                    int cx = rc.x + 5;             
                    cx -= 5;
                    cy += lineHeight;
                    renderTextWrapped(app, "$ ", inputText, cx, cy, maxWidth);
                }
            }
        }

        int totalLines = static_cast<int>(outputLines.size());
        std::string prompt;

        {
#ifndef FOR_WASM
            std::lock_guard<std::mutex> dlock(directoryMutex);
#endif
            if(currentDirectory.empty()) {
                prompt = "$ ";
            } else {
                prompt = "[" + currentDirectory + "] $ ";
            }
        }

        int promptWidth;
        TTF_SizeText(font,prompt.c_str(), &promptWidth, nullptr);
        int total = calculateWrappedLinesForText(inputText, rc.w - 20, promptWidth);
        totalLines += total;

        if (totalLines > maxVisibleLines) {
            int offx = rc.x + rc.w;    
            int offy = rc.y;     
            int availableHeight = rc.h - 28;  

            scrollBarHeight = (maxVisibleLines * availableHeight) / totalLines;

            if (scrollBarHeight < 10) {
                scrollBarHeight = 10;
            }

            scrollBarPosY = offy + (scrollOffset * (availableHeight - scrollBarHeight)) / (totalLines - maxVisibleLines);

            if (scrollBarPosY + scrollBarHeight > rc.y + rc.h) {
                scrollBarPosY = rc.y + rc.h - scrollBarHeight;
            }

            SDL_Rect scrollBarRect = {offx - scrollBarWidth, scrollBarPosY, scrollBarWidth, scrollBarHeight};
            SDL_SetRenderDrawColor(app.ren, 100, 100, 100, 255);
            SDL_RenderFillRect(app.ren, &scrollBarRect);
        }
    }


    void Terminal::renderText(mxApp &app, const std::string &text, int x, int y) {
        if(!text.empty()) {

            SDL_Rect rc;
            Window::getRect(rc);
            if(!atBottom() && y >= rc.y+(rc.h-28)) 
                return;

            SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), text_color);
            if(surface == nullptr) {
                std::cerr << "MasterX System Error: Render Text failed.\n";
                exit(EXIT_FAILURE);
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(app.ren, surface);
            if(texture == nullptr) {
                std::cerr << "MasterX System Error: Create Texture failed.\n";
                exit(EXIT_FAILURE);
            }
            SDL_Rect dstRect = {x, y, surface->w, surface->h};
            SDL_RenderCopy(app.ren, texture, nullptr, &dstRect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
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
    
    void Terminal::drawCursor(mxApp &app, int x, int y, bool showCursor) {
        if (showCursor) {
            int textHeight = TTF_FontHeight(font);
            SDL_SetRenderDrawColor(app.ren, text_color.r, text_color.g, text_color.b, 255);
            SDL_RenderDrawLine(app.ren, x, y, x, y + textHeight);  
        }
    }

    void Terminal::requestCurrentDirectory() {
        #ifdef _WIN32
            std::string cmd = "echo BEGIN_PWD && pwd && echo END_PWD\n";
            DWORD written;
            WriteFile(hChildStdinWr, cmd.c_str(), cmd.length(), &written, NULL);
        #elif !defined(FOR_WASM)
            std::string cmd = "echo BEGIN_PWD && pwd && echo END_PWD\n";
            write(pipe_in[1], cmd.c_str(), cmd.size());
        #endif
    }

    bool Terminal::atBottom() {
        int totalLines = total_Lines();
        return scrollOffset >= totalLines - maxVisibleLines;
    }
            
    void Terminal::renderTextWrapped(mxApp &app, const std::string &prompt, const std::string &inputText, int &x, int &y, int maxWidth) {
        SDL_Rect rc;
        Window::getRect(rc);

        int margin = 5;
        int availableWidth = maxWidth - margin * 2;
        x = rc.x + margin;

        int promptWidth;
        std::string nprompt;
        {      
#ifndef FOR_WASM
        std::lock_guard<std::mutex> dlock(directoryMutex);
#endif
            if (currentDirectory.empty()) {
                nprompt = "$ ";
            } else {
                nprompt = "[" + currentDirectory + "] $ ";
            }
        }
        TTF_SizeText(font, nprompt.c_str(), &promptWidth, nullptr);
        renderText(app, nprompt, x, y);
        x += promptWidth;
        availableWidth -= promptWidth;

        std::string remainingText = inputText;
        int lineHeight = TTF_FontHeight(font);
        int cursorX = x;
        int cursorY = y;

        while (!remainingText.empty()) {
            std::string lineToRender;
            int currentWidth = 0;
            size_t i = 0;
            int thisLineWidth = availableWidth;

            while (i < remainingText.length()) {
                std::string testLine = lineToRender + remainingText[i];
                TTF_SizeText(font, testLine.c_str(), &currentWidth, nullptr);

                if (currentWidth > thisLineWidth) {
                    break;
                }

                lineToRender += remainingText[i++];
            }

            renderText(app, lineToRender, x, y);
            cursorX = x + currentWidth;
            cursorY = y;

            y += lineHeight;
            x = rc.x + margin;
            thisLineWidth = maxWidth - margin * 2;

            remainingText = remainingText.substr(i);
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - cursorTimer >= cursorBlinkInterval) {
            showCursor = !showCursor;
            cursorTimer = currentTime;
        }

        drawCursor(app, cursorX, cursorY, showCursor);
    }

    bool Terminal::event(mxApp &app, SDL_Event &e) {
        
        if(!Window::isVisible())
            return false;

        if (e.type == SDL_TEXTINPUT) {
            inputText += e.text.text;
            scroll();
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
                   if(!inputText.empty()) {
                        processCommand(app, inputText);
                        inputText.clear(); 
                        scroll();
                   }
                    break;
            }
            return true;
        }
        SDL_Rect rc;
        Window::getRect(rc);
        rc.y += 28;
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            if (mouseX >= rc.x+rc.w - scrollBarWidth && mouseY >= scrollBarPosY && mouseY <= scrollBarPosY + scrollBarHeight) {
                isScrolling = true;
                scrollBarDragOffset = mouseY - scrollBarPosY;
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            isScrolling = false;
        }

        int totalLines = total_Lines();

        if (e.type == SDL_MOUSEMOTION && isScrolling) {
            int mouseY = e.motion.y;
            int newScrollPosY = mouseY - scrollBarDragOffset;
            scrollOffset = (newScrollPosY * totalLines - maxVisibleLines) / (rc.y+rc.h- scrollBarHeight);
            scrollOffset = my_max(0, my_min(scrollOffset, (totalLines - maxVisibleLines)));
            render_text = false;
            if(atBottom())
                scroll();
        }

        if (e.type == SDL_MOUSEWHEEL) {
            handleScrolling(e.wheel.y);
            render_text = false;
            return true;
        }
        
        if(Window::event(app, e))
            return true;

        return false; 
    }

    void Terminal::handleScrolling(int direction) {
        scrollOffset -= direction;
        int totalLines = total_Lines();
         scrollOffset = my_max(0, my_min(scrollOffset, (totalLines - maxVisibleLines)));

        if(atBottom()) 
            scroll();
    }

    void Terminal::processCommand(mxApp &app, std::string command) {
        if(command.empty()) return;
#ifdef FOR_WASM        
        bool clear = false;
#endif
        {
            std::lock_guard<std::mutex> lock(directoryMutex);
            if(currentDirectory.empty())
                print("\n$ " + command + "\n");
            else
                print("\n[" + currentDirectory + "] $ " + command + "\n");
        }

        std::vector<std::string> words;
        words = splitText(command);

        if(words.size()==0)
            return;



        if(command == "exit") {
            app.shutdown();
        } else if (words.size()==2 && words[0] == "setfull" && words[1] == "true") {
            app.set_fullscreen(app.win, true);
            print("MasterX System: full screen is true\n");
            command.clear();
        } else if (words.size()==2 && words[0] == "setfull" && words[1] == "false") {
           app.set_fullscreen(app.win, false);
           print("MasterX System: full screen is false\n");
           command.clear();
        } else if(words.size()==4 && words[0] == "setcolor") {
            text_color.r = atoi(words[1].c_str());
            text_color.g = atoi(words[2].c_str());
            text_color.b = atoi(words[3].c_str());
            text_color.a = 255;
            print("MasterX System: - set text color\n");
            command.clear();
        } else if(words.size() == 1 && words[0] == "about") {
            print("MasterX System written by Jared Bruni\n(C) 2024 LostSideDead Software.\nhttps://lostsidedead.biz\n");
            command.clear();
        } else if(words.size() == 1  && words[0] == "clear") {
            orig_text = "";    
#ifdef FOR_WASM
        clear = true;
#endif
#ifndef _WIN32
            print("");
#else
             command.clear();
#endif
        }
        
#ifdef _WIN32
    std::lock_guard<std::mutex> lock(outputMutex);
    std::string cmd = command + "\n";



    DWORD written;
    if (hChildStdinWr == INVALID_HANDLE_VALUE) {
        std::cerr << "MasterX System: Invalid handle for stdin.\n";
    }

    std::cout << "MasterX: commad [ "  << command << " ]\n";

    WriteFile(hChildStdinWr, cmd.c_str(), cmd.length(), &written, NULL);
    if(written == 0) {
        std::cerr << "MasterX System: Error wrote zero bytes..\n";
    } 
    requestCurrentDirectory();
#elif !defined(FOR_WASM) 
    std::string cmd = command + "\n";
    if(command != "clear")
        write(pipe_in[1], cmd.c_str(), cmd.size());

    requestCurrentDirectory();
#elif defined(FOR_WASM)
    if(command.length()>0 && clear == false) {
        print(scanATS(command));
    }
#endif
        
        scroll();
    }

    bool isAscii(char c) {
        if(c == ' ')    
            return true;
        if(c == '\t')
            return false;
        return isprint(static_cast<unsigned char>(c)) && c >= 32 && c <= 126;
    }

    std::string trimR(const std::string &s) {
        std::string temp;
        temp.reserve(s.length());
        for(char c : s) {
            if(c == '\t') {
                temp += "    ";
            } else if(isAscii(c))
                temp += c;
        }
        return temp;
    }

    void Terminal::updateText(const std::string &text) {
        if(!text.empty()) 
            orig_text += text;
        
    }

    void Terminal::print(const std::string &s) {
#ifndef FOR_WASM
        std::lock_guard<std::mutex> lock(outputMutex);
#endif
        updateText(s);
        std::string line;
        SDL_Rect rc;
        Window::getRect(rc);
        int maxWidth = rc.w - 10;  
        int w, h;
        outputLines.clear();
	    std::string total = orig_text;
        std::istringstream stream(total);
        while(std::getline(stream, line)) {
            if (line.length() > 0) {
                std::string currentLine;
                for (size_t i = 0; i < line.length(); ++i) {
                    currentLine += line[i];
                    TTF_SizeText(font, currentLine.c_str(), &w, &h);                    
                    if (w > maxWidth) {
                    
                        size_t lastSpace = currentLine.find_last_of(' ');
                        if (lastSpace != std::string::npos) {
                    
                            std::string part = currentLine.substr(0, lastSpace);
                            if(!part.empty())
                            outputLines.push_back(trimR(part));
                            currentLine = currentLine.substr(lastSpace + 1);  
                        } else {
                    
                            if(!currentLine.empty())
                            outputLines.push_back(trimR(currentLine));
                            currentLine.clear();
                        }
                    }
                }
                if (!currentLine.empty()) {
                    outputLines.push_back(trimR(currentLine));
                }
                scroll();  
            }
        
        }
        scroll();
    }

    int Terminal::calculateWrappedLinesForText(const std::string &text, int maxWidth, int promptWidth) {
        int lineCount = 0;  
        std::string lineToRender;
        int currentWidth = 0;
        bool isFirstLine = true;
        for (size_t i = 0; i < text.length(); ++i) {
            lineToRender += text[i];
            TTF_SizeText(font, lineToRender.c_str(), &currentWidth, nullptr);
            int currentMaxWidth = isFirstLine ? maxWidth - promptWidth : maxWidth;
            if (currentWidth > currentMaxWidth) {
                lineCount++;  
                lineToRender.clear();  
                lineToRender += text[i]; 
                TTF_SizeText(font, lineToRender.c_str(), &currentWidth, nullptr);
                isFirstLine = false; 
            }
        }
        if (!lineToRender.empty()) {
            lineCount++;

        }
        if(inputText.empty())
            lineCount++;


        return lineCount;
    }

    int Terminal::calculateTotalWrappedLines() {
        int totalWrappedLines = 0;
        SDL_Rect rc;
        Window::getRect(rc);
        int maxWidth = rc.w - 10;

        for (const std::string &line : outputLines) {
            int w, h;
            TTF_SizeText(font, line.c_str(), &w, &h);

            
            if (w <= maxWidth) {
                totalWrappedLines++;
            } else {
            
                int wrappedLines = (w + maxWidth - 1) / maxWidth;  
                totalWrappedLines += wrappedLines;
            }
        }

        return totalWrappedLines;
    }

    int Terminal::total_Lines() {
        int totalLines = static_cast<int>(outputLines.size());
        SDL_Rect rc;
        Window::getRect(rc);

        std::string prompt;
        if(currentDirectory.empty()) {
            prompt = "$ ";
        } else {
            prompt = "[" + currentDirectory + "] $ ";
        }
        int promptWidth;
        TTF_SizeText(font,prompt.c_str(), &promptWidth, nullptr);
        int total = calculateWrappedLinesForText(inputText, rc.w - 20, promptWidth);
        total += totalLines;
        return total;
    }

    void Terminal::scroll() {
        int totalLines = total_Lines();  
        SDL_Rect rc;
        Window::getRect(rc);
        int lineHeight = TTF_FontHeight(font);
        maxVisibleLines = (rc.h - 28) / lineHeight;
        if (totalLines > maxVisibleLines) {
            if (scrollOffset < totalLines - maxVisibleLines) {
                scrollOffset = my_max(0, totalLines - maxVisibleLines);
            }
        } else {
            scrollOffset = 0;
        }

        if(!atBottom()) {
            maxVisibleLines -= 1;
        }
    }


    void Terminal::stateChanged(bool min, bool max, bool closed) {
        isMaximized = max;
        print("");
        Window::dragging = false;
    }

    std::string getLastDirectory(const std::string& fullPath) {
        std::size_t pos = fullPath.find_last_of("\\/");
        if (pos != std::string::npos) {
            return trimR(fullPath.substr(pos + 1));  
        }
        return trimR(fullPath);  
    }

#ifdef _WIN32
    DWORD WINAPI Terminal::bashReaderThread(LPVOID param) {
        Terminal* terminal = static_cast<Terminal*>(param);
        char buffer[1024];
        std::string output;
        std::string pwdOutput;
        bool inPwdBlock = false;
        DWORD bytesRead;

        while (terminal->active) {
            while (ReadFile(terminal->hChildStdoutRd, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                output.append(buffer);

                while (!output.empty()) {
                    if (!inPwdBlock) {
                        std::size_t beginPos = output.find("BEGIN_PWD");
                        if (beginPos != std::string::npos) {
                            terminal->print(output.substr(0, beginPos));
                            output = output.substr(beginPos + std::string("BEGIN_PWD").length());
                            inPwdBlock = true;
                            pwdOutput.clear();
                        } else {
                            terminal->print(output);
                            output.clear();
                        }
                    } else {
                        std::size_t endPos = output.find("END_PWD");
                        if (endPos != std::string::npos) {
                            pwdOutput += output.substr(0, endPos);
                            {
                                std::lock_guard<std::mutex> lock(terminal->directoryMutex);
                                terminal->currentDirectory = getLastDirectory(pwdOutput);
                                terminal->completePath = trimR(pwdOutput);
                            }
                            output = output.substr(endPos + std::string("END_PWD").length());
                            inPwdBlock = false;
                        } else {
                            pwdOutput += output;
                            output.clear();
                        }
                    }
                }
            }

            if (bytesRead == 0 || GetLastError() != ERROR_MORE_DATA) {
                break;
            }

            Sleep(10);
        }
        return 0;
    }
#elif !defined(FOR_WASM)
    int Terminal::bashReaderThread(void *ptr) {
        Terminal *terminal = static_cast<Terminal *>(ptr);
        char buffer[1024];
        std::string output;
        std::string pwdOutput;
        bool inPwdBlock = false; // Tracks if we're in a PWD block

        while (terminal->active) {
            ssize_t count;

            
            while ((count = read(terminal->pipe_out[0], buffer, sizeof(buffer) - 1)) > 0) {
                buffer[count] = '\0';  
                output.append(buffer); 

                
                while (!output.empty()) {
                    if (!inPwdBlock) {
                
                        std::size_t beginPos = output.find("BEGIN_PWD");
                        if (beginPos != std::string::npos) {
                            terminal->print(output.substr(0, beginPos));  
                            output = output.substr(beginPos + std::string("BEGIN_PWD").length()); 
                            inPwdBlock = true;
                            pwdOutput.clear(); 
                        } else {
                            terminal->print(output);
                            output.clear();
                        }
                    } else {
                        std::size_t endPos = output.find("END_PWD");
                        if (endPos != std::string::npos) {
                            pwdOutput += output.substr(0, endPos);
                            {
                                std::lock_guard<std::mutex> lock(terminal->directoryMutex);
                                terminal->currentDirectory = getLastDirectory(pwdOutput);
                                terminal->completePath = trimR(pwdOutput);
                            }
                            output = output.substr(endPos + std::string("END_PWD").length());
                            inPwdBlock = false;
                        } else {
                            pwdOutput += output;
                            output.clear();
                        }
                    }
                }
            }

            if (count == -1 && errno != EAGAIN) {
                std::cerr << "Error reading from pipe: " << strerror(errno) << std::endl;
                break;
            }

            usleep(10000); 
        }

        return 0;
    }
#endif
}
 