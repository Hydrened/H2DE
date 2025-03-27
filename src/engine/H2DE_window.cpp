#include "H2DE/H2DE_window.h"
#include "H2DE/H2DE_asset_loader.h"

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
    SDL_WindowFlags flag = getFlags(data.fullscreen, data.resizable);

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

    fixRatioSize(H2DE_AbsSize{ w, h });
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

// EVENTS
void H2DE_Engine::H2DE_Window::fixRatioSize(const H2DE_AbsSize& size) {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        return;
    }

    if (oldSize == size) {
        return;
    }

    float ratio;
    switch (data.ratio) {
        case H2DE_WINDOW_RATIO_4_3: ratio = 4.0f / 3.0f; break;
        case H2DE_WINDOW_RATIO_3_2: ratio = 3.0f / 2.0f; break;
        case H2DE_WINDOW_RATIO_5_4: ratio = 5.0f / 4.0f; break;
        case H2DE_WINDOW_RATIO_16_10: ratio = 16.0f / 10.0f; break;
        case H2DE_WINDOW_RATIO_16_9: ratio = 16.0f / 9.0f; break;
        case H2DE_WINDOW_RATIO_21_9: ratio = 21.0f / 9.0f; break;
        case H2DE_WINDOW_RATIO_32_9: ratio = 32.0f / 9.0f; break;
        default: return;
    }

    int wDiff = std::abs(size.x - oldSize.x);
    int hDiff = std::abs(size.y - oldSize.y);

    bool resizeWidth = (!oldSize.isNull())
        ? wDiff < hDiff
        : false;

    H2DE_AbsSize finalSize = size;
    
    if (resizeWidth) {
        finalSize.x = static_cast<int>(size.y * ratio);
    } else {
        finalSize.y = static_cast<int>(size.x / ratio);
    }

    SDL_SetWindowSize(window, finalSize.x, finalSize.y);
    oldSize = finalSize;
}

// UPDATE
void H2DE_Engine::H2DE_Window::update() {
    oldSize = H2DE_GetWindowSize(engine);
}

// GETTER
SDL_WindowFlags H2DE_Engine::H2DE_Window::getFlags(bool fullscreen, bool resizable) const {
    return (fullscreen) ? SDL_WINDOW_FULLSCREEN : (resizable) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_SHOWN;
}

H2DE_AbsPos H2DE_GetWindowPos(const H2DE_Engine* engine) {
    int x, y;
    SDL_GetWindowPosition(engine->window->window, &x, &y);
    return H2DE_AbsPos{ x, y };
}

H2DE_AbsSize H2DE_GetWindowSize(const H2DE_Engine* engine) {
    int w, h;
    SDL_GetWindowSize(engine->window->window, &w, &h);
    return H2DE_AbsSize{ w, h };
}

// SETTER
void H2DE_SetWindowPos(const H2DE_Engine* engine, const H2DE_AbsPos& pos) {
    SDL_SetWindowPosition(engine->window->window, pos.x, pos.y);
}

void H2DE_SetWindowSize(const H2DE_Engine* engine, const H2DE_AbsSize& size) {
    SDL_SetWindowSize(engine->window->window, size.x, size.y);
    H2DE_SetWindowSize(engine, size);
}

void H2DE_SetWindowMinimumSize(const H2DE_Engine* engine, const H2DE_AbsSize& minimumSize) {
    SDL_SetWindowMinimumSize(engine->window->window, minimumSize.x, minimumSize.y);
}

void H2DE_SetWindowMaximumSize(const H2DE_Engine* engine, const H2DE_AbsSize& maximumSize) {
    SDL_SetWindowMaximumSize(engine->window->window, maximumSize.x, maximumSize.y);
}

void H2DE_SetWindowTitle(const H2DE_Engine* engine, const std::string& title) {
    SDL_SetWindowTitle(engine->window->window, title.c_str());
}

void H2DE_SetWindowIcon(const H2DE_Engine* engine, const std::string& textureName) {
    std::vector<std::filesystem::path> files = engine->assetLoader->getFilesToLoad("assets");

    for (const std::filesystem::path& file : files) {
        if (file.filename() == textureName) {

            SDL_Surface* surface = IMG_Load(file.string().c_str());
            if (surface) {
                SDL_SetWindowIcon(engine->window->window, surface);
                SDL_FreeSurface(surface);
            }

            break;
        }
    }
}

void H2DE_SetWindowFullscreen(const H2DE_Engine* engine, bool fullscreen) {
    SDL_WindowFlags flag = engine->window->getFlags(fullscreen, engine->window->data.resizable);
    SDL_SetWindowFullscreen(engine->window->window, flag);
}

void H2DE_SetWindowResizable(const H2DE_Engine* engine, bool resizable) {
    SDL_SetWindowResizable(engine->window->window, (resizable) ? SDL_TRUE : SDL_FALSE);
}

void H2DE_SetWindowGrab(const H2DE_Engine* engine, bool grab) {
    SDL_SetWindowGrab(engine->window->window, (grab) ? SDL_TRUE : SDL_FALSE);
}

void H2DE_SetWindowRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio) {
    engine->window->data.ratio = ratio;

    if (ratio != H2DE_WINDOW_RATIO_NO_RATIO) {
        engine->window->fixRatioSize(H2DE_GetWindowSize(engine));
    }
}
