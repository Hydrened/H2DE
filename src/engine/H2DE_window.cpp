#include "H2DE/engine/H2DE_window.h"

#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Window::H2DE_Window(H2DE_Engine* e, const H2DE_WindowData& d) : engine(e), data(d) {
    initSDL();
    initSettings();
    create();
}

void H2DE_Window::initSDL() const {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        H2DE_Error::throwError("SDL_Init_Video failed: " + std::string(SDL_GetError()));
    }
    else if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        H2DE_Error::throwError("SDL_Init_Audio failed: " + std::string(SDL_GetError()));
    }
    else if (!IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG) {
        H2DE_Error::throwError("IMG_Init failed: " + std::string(IMG_GetError()));
    }
    else if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) {
        H2DE_Error::throwError("Mix_Init failed: " + std::string(Mix_GetError()));
    }
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
        H2DE_Error::throwError("Mix_OpenAudio failed: " + std::string(Mix_GetError()));
    }
}

void H2DE_Window::initSettings() const {
    if (!data.saveState || data.fullscreen) {
        return;
    }
     
    static H2DE_Settings* settings = engine->getSettings();

    settings->addSection("WINDOW");
    settings->addKey("WINDOW", "x", std::to_string(data.pos.x));
    settings->addKey("WINDOW", "y", std::to_string(data.pos.y));
    settings->addKey("WINDOW", "w", std::to_string(data.size.x));
    settings->addKey("WINDOW", "h", std::to_string(data.size.y));
}

void H2DE_Window::create() {
    static H2DE_Settings* settings = engine->getSettings();

    const SDL_WindowFlags flag = H2DE_Window::getFlags(data.fullscreen, data.resizable);

    int x = settings->getKeyInteger("WINDOW", "x", data.pos.x);
    int y = settings->getKeyInteger("WINDOW", "y", data.pos.y);
    int w = settings->getKeyInteger("WINDOW", "w", data.size.x);
    int h = settings->getKeyInteger("WINDOW", "h", data.size.y);

    if (data.fullscreen) {
        SDL_DisplayMode dm;
        if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
            H2DE_Error::throwError("Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
        }

        x = 0;
        y = 0;
        w = dm.w;
        h = dm.h;
    }

    window = SDL_CreateWindow(data.title, x, y, w, h, SDL_WINDOW_SHOWN | flag);
    if (!window) {
        SDL_Quit();
        H2DE_Error::throwError("Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        H2DE_Error::throwError("Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }

    customRatio = (H2DE_WINDOW_RATIO_CUSTOM) ? static_cast<float>(data.size.x) / static_cast<float>(data.size.y) : 0.0f;
    fixRatioSize(H2DE_PixelSize{ w, h });
}

// CLEANUP
H2DE_Window::~H2DE_Window() {
    saveState();
    quitSDL();
}

void H2DE_Window::saveState() const {
    if (!data.saveState || data.fullscreen) {
        return;
    }

    const H2DE_PixelPos pos = getPos();
    const H2DE_PixelSize size = getSize();
    
    static H2DE_Settings* settings = engine->getSettings();

    settings->setKeyValue("WINDOW", "x", std::to_string(pos.x));
    settings->setKeyValue("WINDOW", "y", std::to_string(pos.y));
    settings->setKeyValue("WINDOW", "w", std::to_string(size.x));
    settings->setKeyValue("WINDOW", "h", std::to_string(size.y));
}

void H2DE_Window::quitSDL() const {
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

// UPDATE
void H2DE_Window::update() {
    oldSize = getSize();
}

// ACTIONS
void H2DE_Window::fixRatioSize(const H2DE_PixelSize& size) {
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        return;
    }

    static constexpr std::array<float, 8> ratios = { 0.0f, 4.0f / 3.0f, 3.0f / 2.0f, 5.0f / 4.0f, 16.0f / 10.0f, 16.0f / 9.0f, 21.0f / 9.0f, 32.0f / 9.0f };

    const int ratioIndex = static_cast<int>(data.ratio) - 1;
    if (ratioIndex < 0 || ratioIndex >= ratios.size()) {
        return;
    }

    const float ratio = (data.ratio != H2DE_WINDOW_RATIO_CUSTOM)
        ? ratios.at(ratioIndex)
        : customRatio;

    const int wDiff = H2DE::abs(size.x - oldSize.x);
    const int hDiff = H2DE::abs(size.y - oldSize.y);

    const bool resizeWidth = (!oldSize.isNull())
        ? (wDiff < hDiff)
        : false;

    H2DE_PixelSize finalSize = size;
    
    if (resizeWidth) {
        finalSize.x = static_cast<int>(size.y * ratio);
    } else {
        finalSize.y = static_cast<int>(size.x / ratio);
    }

    SDL_SetWindowSize(window, finalSize.x, finalSize.y);
    oldSize = finalSize;
}

// GETTER
H2DE_PixelPos H2DE_Window::getPos() const {
    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    return H2DE_PixelPos{ x, y };
}

H2DE_PixelSize H2DE_Window::getSize() const {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return H2DE_PixelSize{ w, h };
}

// SETTER
void H2DE_Window::setPos(const H2DE_PixelPos& pos) {
    SDL_SetWindowPosition(window, pos.x, pos.y);
}

void H2DE_Window::setSize(const H2DE_PixelSize& size) {
    SDL_SetWindowSize(window, size.x, size.y);
    fixRatioSize(getSize());
}

void H2DE_Window::setMinimumSize(const H2DE_PixelSize& minimumSize) {
    SDL_SetWindowMinimumSize(window, minimumSize.x, minimumSize.y);
}

void H2DE_Window::setMaximumSize(const H2DE_PixelSize& maximumSize) {
    SDL_SetWindowMaximumSize(window, maximumSize.x, maximumSize.y);
}

void H2DE_Window::setTitle(const std::string& title) {
    SDL_SetWindowTitle(window, title.c_str());
}

void H2DE_Window::setIcon(const std::string& textureName) {
    const std::vector<std::filesystem::path> files = engine->assetLoaderManager->getFilesToLoad("assets");

    for (const std::filesystem::path& file : files) {
        if (file.filename() != textureName) {
            continue;
        }

        SDL_Surface* surface = IMG_Load(file.string().c_str());
        if (surface) {
            SDL_SetWindowIcon(window, surface);
            SDL_FreeSurface(surface);
        }

        break;
    }
}

void H2DE_Window::setFullscreen(const H2DE_Engine* engine, bool fullscreen) {
    SDL_WindowFlags flag = getFlags(fullscreen, data.resizable);
    SDL_SetWindowFullscreen(window, flag);
}

void H2DE_Window::setResizable(const H2DE_Engine* engine, bool resizable) {
    SDL_SetWindowResizable(window, (resizable) ? SDL_TRUE : SDL_FALSE);
}

void H2DE_Window::setGrab(const H2DE_Engine* engine, bool grab) {
    SDL_SetWindowGrab(window, (grab) ? SDL_TRUE : SDL_FALSE);
}

void H2DE_Window::setRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio) {
    data.ratio = ratio;
    customRatio = (H2DE_WINDOW_RATIO_CUSTOM) ? static_cast<float>(data.size.x) / static_cast<float>(data.size.y) : 0.0f;
    fixRatioSize(getSize());
}

