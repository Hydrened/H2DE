#include "H2DE_window.h"

// INIT
H2DE_Window::H2DE_Window(H2DE_Engine* e, H2DE_WindowData d) : engine(e), data(d) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("H2DE-101: SDL_Init_Video failed: " + std::string(SDL_GetError()));
    }
    else if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error("H2DE-102: SDL_Init_Audio failed: " + std::string(SDL_GetError()));
    }
    else if (!IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG) {
        throw std::runtime_error("H2DE-103: IMG_Init failed: " + std::string(IMG_GetError()));
    }
    else if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
        throw std::runtime_error("H2DE-104: Mix_Init failed: " + std::string(Mix_GetError()));
    }
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
        throw std::runtime_error("H2DE-105: Mix_OpenAudio failed: " + std::string(Mix_GetError()));
    }

    SDL_WindowFlags flag = (data.fullscreen) ? SDL_WINDOW_FULLSCREEN : (data.resizable) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_SHOWN;
    window = SDL_CreateWindow(data.title.c_str(), data.pos.x, data.pos.y, data.size.w, data.size.h, SDL_WINDOW_SHOWN | flag);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("H2DE-106: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("H2DE-107: Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
}

// CLEANUP
H2DE_Window::~H2DE_Window() {
    IMG_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}

// GETTER
SDL_Renderer* H2DE_GetWindowsRenderer(H2DE_Window* window) {
    return window->renderer;
}

H2DE_AbsSize H2DE_GetWindowSize(H2DE_Window* window) {
    int w, h;
    SDL_GetWindowSize(window->window, &w, &h);
    return { w, h };
}

H2DE_AbsSize H2DE_GetWindowMinimumSize(H2DE_Window* window) {
    int w, h;
    SDL_GetWindowMinimumSize(window->window, &w, &h);
    return { w, h };
}

H2DE_AbsSize H2DE_GetWindowMaximumSize(H2DE_Window* window) {
    int w, h;
    SDL_GetWindowMaximumSize(window->window, &w, &h);
    return { w, h };
}

// SETTER
void H2DE_SetWindowTitle(H2DE_Window* window, std::string title) {
    SDL_SetWindowTitle(window->window, title.c_str());
}

void H2DE_SetWindowSize(H2DE_Window* window, H2DE_AbsSize size) {
    SDL_SetWindowSize(window->window, size.w, size.h);
}

void H2DE_SetWindowMinimumSize(H2DE_Window* window, H2DE_AbsSize minSize) {
    SDL_SetWindowMinimumSize(window->window, minSize.w, minSize.h);
}

void H2DE_SetWindowMaximumSize(H2DE_Window* window, H2DE_AbsSize maxSize) {
    SDL_SetWindowMaximumSize(window->window, maxSize.w, maxSize.h);
}

void H2DE_SetWindowFullscreen(H2DE_Window* window, bool state) {
    SDL_SetWindowFullscreen(window->window, (state) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void H2DE_SetWindowGrab(H2DE_Window* window, bool state) {
    SDL_SetWindowGrab(window->window, (state) ? SDL_TRUE : SDL_FALSE);
}

void H2DE_SetWindowResizable(H2DE_Window* window, bool state) {
    SDL_SetWindowResizable(window->window, (state) ? SDL_TRUE : SDL_FALSE);
}
