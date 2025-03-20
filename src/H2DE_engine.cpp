#include "H2DE_engine.h"
#include "H2DE_window.h"
#include "H2DE_renderer.h"
#include "H2DE_asset_loader.h"
#include "H2DE_settings.h"

// INIT
H2DE_Engine::H2DE_Engine(H2DE_EngineData d) : data(d), fps(data.window.fps) {
    try {
        static bool once = false;
        if (once) {
            throw std::runtime_error("H2DE-101: Can't create more than one engine");
        }
        once = true;

        settings = new H2DE_Settings();
        window = new H2DE_Window(this, data.window);
        renderer = new H2DE_Renderer(this);
        assetLoader = new H2DE_AssetLoader(this, window->getRenderer());

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data) {
    return new H2DE_Engine(data);
}

// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    window->saveState();

    for (H2DE_LevelObject* object : objects) {
        H2DE_DestroyLevelObject(this, object);
    }
    
    delete assetLoader;
    delete renderer;
    delete window;
    delete settings;
}

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    if (engine) {
        delete engine;
    }
}

// RUN
void H2DE_RunEngine(H2DE_Engine* engine) {
    Uint32 now = SDL_GetTicks();
    int frameTime;
    SDL_Event event;

    try {
        while (engine->isRunning) {
            now = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: engine->isRunning = false; break;
                    default: break;
                }

                if (engine->handleEvents) engine->handleEvents(event);
            }

            if (!engine->paused) {
                if (engine->update) {
                    engine->update();
                }
            }

            engine->renderer->render();
            
            frameTime = SDL_GetTicks() - now;
            engine->currentFPS = 1000.0f / static_cast<float>((frameTime > 0) ? frameTime : 1);
            int timePerFrame = 1000 / engine->fps;
            if (timePerFrame >= frameTime) {
                SDL_Delay(timePerFrame - frameTime);
            }
        }
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

void H2DE_StopEngine(H2DE_Engine* engine) {
    engine->isRunning = false;
}

// ASSETS
void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory) {
    engine->assetLoader->load(directory);
    engine->renderer->setTextures(engine->assetLoader->getLoadedTextures());
    engine->renderer->setSounds(engine->assetLoader->getLoadedSounds());
}

// CALLS
void H2DE_SetHandleEventsCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call) {
    engine->handleEvents = call;
}

void H2DE_SetUpdateCall(H2DE_Engine* engine, std::function<void()> call) {
    engine->update = call;
}

// FPS
int H2DE_GetCurrentFps(H2DE_Engine* engine) {
    return engine->currentFPS;
}

void H2DE_SetFPS(H2DE_Engine* engine, unsigned int fps) {
    engine->fps = fps;
}

// PAUSE
bool H2DE_IsPaused(H2DE_Engine* engine) {
    return engine->paused;
}

void H2DE_Pause(H2DE_Engine* engine) {
    engine->paused = true;
}

void H2DE_Resume(H2DE_Engine* engine) {
    engine->paused = false;
}

// WINDOW
H2DE_AbsPos H2DE_GetWindowPos(H2DE_Engine* engine) {
    int x, y;
    SDL_GetWindowPosition(engine->window->getWindow(), &x, &y);
    return H2DE_AbsPos{ x, y };
}

H2DE_AbsSize H2DE_GetWindowSize(H2DE_Engine* engine) {
    int w, h;
    SDL_GetWindowSize(engine->window->getWindow(), &w, &h);
    return H2DE_AbsSize{ w, h };
}

bool H2DE_IsWindowFullscreen(H2DE_Engine* engine) {
    Uint32 flags = SDL_GetWindowFlags(engine->window->getWindow());
    return (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
}

bool H2DE_IsWindowResizable(H2DE_Engine* engine) {
    return SDL_GetWindowFlags(engine->window->getWindow()) & SDL_WINDOW_RESIZABLE;
}

// SETTINGS
bool H2DE_SettingsAddSection(H2DE_Engine* engine, const std::string& section) {
    return engine->settings->addSection(section);
}

bool H2DE_SettingsAddKey(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value) {
    return engine->settings->addKey(section, key, value);
}

bool H2DE_SettingsSetKeyValue(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value) {
    return engine->settings->setKeyValue(section, key, value);
}

int H2DE_SettingsGetKeyInteger(H2DE_Engine* engine, const std::string& section, const std::string& key, int defaultValue) {
    return engine->settings->getReader()->GetInteger(section, key, defaultValue);
}
