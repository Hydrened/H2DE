#include "H2DE/H2DE_engine.h"
#include "H2DE/H2DE_error.h"
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
            H2DE_Error::throwError("Can't create more than one engine");
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
    if (window) {
        window->saveState();
    }

    for (H2DE_Object* object : objects) {
        H2DE_DestroyObject(this, object);
    }
    
    if (camera) {
        delete camera;
        camera = nullptr;
    }
    if (assetLoader) {
        delete assetLoader;
        assetLoader = nullptr;
    }
    if (volume) {
        delete volume;
        volume = nullptr;
    }
    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }
    if (window) {
        delete window;
        window = nullptr;
    }
    if (settings) {
        delete settings;
        settings = nullptr;
    }
}

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    if (engine) {
        delete engine;
        engine = nullptr;
    }
}

// RUN
void H2DE_RunEngine(H2DE_Engine* engine) {
    Uint32 now = SDL_GetTicks();
    int frameTime;
    SDL_Event event;

    try {
        H2DE_Error::checkEngine(engine);

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
    H2DE_Error::checkEngine(engine);
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

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    window->fixRatioSize(H2DE_AbsSize{ event.window.data1, event.window.data2 });
                }
                break;

            default: break;
        }

        handleButtonsEvents(event);

        if (handleEventsCall) {
            handleEventsCall(event);
        }
    }
}

void H2DE_Engine::handleButtonsEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            handleButtonsMouseDownEvent();
            break;

        case SDL_MOUSEBUTTONUP:
            handleButtonsMouseUpEvent();
            break;

        case SDL_MOUSEMOTION:
            handleButtonsBlurEvents();
            handleButtonsHoverEvents();
            break;

        default: return;
    }
}

void H2DE_Engine::handleButtonsMouseDownEvent() {
    for (H2DE_ButtonObject* button : getValidButtons()) {
        H2DE_Error::checkObject(button);

        if (!button->bod.onMouseDown) {
            continue;
        }

        for (const auto& [name, hitbox] : button->od.hitboxes) {
            H2DE_LevelRect buttonRect = button->od.pos.makeNullRect() + hitbox.rect;

            if (buttonRect.collides(H2DE_GetMousePos(this, button->od.absolute))) {
                button->bod.onMouseDown(button);
                mouseDown = button;
                return;
            }
        }
    }
}

void H2DE_Engine::handleButtonsMouseUpEvent() {
    if (!mouseDown) {
        return;
    }

    if (mouseDown->bod.onMouseUp) {
        mouseDown->bod.onMouseUp(mouseDown);
        mouseDown = nullptr;
    }
}

void H2DE_Engine::handleButtonsBlurEvents() {
    if (!hovered) {
        return;
    }

    bool stillHovering = false;

    for (const auto& [name, hitbox] : hovered->od.hitboxes) {
        const H2DE_LevelRect buttonRect = hovered->od.pos.makeNullRect() + hitbox.rect;

        if (buttonRect.collides(H2DE_GetMousePos(this, hovered->od.absolute))) {
            stillHovering = true;
            break;
        }
    }

    if (!stillHovering) {
        if (hovered->bod.onBlur) {
            hovered->bod.onBlur(hovered);
        }

        hovered = nullptr;
    }
}

void H2DE_Engine::handleButtonsHoverEvents() {
    for (H2DE_ButtonObject* button : getValidButtons()) {
        H2DE_Error::checkObject(button);

        if (!button->bod.onHover && !button->bod.onBlur) {
            continue;
        }

        for (const auto& [name, hitbox] : button->od.hitboxes) {
            const H2DE_LevelRect buttonRect = button->od.pos.makeNullRect() + hitbox.rect;

            if (!buttonRect.collides(H2DE_GetMousePos(this, button->od.absolute))) {
                continue;
            }

            if (hovered) {
                if (hovered == button) {
                    continue;
                }

                if (hovered->od.index > button->od.index) {
                    continue;
                }

                if (H2DE_Engine::isPositionGreater(button, hovered)) {
                    continue;
                }
            }

            if (hovered) {
                if (hovered->bod.onBlur) {
                    hovered->bod.onBlur(hovered);
                }
            }

            hovered = button;
            if (button->bod.onHover) {
                button->bod.onHover(button);
            }

            return;
        }
    }
}

