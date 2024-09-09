#ifndef _DIMENSION_H
#define _DIMENSION_H

#include "window.hpp"
#include<memory>
#include<vector>
#include<string>
#include<iostream>
#include<functional>

namespace mx {

    extern bool cursor_shown;

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

    class Window;
    using EventCallback = bool (*)(mxApp &app, Window *window, SDL_Event &e);
    using ResizeCallback = std::function<void(Window*, int, int)>;

    class Control : public Screen {
    public:
        virtual ~Control() = default;
        virtual void setWindowPos(int x, int y) = 0;
        virtual void resizeWindow(int w, int h) {}

        template<typename F>
        void setCallback(F callb) { callback = callb; }  
        template<typename F>
        void setResizeCallback(F callb) { rcallback = callb; }
        
        Window *parent = nullptr;
        EventCallback callback = nullptr;
        ResizeCallback rcallback = nullptr;
    };

    class Window : public Screen {
    public:
        Window(mxApp &app);
        virtual ~Window();
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        virtual void stateChanged(bool min, bool max, bool close) {}
        void create(const std::string &name, const int x, const int y, const int w, const int h);
        void show(bool b);
        void minimize(bool m);
        void maximize(bool m);
        void getRect(SDL_Rect &rc);
        void setRect(const SDL_Rect &rc);
        Control *getControl();
        Control *getControl(int index);
        bool isVisible() const;
        bool reload() const;
        void setReload(bool r);
        void setCanResize(bool r);
        bool canResize() const;
        
    private:
        int x,y,w,h;
        int dim_w = 0, dim_h = 0;
        std::string title = "Window";
        bool shown = false;
        bool minimized = false;
        bool maximized = false;
        bool dragging = false;
        int dragOffsetX = 0, dragOffsetY = 0;
        int oldX = 0, oldY = 0, oldW = 0, oldH = 0;
        bool is_visible = true;
        bool reload_window = false;
        bool can_resize = false;
    public:
        std::vector<std::unique_ptr<Control>> children;
        SDL_Rect minimizeButton, closeButton, maximizeButton;
        SDL_bool minimizeHovered, closeHovered, maximizeHovered;
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
    class Label;
    class Button;
    class Image;

    class Dimension : public Screen {
    public:
        Dimension(mxApp &app);
        virtual ~Dimension();
        void setCurrentDimension(int dim);
        int  getCurrentDimension() const;
        virtual void draw(mxApp &app) override;
        virtual bool event(mxApp &app, SDL_Event &e) override;
        void drawDash(mxApp &app);
        Screen *getDimension();
        Screen *getDimension(int index);
    private:
        SDL_Texture *wallpaper;
        std::vector<std::unique_ptr<Screen>> objects;
        std::vector<std::unique_ptr<Screen>> dimensions;
        DimensionContainer *welcome, *dash;
        DimensionContainer *about;
        DimensionContainer *term;
        SystemBar *system_bar;
        Window *welcome_window;
        Window *about_window;
        Button *about_window_ok, *welcome_ok;
        Image *welcome_image;
        Label *about_window_info;
        Image *about_window_logo;
        int current_dim;
        SDL_Texture *hand_cursor, *reg_cursor;
        int cursor_x = 0, cursor_y = 0;
        Terminal *termx;
    };

}

#endif