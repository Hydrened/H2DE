#include "H2DE/engine/H2DE_engine.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Engine::H2DE_Engine(const H2DE_EngineData& d) : data(d), fps(data.window.fps), deltaTime(1.0f / data.window.fps) {
    try {
        static bool once = false;
        if (once) {
            H2DE_Error::throwError("Can't create more than one engine");
        }
        once = true;

        objects.reserve(10000);

        settings = new H2DE_Settings(this);
        window = new H2DE_Window(this, data.window);
        assetLoaderManager = new H2DE_AssetLoaderManager(this, window->renderer);
        renderer = new H2DE_Renderer(this, window->renderer, objects);
        volume = new H2DE_Volume(this);
        timelineManager = new H2DE_TimelineManager(this);
        camera = new H2DE_Camera(this, data.camera);
        objectManager = new H2DE_ObjectManager(this);

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data) {
    return new H2DE_Engine(data);
}

// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    destroy();
}

void H2DE_Engine::destroy() {
    if (window != nullptr) {
        delete window;
        window = nullptr;
    }

    if (assetLoaderManager != nullptr) {
        delete assetLoaderManager;
        assetLoaderManager = nullptr;
    }

    if (renderer != nullptr) {
        delete renderer;
        renderer = nullptr;
    }

    if (volume != nullptr) {
        delete volume;
        volume = nullptr;
    }

    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }

    if (objectManager != nullptr) {
        delete objectManager;
        objectManager = nullptr;
    }

    destroyObjects();

    if (timelineManager != nullptr) {
        delete timelineManager;
        timelineManager = nullptr;
    }

    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
    }
}

void H2DE_Engine::destroyObjects() {
    for (auto it = objects.begin(); it != objects.end(); ) {
        H2DE_Object* object = *it;

        if (object == nullptr) {
            ++it;
            continue;
        }
        
        delete object;
        object = nullptr;
        it = objects.erase(it);
    }
}

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    if (engine != nullptr) {
        delete engine;
    }
}

// RUN
void H2DE_Engine::run() {
    if (isRunning) {
        return;
    }

    isRunning = true;

    try {
        Uint64 perfFreq = SDL_GetPerformanceFrequency();
        Uint64 lastTime = SDL_GetPerformanceCounter();
        Uint64 lastSec = lastTime;

        SDL_Event event;

        while (isRunning) {
            Uint64 now = SDL_GetPerformanceCounter();
            deltaTime = (now - lastTime) / static_cast<float>(perfFreq);
            currentFPS = (deltaTime > 0.0f) ? 1.0f / deltaTime : 0.0f;

            handleEvents(event);

            if (!debugModeEnabled) {
                update();
                renderer->render();
            }

            const float targetFrameTime = 1.0f / fps;
            while ((SDL_GetPerformanceCounter() - now) / static_cast<float>(perfFreq) < targetFrameTime) {
                // Spin-wait
            }

            if ((now - lastSec) / static_cast<float>(perfFreq) >= 1.0f) {
                lastSec = now;
            }

            lastTime = now;
        }

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

// EVENTS
void H2DE_Engine::handleEvents(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    window->setSize(H2DE_PixelSize{ event.window.data1, event.window.data2 });
                    camera->updateGrid();
                }
                break;

            case SDL_MOUSEMOTION:
                mousePos = { event.button.x, event.button.y };
                break;

            default: break;
        }

        if (handleEventsCall) {
            handleEventsCall(event);
        }

        objectManager->handleEvents(event);
    }
}

// UPDATE
void H2DE_Engine::update() {
    window->update();
    timelineManager->update();

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
        object->update();
    }
}

// ACTIONS

// -- assets
void H2DE_Engine::loadAssets(const std::string& directory) {
    assetLoaderManager->loadAssets(directory);
}

void H2DE_Engine::loadFont(const std::string& name, const H2DE_Font& font) {
    assetLoaderManager->loadFont(name, font);
}

// -- debug mode
void H2DE_Engine::debugMode(bool state) {
    debugModeEnabled = state;
}

void H2DE_Engine::debugModeNextFrame() {
    if (!debugModeEnabled) {
        return;
    }

    update();
    renderer->render();
}

void H2DE_Engine::debugModePreviousFrame() {
    if (!debugModeEnabled) {
        return;
    }
}

// -- timeline
H2DE_TimelineID H2DE_Engine::createTimeline(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive) {
    return timelineManager->create(duration, easing, update, completed, loops, pauseSensitive);
}

void H2DE_Engine::pauseTimeline(H2DE_TimelineID id) {
    timelineManager->pause(id);
}

void H2DE_Engine::resumeTimeline(H2DE_TimelineID id) {
    timelineManager->resume(id);
}

void H2DE_Engine::togglePauseTimeline(H2DE_TimelineID id) {
    timelineManager->togglePause(id);
}

void H2DE_Engine::resetTimeline(H2DE_TimelineID id) {
    timelineManager->reset(id);
}

void H2DE_Engine::stopTimeline(H2DE_TimelineID id, bool callCompleted) {
    timelineManager->stop(id, callCompleted);
}

bool H2DE_Engine::isTimelinePaused(H2DE_TimelineID id) const {
    return timelineManager->isPaused(id);
}

// -- delay
H2DE_DelayID H2DE_Engine::delay(uint32_t duration, const std::function<void()>& callback, bool pauseSensitive) {
    return timelineManager->create(duration, H2DE_EASING_LINEAR, nullptr, callback, 1, pauseSensitive);
}

// -- objects
void H2DE_Engine::refreshObjectManager() {
    objectManager->refreshButtonBuffer(objects);
}

bool H2DE_Engine::destroyObject(H2DE_Object* object) {
    auto it = std::find(objects.begin(), objects.end(), object);

    if (it == objects.end()) {
        return false;
    }

    bool isButton = (dynamic_cast<H2DE_ButtonObject*>(object) != nullptr);

    objects.erase(it);
    
    delete object;
    
    if (isButton) {
        objectManager->refreshButtonBuffer(objects);
    }

    return true;
}

// -- mouse
const H2DE_Translate H2DE_Engine::getMousePos(bool absolute) const {
    return renderer->pixelToLevel(mousePos, absolute);
}

// GETTER
bool H2DE_Engine::isPositionGreater(H2DE_Object* a, H2DE_Object* b) {
    const H2DE_Translate aTranslate = a->getTranslate();
    const H2DE_Translate bTranslate = b->getTranslate();

    bool equalsX = (aTranslate.x == bTranslate.x);

    return (equalsX)
        ? (aTranslate.y < bTranslate.y)
        : (aTranslate.x < bTranslate.x);
}

uint32_t H2DE_Engine::getObjectsRenderedNumber() const {
    return renderer->objectsRendered;
}

uint32_t H2DE_Engine::getSurfacesRenderedNumber() const {
    return renderer->surfacesRendered;
}

uint32_t H2DE_Engine::getHitboxesRenderedNumber() const {
    return renderer->hitboxesRendered;
}
