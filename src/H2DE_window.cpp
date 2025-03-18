#include "H2DE/H2DE_window.h"

// INIT
H2DE_Engine::H2DE_Window::H2DE_Window(H2DE_Engine* e, H2DE_WindowData d) : engine(e), data(d) {
    initSDL();
    create();
}

void H2DE_Engine::H2DE_Window::initSDL() {
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
    window = SDL_CreateWindow(data.title, data.pos.x, data.pos.y, data.size.x, data.size.y, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("H2DE-107: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("H2DE-108: Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
}

// CLEANUP
H2DE_Engine::H2DE_Window::~H2DE_Window() {
    quitSDL();
}

void H2DE_Engine::H2DE_Window::quitSDL() {
    Mix_CloseAudio();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    if (window) SDL_DestroyWindow(window);
    if (renderer) SDL_DestroyRenderer(renderer);
}

// GETTER
SDL_Renderer* H2DE_Engine::H2DE_Window::getRenderer() const {
    return renderer;
}
