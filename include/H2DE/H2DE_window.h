#ifndef H2DE_WINDOW_H
#define H2DE_WINDOW_H

#include "H2DE_engine.h"
class H2DE_Engine;

class H2DE_Window {
private:
    H2DE_Engine* engine;
    SDL_Window* window;
    SDL_Renderer* renderer;
    H2DE_WindowData data;

public:
    H2DE_Window(H2DE_Engine* engine, H2DE_WindowData data);
    ~H2DE_Window();

    friend SDL_Renderer* H2DE_GetWindowsRenderer(H2DE_Window* window);
    friend H2DE_AbsSize H2DE_GetWindowSize(H2DE_Window* window);
    friend H2DE_AbsSize H2DE_GetWindowMinimumSize(H2DE_Window* window);
    friend H2DE_AbsSize H2DE_GetWindowMaximumSize(H2DE_Window* window);
    
    friend void H2DE_SetWindowTitle(H2DE_Window* window, std::string title);
    friend void H2DE_SetWindowSize(H2DE_Window* window, H2DE_AbsSize size);
    friend void H2DE_SetWindowMinimumSize(H2DE_Window* window, H2DE_AbsSize minSize);
    friend void H2DE_SetWindowMaximumSize(H2DE_Window* window, H2DE_AbsSize maxSize);
    friend void H2DE_SetWindowFullscreen(H2DE_Window* window, bool state);
    friend void H2DE_SetWindowGrab(H2DE_Window* window, bool state);
    friend void H2DE_SetWindowResizable(H2DE_Window* window, bool state);
};

#endif