void H2DE_Window::setCursor(H2DE_Cursor c) {
    if (cursor != nullptr) {
        SDL_FreeCursor(cursor);
    }

    cursor = SDL_CreateSystemCursor(H2DE_Window::getSDLCursor(c));
    SDL_SetCursor(cursor);
}

// GETTER
SDL_SystemCursor H2DE_Window::getSDLCursor(H2DE_Cursor cursor) {
    switch (cursor) {
        case H2DE_CURSOR_ARROW: return SDL_SYSTEM_CURSOR_ARROW;
        case H2DE_CURSOR_IBEAM: return SDL_SYSTEM_CURSOR_IBEAM;
        case H2DE_CURSOR_WAIT: return SDL_SYSTEM_CURSOR_WAIT;
        case H2DE_CURSOR_CROSSHAIR: return SDL_SYSTEM_CURSOR_CROSSHAIR;
        case H2DE_CURSOR_WAITARROW: return SDL_SYSTEM_CURSOR_WAITARROW;
        case H2DE_CURSOR_SIZENWSE: return SDL_SYSTEM_CURSOR_SIZENWSE;
        case H2DE_CURSOR_SIZENESW: return SDL_SYSTEM_CURSOR_SIZENESW;
        case H2DE_CURSOR_SIZEWE: return SDL_SYSTEM_CURSOR_SIZEWE;
        case H2DE_CURSOR_SIZENS: return SDL_SYSTEM_CURSOR_SIZENS;
        case H2DE_CURSOR_SIZEALL: return SDL_SYSTEM_CURSOR_SIZEALL;
        case H2DE_CURSOR_NO: return SDL_SYSTEM_CURSOR_NO;
        case H2DE_CURSOR_HAND: return SDL_SYSTEM_CURSOR_HAND;
        default: return SDL_SYSTEM_CURSOR_ARROW;
    }
}
