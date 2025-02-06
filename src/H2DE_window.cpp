#include "H2DE/H2DE_window.h"

// INIT
H2DE_Window::H2DE_Window(H2DE_Engine* e, H2DE_WindowData d) : engine(e), data(d) {
    static bool once = false;
    if (once) throw std::runtime_error("H2DE-110: Can't create more than one window");
    once = true;
    
    initSDL();
    createWindow();
}

void H2DE_Window::initSDL() {
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
}

void H2DE_Window::loadData() {
    std::filesystem::path file = "window.dat";
    if (std::filesystem::exists(file)) {
        try {
            json jsonData = H2DE_ReadJson(file);

            bool fullscreen = jsonData["f"];
            data.pos = (fullscreen) ? H2DE_AbsPos{ 0, 0 } : H2DE_AbsPos{ jsonData["x"], jsonData["y"] };
            data.size = (fullscreen) ? H2DE_AbsSize{ 0, 0 } : H2DE_AbsSize{ jsonData["w"], jsonData["h"] };
            data.fullscreen = fullscreen;

        } catch (const std::exception& eOpen) {
            try {
                if (!std::filesystem::remove(file)) std::runtime_error("H2DE-501: Can't remove window data");
            } catch (std::exception& eRemove) {
                throw std::runtime_error("H2DE-502: Can't remove window data: " + std::string(eRemove.what()));
            }
            throw std::runtime_error("H2DE-203: Error loading window data: " + std::string(eOpen.what()));
        }
    }
}

void H2DE_Window::createWindow() {
    if (data.saveState) loadData();

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
    if (data.saveState) saveData();
    quitSDL();
}

void H2DE_Window::saveData() {
    std::filesystem::path file = "window.dat";

    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    Uint32 flags = SDL_GetWindowFlags(window);

    json data;
    data["x"] = x;
    data["y"] = y;
    data["w"] = w;
    data["h"] = h;
    data["f"] = (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);

    H2DE_WriteJson(file, data, true);
}

void H2DE_Window::quitSDL() {
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
