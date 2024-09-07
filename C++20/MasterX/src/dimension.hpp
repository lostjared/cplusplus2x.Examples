#ifndef _DIMENSION_H
#define _DIMENSION_H

#include "window.hpp"
#include<memory>
#include<vector>
#include<string>
#include<iostream>
namespace mx {

     class MenuBar : public Screen {
    public:
        MenuBar(mxApp &app);
        virtual ~MenuBar();
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp  &app, SDL_Event &e) override;
        bool menuOpen;
        bool animating;
        int currentY;
        int targetY;
        int itemClicked(mxApp &app, int x, int y);
    private:
        TTF_Font *font;
        SDL_Surface *itemSurface;
        SDL_Texture *itemTexture;
        SDL_Rect itemRect;
        SDL_Color white;
    };

    class SystemBar : public Screen {
    public:
        SystemBar(mxApp &app);
        virtual ~SystemBar();
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void setDimensions(std::vector<std::unique_ptr<Screen>> *dim);
        void setCurrentDimension(int dim);
        int  getCurrentDimesnion() const;
        void activateDimension(int dim);
        void deactivateDimension(int dim);
        bool empty() const;
    
        void performAction(int action);
    private:
        int yPos;
        bool animationComplete;
        TTF_Font  *font;
        SDL_Color start_bg;
        bool isHovering = false;
        std::vector<std::unique_ptr<Screen>> objects;
        std::vector<std::unique_ptr<Screen>> *dimensions;
        MenuBar *menu;
        int cur_dim = 0;
        int prev_dim = 0;

        void drawDimensions(mxApp &app);
        std::vector<int> activeDimensionsStack;
    };

    class Window : public Screen {
    public:
        Window(mxApp &app);
        virtual ~Window();
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void create(const std::string &name, const int x, const int y, const int w, const int h);
    private:
        int x,y,w,h;
        std::string title = "Window";
        bool shown = false;
        std::vector<std::unique_ptr<Screen>> children;
    };

    class DimensionContainer : public Screen {
    public:
        std::string name = "Default";
        DimensionContainer(mxApp &app);
        virtual ~DimensionContainer();
        void init(const std::string &name, SDL_Texture *wallpaperx);
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        std::vector<std::unique_ptr<Screen>> objects;
        void setActive(bool a);
        bool isActive() const;

        bool hoveringX = false;
    private:
        SDL_Texture *wallpaper;
        bool active = false;
    };

    class Dimension : public Screen {
    public:
        Dimension(mxApp &app);
        virtual ~Dimension();
        void setCurrentDimension(int dim);
        int  getCurrentDimension() const;
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void drawDash(mxApp &app);
    private:
        SDL_Texture *wallpaper;
        std::vector<std::unique_ptr<Screen>> objects;
        std::vector<std::unique_ptr<Screen>> dimensions;
        DimensionContainer *welcome;
        DimensionContainer *about;
        SystemBar *system_bar;
        int current_dim;
    };

}

#endif