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
        int  getCurrentDimension() const;
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

    class Control : public Screen {
    public:
        virtual ~Control() = default;
        virtual void setWindowPos(int x, int y) = 0;  
    };


    class Label : public Control {
    public:
        Label(mxApp &app);
        virtual ~Label();
        virtual void draw(mxApp &app) override;
        virtual void setWindowPos(int xx, int yy) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void create(const std::string &text, SDL_Color col, int x, int y);
        void loadFont(const std::string &name, int size);
        void setText(const std::string  &text, SDL_Color color);
        void setGeometry(int xx, int yy);
        void linkMode(bool m);
    private:
        TTF_Font *font_;
        std::string name_;
        std::string text_;
        int x,y,size_;
        SDL_Color color_;
        int wx, wy;
        int w, h;
        bool mode = false;
        bool under_ = false;
    };

    class Window : public Screen {
    public:
        Window(mxApp &app);
        virtual ~Window();
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void create(const std::string &name, const int x, const int y, const int w, const int h);
        void show(bool b);
        void minimize(bool m);
        void getRect(SDL_Rect &rc);
    private:
        int x,y,w,h;
        std::string title = "Window";
        bool shown = false;
        bool minimized = false;
        bool dragging = false;
        int dragOffsetX = 0, dragOffsetY = 0;
    public:
        std::vector<std::unique_ptr<Control>> children;
        SDL_Rect minimizeButton, closeButton;
        SDL_bool minimizeHovered, closeHovered;
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
        void setVisible(bool v);
        bool hoveringX = false;
        void startTransition(SDL_Texture *);
        void updateTransition();
        SDL_Texture *wallpaper, *nextWallpaper;
    private:
        bool active = false;
        bool visible = false;
        bool transitioning = false;
        int currentDimension = 0;
        int nextDimension = -1;
        int transitionAlpha = 255;
        int transitionSpeed = 3;
    };
    
    class Terminal;

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
        DimensionContainer *term;
        SystemBar *system_bar;
        Window *welcome_window;
        Label *welcome_label;
        int current_dim;
        SDL_Texture *hand_cursor, *reg_cursor;
        int cursor_x = 0, cursor_y = 0;
        Terminal *termx;
    };

}

#endif