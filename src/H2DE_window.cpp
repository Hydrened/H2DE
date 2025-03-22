#include "H2DE/H2DE_window.h"

// INIT
H2DE_Engine::H2DE_Window::H2DE_Window(H2DE_Engine* e, H2DE_WindowData d) : engine(e), data(d) {
    initSDL();
    create();
    initSettings();
}

void H2DE_Engine::H2DE_Window::initSDL() const {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("H2DE-102: SDL_Init_Video failed: " + std::string(SDL_GetError()));
    }
    else if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        throw std::runtime_error("H2DE-103: SDL_Init_Audio failed: " + std::string(SDL_GetError()));
    }
    else if (!IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG) {
        throw std::runtime_error("H2DE-104: IMG_Init failed: " + std::string(IMG_GetError()));
    }
    else if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
        throw std::runtime_error("H2DE-105: Mix_Init failed: " + std::string(Mix_GetError()));
    }
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
        throw std::runtime_error("H2DE-106: Mix_OpenAudio failed: " + std::string(Mix_GetError()));
    }
}

void H2DE_Engine::H2DE_Window::create() {
    SDL_WindowFlags flag = (data.fullscreen) ? SDL_WINDOW_FULLSCREEN : (data.resizable) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_SHOWN;

    int x = H2DE_SettingsGetKeyInteger(engine, "WINDOW", "x", data.pos.x);
    int y = H2DE_SettingsGetKeyInteger(engine, "WINDOW", "y", data.pos.y);
    int w = H2DE_SettingsGetKeyInteger(engine, "WINDOW", "w", data.size.x);
    int h = H2DE_SettingsGetKeyInteger(engine, "WINDOW", "h", data.size.y);

    if (data.fullscreen) {
        SDL_DisplayMode dm;
        if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
            throw std::runtime_error("H2DE-107: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
        }

        x = 0;
        y = 0;
        w = dm.w;
        h = dm.h;

    }

    window = SDL_CreateWindow(data.title, x, y, w, h, SDL_WINDOW_SHOWN | flag);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("H2DE-108: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("H2DE-109: Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
}

void H2DE_Engine::H2DE_Window::initSettings() const {
    if (!data.saveState) {
        return;
    }

    H2DE_SettingsAddSection(engine, "WINDOW");
    H2DE_SettingsAddKey(engine, "WINDOW", "x", std::to_string(data.pos.x));
    H2DE_SettingsAddKey(engine, "WINDOW", "y", std::to_string(data.pos.y));
    H2DE_SettingsAddKey(engine, "WINDOW", "w", std::to_string(data.size.x));
    H2DE_SettingsAddKey(engine, "WINDOW", "h", std::to_string(data.size.y));
}

// CLEANUP
H2DE_Engine::H2DE_Window::~H2DE_Window() {
    quitSDL();
}

void H2DE_Engine::H2DE_Window::quitSDL() const {
    Mix_CloseAudio();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
}

void H2DE_Engine::H2DE_Window::saveState() const {
    if (!data.saveState) {
        return;
    }

    H2DE_AbsPos pos = H2DE_GetWindowPos(engine);
    H2DE_AbsSize size = H2DE_GetWindowSize(engine);
    
    H2DE_SettingsSetKeyValue(engine, "WINDOW", "x", std::to_string(pos.x));
    H2DE_SettingsSetKeyValue(engine, "WINDOW", "y", std::to_string(pos.y));
    H2DE_SettingsSetKeyValue(engine, "WINDOW", "w", std::to_string(size.x));
    H2DE_SettingsSetKeyValue(engine, "WINDOW", "h", std::to_string(size.y));
}

// GETTER
SDL_Window* H2DE_Engine::H2DE_Window::getWindow() const {
    return window;
}

SDL_Renderer* H2DE_Engine::H2DE_Window::getRenderer() const {
    return renderer;
}

H2DE_AbsPos H2DE_GetWindowPos(const H2DE_Engine* engine) {
    int x, y;
    SDL_GetWindowPosition(engine->window->getWindow(), &x, &y);
    return H2DE_AbsPos{ x, y };
}

H2DE_AbsSize H2DE_GetWindowSize(const H2DE_Engine* engine) {
    int w, h;
    SDL_GetWindowSize(engine->window->getWindow(), &w, &h);
    return H2DE_AbsSize{ w, h };
}

bool H2DE_IsWindowFullscreen(const H2DE_Engine* engine) {
    Uint32 flags = SDL_GetWindowFlags(engine->window->getWindow());
    return (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool H2DE_IsWindowResizable(const H2DE_Engine* engine) {
    return SDL_GetWindowFlags(engine->window->getWindow()) & SDL_WINDOW_RESIZABLE;
}