std::vector<H2DE_ButtonObject*> H2DE_Engine::getValidButtons() const {
    std::vector<H2DE_ButtonObject*> res;

    for (H2DE_Object* object : objects) {
        H2DE_Error::checkObject(object);

        H2DE_ButtonObject* button = dynamic_cast<H2DE_ButtonObject*>(object);

        if (button) {
            if (button->hidden) {
                continue;
            }

            if (paused && button->bod.pauseSensitive) {
                continue;
            }

            res.push_back(button);
        }
    }

    std::sort(res.begin(), res.end(), [](H2DE_ButtonObject* a, H2DE_ButtonObject* b) {
        const int indexA = H2DE_GetObjectIndex(a);
        const int indexB = H2DE_GetObjectIndex(b);

        if (indexA == indexB) {
            return H2DE_Engine::isPositionGreater(a, b);
        }

        return indexA > indexB;
    });

    return res;
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
}

void H2DE_Engine::updateObjects() {
    for (H2DE_Object* object : objects) {
        H2DE_Error::checkObject(object);
        object->update();
    }
}

// CALLS
void H2DE_SetHandleEventsCall(H2DE_Engine* engine, const std::function<void(SDL_Event)>& call) {
    H2DE_Error::checkEngine(engine);
    engine->handleEventsCall = call;
}

void H2DE_SetUpdateCall(H2DE_Engine* engine, const std::function<void()>& call) {
    H2DE_Error::checkEngine(engine);
    engine->updateCall = call;
}

// FPS
unsigned int H2DE_GetCurrentFps(const H2DE_Engine* engine) {
    H2DE_Error::checkEngine(engine);
    return engine->currentFPS;
}

unsigned int H2DE_GetFps(const H2DE_Engine* engine) {
    H2DE_Error::checkEngine(engine);
    return engine->fps;
}

void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps) {
    H2DE_Error::checkEngine(engine);
    engine->fps = fps;
}

// PAUSE
bool H2DE_IsPaused(const H2DE_Engine* engine) {
    H2DE_Error::checkEngine(engine);
    return engine->paused;
}

void H2DE_Pause(H2DE_Engine* engine) {
    H2DE_Error::checkEngine(engine);

    engine->paused = true;
    engine->volume->pause();
}

void H2DE_Resume(H2DE_Engine* engine) {
    H2DE_Error::checkEngine(engine);

    engine->paused = false;
    engine->volume->resume();
}

// MOUSE
H2DE_LevelPos H2DE_GetMousePos(const H2DE_Engine* engine, bool absolute) {
    H2DE_Error::checkEngine(engine);
    return engine->renderer->absToLvl(engine->mousePos, absolute);
}

// DELAY
unsigned int H2DE_Delay(const H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, bool pauseSensitive) {
    H2DE_Error::checkEngine(engine);
    return H2DE_CreateTimeline(engine, ms, H2DE_EASING_LINEAR, nullptr, callback, 0, pauseSensitive);
}

void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id) {
    H2DE_Error::checkEngine(engine);
    H2DE_ResetTimeline(engine, id);
}

void H2DE_StopDelay(const H2DE_Engine* engine, unsigned int id, bool call) {
    H2DE_Error::checkEngine(engine);
    H2DE_StopTimeline(engine, id, call);
}

// GETTER
bool H2DE_Engine::isPositionGreater(H2DE_Object* object1, H2DE_Object* object2) {
    H2DE_LevelPos object1Pos = H2DE_GetObjectPos(object1);
    H2DE_LevelPos object2Pos = H2DE_GetObjectPos(object2);

    bool equalsX = object1Pos.x == object2Pos.x;
    return (equalsX) ? object1Pos.y < object2Pos.y : object1Pos.x < object2Pos.x;
}
