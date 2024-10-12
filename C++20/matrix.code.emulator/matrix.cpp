/* 
    Matrix Rain C++20 Demo
    written by Jared Bruni
    (C) 2024 
*/
#include<iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <unordered_map>
#include "SDL.h"
#include "SDL_ttf.h"
#include "argz.hpp"

namespace mx {
    std::unordered_map<std::string, SDL_Texture*> char_textures;
    SDL_Color bright_green = {180, 255, 180, 255};
    SDL_Color dim_green = {0, 255, 0, 150};

    void releaseMatrix() {
        for(auto &i : char_textures) {
            if(i.second != nullptr) {
                SDL_DestroyTexture(i.second);
            }
        }
    }

    std::string unicodeToUTF8(int codepoint) {
        std::string utf8;
        if (codepoint <= 0x7F) {
            utf8 += static_cast<char>(codepoint);
        } else if (codepoint <= 0x7FF) {
            utf8 += static_cast<char>((codepoint >> 6) | 0xC0);
            utf8 += static_cast<char>((codepoint & 0x3F) | 0x80);
        } else if (codepoint <= 0xFFFF) {
            utf8 += static_cast<char>((codepoint >> 12) | 0xE0);
            utf8 += static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
            utf8 += static_cast<char>((codepoint & 0x3F) | 0x80);
        } else if (codepoint <= 0x10FFFF) {
            utf8 += static_cast<char>((codepoint >> 18) | 0xF0);
            utf8 += static_cast<char>(((codepoint >> 12) & 0x3F) | 0x80);
            utf8 += static_cast<char>(((codepoint >> 6) & 0x3F) | 0x80);
            utf8 += static_cast<char>((codepoint & 0x3F) | 0x80);
        }
        return utf8;
    }

    std::vector<std::pair<int, int>> codepoint_ranges = {
        {0x3041, 0x3096},   
        {0x30A0, 0x30FF}
    };

    int getRandomCodepoint() {
        int range_index = rand() % codepoint_ranges.size();
        int start = codepoint_ranges[range_index].first;
        int end = codepoint_ranges[range_index].second;
        return start + rand() % (end - start + 1);
    }

    SDL_Color computeTrailColor(int trail_offset, int trail_length) {
        float intensity = 1.0f - (float)trail_offset / (float)trail_length;
        if (intensity < 0.0f) intensity = 0.0f;

        Uint8 alpha = static_cast<Uint8>(255 * intensity);
        if (alpha < 50) alpha = 50;

        Uint8 green = static_cast<Uint8>(255 * intensity);
        if (green < 100) green = 100; 
        SDL_Color color = {0, green, 0, alpha};
        return color;
    }

