#include "H2DE/H2DE_engine.h"
#include "H2DE/H2DE_window.h"
#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_volume.h"
#include "H2DE/H2DE_asset_loader.h"
#include "H2DE/H2DE_settings.h"
#include "H2DE/H2DE_camera.h"
#include "H2DE/H2DE_timeline.h"

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
        renderer = new H2DE_Renderer(this, window->renderer, objects);
        volume = new H2DE_Volume(this);
        assetLoader = new H2DE_AssetLoader(this, window->renderer);
        camera = new H2DE_Camera(this, data.camera);
        timeline = new H2DE_Timeline(this);

        volume->loadData();

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

    for (H2DE_Object* object : objects) {
        H2DE_DestroyObject(this, object);
    }
    
    delete camera;
    delete assetLoader;
    delete volume;
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

            engine->handleEvents(event);
            engine->update();
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

// EVENTS
void H2DE_Engine::handleEvents(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_MOUSEMOTION:
                mousePos = { event.motion.x, event.motion.y };
                break;

            case SDL_MOUSEBUTTONDOWN:
                click = { event.button.x, event.button.y };
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    window->fixRatioSize(H2DE_AbsSize{ event.window.data1, event.window.data2 });
                }
                break;

            default: break;
        }

        if (handleEventsCall) {
            handleEventsCall(event);
        }
    }
}

// UPDATE
void H2DE_Engine::update() {
    window->update();
    
    timeline->update();

    if (!paused) {
        if (updateCall) {
            updateCall();
        }

        updateObjects();
        camera->update();
    }

    click = std::nullopt;
}

void H2DE_Engine::updateObjects() {
    for (H2DE_Object* object : objects) {
        object->update();
    }
}

// CALLS
void H2DE_SetHandleEventsCall(H2DE_Engine* engine, const std::function<void(SDL_Event)>& call) {
    engine->handleEventsCall = call;
}

void H2DE_SetUpdateCall(H2DE_Engine* engine, const std::function<void()>& call) {
    engine->updateCall = call;
}

// FPS
unsigned int H2DE_GetCurrentFps(const H2DE_Engine* engine) {
    return engine->currentFPS;
}

unsigned int H2DE_GetFps(const H2DE_Engine* engine) {
    return engine->fps;
}

void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps) {
    engine->fps = fps;
}

// PAUSE
bool H2DE_IsPaused(const H2DE_Engine* engine) {
    return engine->paused;
}

void H2DE_Pause(H2DE_Engine* engine) {
    engine->paused = true;
    engine->volume->pause();
}

void H2DE_Resume(H2DE_Engine* engine) {
    engine->paused = false;
    engine->volume->resume();
}

// MOUSE
H2DE_LevelPos H2DE_GetMousePos(const H2DE_Engine* engine, bool absolute) {
    return engine->renderer->absToLvl(engine->mousePos, absolute);
}

// DELAY
unsigned int H2DE_Delay(const H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, bool pauseSensitive) {
    return H2DE_CreateTimeline(engine, ms, H2DE_EASING_LINEAR, nullptr, callback, 0, pauseSensitive);
}

void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id) {
    H2DE_ResetTimeline(engine, id);
}

void H2DE_StopDelay(const H2DE_Engine* engine, unsigned int id, bool call) {
    H2DE_StopTimeline(engine, id, call);
}
