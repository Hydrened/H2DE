#include "H2DE/H2DE_engine.h"
#include "H2DE/H2DE_window.h"
#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_asset_loader.h"
#include "H2DE/H2DE_settings.h"
#include "H2DE/H2DE_camera.h"
#include "H2DE/H2DE_level_object.h"
#include "H2DE/H2DE_interface_object_bar.h"
#include "H2DE/H2DE_interface_object_button.h"
#include "H2DE/H2DE_interface_object_image.h"
#include "H2DE/H2DE_interface_object_text.h"

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
        camera = new H2DE_Camera(this, data.camera);

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data) {
    return new H2DE_Engine(data);
}

// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    window->saveState();

    for (H2DE_LevelObject* levelObject : levelObjects) {
        H2DE_DestroyLevelObject(this, levelObject);
    }

    for (H2DE_InterfaceObject* interfaceObject : interfaceObjects) {
        H2DE_DestroyInterfaceObject(this, interfaceObject);
    }
    
    delete camera;
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
            engine->currentFPS = static_cast<unsigned int>(1000.0f / static_cast<float>((frameTime > 0) ? frameTime : 1));
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
unsigned int H2DE_GetCurrentFps(H2DE_Engine* engine) {
    return engine->currentFPS;
}

unsigned int H2DE_GetFps(H2DE_Engine* engine) {
    return engine->fps;
}

void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps) {
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

// OBJECTS
void H2DE_Engine::addLevelObject(H2DE_LevelObject* obj) {
    levelObjects.push_back(obj);
}

void H2DE_Engine::destroyLevelObject(H2DE_LevelObject* obj) {
    if (obj) {
        auto it = std::find(levelObjects.begin(), levelObjects.end(), obj);

        if (it != levelObjects.end()) {
            levelObjects.erase(it);
        }
    }
}

void H2DE_Engine::addInterfaceObject(H2DE_InterfaceObject* obj) {
    interfaceObjects.push_back(obj);
}

void H2DE_Engine::destroyInterfaceObject(H2DE_InterfaceObject* obj) {
    if (obj) {
        auto it = std::find(interfaceObjects.begin(), interfaceObjects.end(), obj);

        if (it != interfaceObjects.end()) {
            interfaceObjects.erase(it);
        }
    }
}