    void createMatrixRain(SDL_Renderer* renderer, TTF_Font* font, int screen_width, int screen_height) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);  
        SDL_Rect screenRect = {0, 0, screen_width, screen_height};
        SDL_RenderFillRect(renderer, &screenRect);

        int char_width = 0;
        int char_height = 0;
        TTF_SizeUTF8(font, "A", &char_width, &char_height); 
        int num_columns = screen_width / char_width + 1;
        int num_rows = screen_height / char_height + 1;

        static std::vector<float> fall_positions(num_columns, 0.0f);
        static std::vector<int> fall_speeds(num_columns, 0);
        static std::vector<int> trail_lengths(num_columns, 0);
        static Uint32 last_time = 0;
        float speed_multiplier = 2.0f;

        if (fall_positions[0] == 0.0f) {
            for (int col = 0; col < num_columns; ++col) {
                fall_positions[col] = static_cast<float>(rand() % num_rows);
                fall_speeds[col] = (rand() % 7 + 3) * speed_multiplier;
                trail_lengths[col] = rand() % 15 + 5; 
            }
            last_time = SDL_GetTicks();
        }

        Uint32 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        for (int col = 0; col < num_columns; ++col) {
            fall_positions[col] += fall_speeds[col] * delta_time;
            if (fall_positions[col] >= num_rows) {
                fall_positions[col] -= num_rows;
                
                trail_lengths[col] = rand() % 15 + 5;
                fall_speeds[col] = (rand() % 7 + 3) * speed_multiplier;
            }

            for (int trail_offset = 0; trail_offset < trail_lengths[col]; ++trail_offset) {
                int row = static_cast<int>(fall_positions[col] - trail_offset + num_rows) % num_rows;

                int random_char_code = getRandomCodepoint();
                std::string random_char = unicodeToUTF8(random_char_code);

                SDL_Texture* char_texture = nullptr;
                if (char_textures.find(random_char) == char_textures.end()) {
                    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, random_char.c_str(), {255, 255, 255, 255});
                    char_texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_FreeSurface(surface);
                    char_textures[random_char] = char_texture;
                } else {
                    char_texture = char_textures[random_char];
                }

                SDL_Color color = computeTrailColor(trail_offset, trail_lengths[col]);
                SDL_SetTextureColorMod(char_texture, color.r, color.g, color.b);
                SDL_SetTextureAlphaMod(char_texture, color.a);

                SDL_Rect dst_rect = {col * char_width, row * char_height, char_width, char_height};
                SDL_RenderCopy(renderer, char_texture, nullptr, &dst_rect);
            }
        }
    }
}


int main(int argc, char **argv) {
    Argz<std::string> argz(argc, argv);
    argz.addOptionSingleValue('i', "input font")
    .addOptionSingleValue('s', "font size")
    .addOptionSingleValue('r', "window resolution")
    .addOptionSingle('h', "help message")
    .addOptionSingle('f', "Full screen");

    std::string font_name = "./keifont.ttf";
    int font_size = 24;
    int window_width = 1280, window_height = 720;
    Argument<std::string> arg;
    int value = 0;
    bool fullscreen = false;

    if(argc == 1) {
        std::cout << "Matrix v1.0\nhttps://lostsidedead.biz\nCoded by Jared Bruni\n";
        argz.help(std::cout);
        std::cout << "Press Escape to Quit..\n";
        std::cout.flush();
    }

    try {
        while((value = argz.proc(arg)) != -1) {
            switch(value) {
                case 'h':
                    argz.help(std::cout);
                    exit(EXIT_SUCCESS);
                    break;
                case 's':
                    font_size = atoi(arg.arg_value.c_str());
                    break;
                case 'i':
                    font_name = arg.arg_value;
                    break;
                case 'r': {
                    auto pos = arg.arg_value.find("x");
                    if(pos == std::string::npos)  {
                        std::cerr << "argument error: use format WitthxHeight\n";
                        std::cerr.flush();
                        exit(EXIT_FAILURE);
                    }
                    std::string left = arg.arg_value.substr(0, pos);
                    std::string right = arg.arg_value.substr(pos + 1);
                    window_width = atoi(left.c_str());
                    window_height = atoi(right.c_str());
                }
                    break;
                case 'f':
                    fullscreen = true;
                    break;
            }
        }
    } catch(const ArgException<std::string> &e) {
        std::cerr << "Syntax Error: " << e.text() << "\n";
        exit(EXIT_FAILURE);
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }
    if(TTF_Init() < 0) {
        std::cerr << "Error init TTF.\n";
        SDL_Quit();
        return -1;
    }
    TTF_Font *font = TTF_OpenFont(font_name.c_str(), font_size);
    if(!font) {
        std::cerr << "Couldn't open the font...\n";
        SDL_Quit();
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("-[ Matrix Code Emulator ]-",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          window_width, window_height, fullscreen ? SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN : SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    bool quit = false;
    SDL_Event e;
     srand(static_cast<unsigned int>(time(0)));
     while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        mx::createMatrixRain(renderer,font, window_width, window_height);
        SDL_RenderPresent(renderer);
    }
    mx::releaseMatrix();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
