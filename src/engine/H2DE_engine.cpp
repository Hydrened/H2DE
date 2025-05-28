#include "H2DE/engine/H2DE_engine.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Engine::H2DE_Engine(const H2DE_EngineData& d) : data(d), fps(data.window.fps) {
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
        buttonManager = new H2DE_ButtonManager(this);

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

    if (timelineManager != nullptr) {
        delete timelineManager;
        timelineManager = nullptr;
    }

    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
    }

    if (buttonManager != nullptr) {
        delete buttonManager;
        buttonManager = nullptr;
    }

    destroyObjects();

    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
    }
}

void H2DE_Engine::destroyObjects() {
    for (H2DE_Object* object : objects) {
        delete object;
        object = nullptr;
    }

    objects.clear();
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
        Uint64 frameCount = 0;

        SDL_Event event;

        while (isRunning) {
            Uint64 now = SDL_GetPerformanceCounter();
            deltaTime = (now - lastTime) / static_cast<float>(perfFreq);
            frameCount++;

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
                currentFPS = frameCount;
                frameCount = 0;
                lastSec = now;
            }

            lastTime = now;
        }

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }

    destroy();
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
                    window->fixRatioSize(H2DE_PixelSize{ event.window.data1, event.window.data2 });
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

        buttonManager->handleEvents(event);
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
unsigned int H2DE_Engine::createTimeline(unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive) {
    return timelineManager->create(duration, easing, update, completed, loops, pauseSensitive);
}

void H2DE_Engine::resetTimeline(unsigned int id) {
    timelineManager->reset(id);
}

void H2DE_Engine::stopTimeline(unsigned int id, bool callCompleted) {
    timelineManager->stop(id, callCompleted);
}

// -- delay
unsigned int H2DE_Engine::delay(unsigned int duration, const std::function<void()>& callback, bool pauseSensitive) {
    return timelineManager->create(duration, H2DE_EASING_LINEAR, nullptr, callback, 1, pauseSensitive);
}

void H2DE_Engine::resetDelay(unsigned int id) {
    timelineManager->reset(id);
}

void H2DE_Engine::stopDelay(unsigned int id,  bool callCompleted) {
    timelineManager->stop(id, callCompleted);
}

// -- objects
H2DE_BarObject* H2DE_Engine::createBarObject(const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData) {
    H2DE_BarObject* object = new H2DE_BarObject(this, objectData, barObjectData);
    objects.push_back(object);
    return object;
}

H2DE_BasicObject* H2DE_Engine::createBasicObject(const H2DE_ObjectData& objectData) {
    H2DE_BasicObject* object = new H2DE_BasicObject(this, objectData);
    objects.push_back(object);
    return object;
}

H2DE_ButtonObject* H2DE_Engine::createButtonObject(const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData) {
    H2DE_ButtonObject* object = new H2DE_ButtonObject(this, objectData, buttonObjectData);
    objects.push_back(object);
    buttonManager->updateButtonBuffer(objects);
    return object;
}

H2DE_TextObject* H2DE_Engine::createTextObject(const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData) {
    H2DE_TextObject* object = new H2DE_TextObject(this, objectData, textObjectData);
    objects.push_back(object);
    return object;
}

bool H2DE_Engine::destroyObject(H2DE_Object* object) {
    auto it = std::find(objects.begin(), objects.end(), object);

    bool isButton = (dynamic_cast<H2DE_ButtonObject*>(object) != nullptr);
    if (isButton) {
        buttonManager->updateButtonBuffer(objects);
    }

    if (it != objects.end()) {
        objects.erase(it);
        return true;
    }

    return false;
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
