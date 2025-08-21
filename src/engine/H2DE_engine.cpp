#include "H2DE/engine/H2DE_engine.h"
#include "H2DE/engine/H2DE_error.h"
#include <windows.h>

// INIT
H2DE_Engine::H2DE_Engine(const H2DE_EngineData& d) : _data(d), _fps(_data.window.fps), _deltaTime(1.0f / _data.window.fps) {
    try {
        static bool once = false;
        if (once) {
            H2DE_Error::throwError("Can't create more than one engine");
        }
        once = true;

        _objects.reserve(10000);

        _settings = new H2DE_Settings(this);
        _window = new H2DE_Window(this, _data.window);
        _assetLoaderManager = new H2DE_AssetLoaderManager(this, _window->_renderer);
        _renderer = new H2DE_Renderer(this, _window->_renderer, _objects);
        _audio = new H2DE_Audio(this);
        _timelineManager = new H2DE_TimelineManager(this);
        _chronoManager = new H2DE_ChronoManager(this);
        _camera = new H2DE_Camera(this, _data.camera);
        _objectManager = new H2DE_ObjectManager(this);

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

H2DE_Engine* H2DE::createEngine(const H2DE_EngineData& data) {
    return new H2DE_Engine(data);
}

// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    _destroy();
}

void H2DE_Engine::_destroy() {
    if (_window != nullptr) {
        delete _window;
        _window = nullptr;
    }

    if (_assetLoaderManager != nullptr) {
        delete _assetLoaderManager;
        _assetLoaderManager = nullptr;
    }

    if (_renderer != nullptr) {
        delete _renderer;
        _renderer = nullptr;
    }

    if (_audio != nullptr) {
        delete _audio;
        _audio = nullptr;
    }

    if (_camera != nullptr) {
        delete _camera;
        _camera = nullptr;
    }

    if (_objectManager != nullptr) {
        delete _objectManager;
        _objectManager = nullptr;
    }

    _destroyObjects();

    if (_timelineManager != nullptr) {
        delete _timelineManager;
        _timelineManager = nullptr;
    }

    if (_chronoManager != nullptr) {
        delete _chronoManager;
        _chronoManager = nullptr;
    }

    if (_settings != nullptr) {
        delete _settings;
        _settings = nullptr;
    }
}

void H2DE_Engine::_destroyObjects() {
    for (auto it = _objects.begin(); it != _objects.end(); ) {
        H2DE_Object* object = *it;

        if (object == nullptr) {
            ++it;
            continue;
        }
        
        delete object;
        object = nullptr;
        it = _objects.erase(it);
    }
}

void H2DE::destroyEngine(H2DE_Engine* engine) {
    if (engine != nullptr) {
        delete engine;
    }
}

// RUN
void H2DE_Engine::run() {
    if (_isRunning) {
        return;
    }

    _isRunning = true;

    try {
        Uint64 perfFreq = SDL_GetPerformanceFrequency();
        Uint64 lastTime = SDL_GetPerformanceCounter();
        Uint64 lastSec = lastTime;

        SDL_Event event;

        SDL_StartTextInput();

        while (_isRunning) {
            Uint64 now = SDL_GetPerformanceCounter();
            _deltaTime = (now - lastTime) / static_cast<float>(perfFreq);
            _currentFPS = (_deltaTime > 0.0f) ? 1.0f / _deltaTime : 0.0f;

            _handleEvents(event);

            if (!_debugModeEnabled) {
                _update();
                _renderer->render();
            }

            const float targetFrameTime = 1.0f / _fps;
            while ((SDL_GetPerformanceCounter() - now) / static_cast<float>(perfFreq) < targetFrameTime) {
                // Spin-wait
            }

            if ((now - lastSec) / static_cast<float>(perfFreq) >= 1.0f) {
                lastSec = now;
            }

            if (_debugModeEnabled) {
                _audio->_pause();
            }

            lastTime = now;
        }

        SDL_StopTextInput();

    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

// EVENTS
void H2DE_Engine::_handleEvents(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            
            case SDL_QUIT:
                _isRunning = false;
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    _window->setSize(H2DE_PixelSize{ event.window.data1, event.window.data2 });
                    _camera->_updateGrid();
                }
                break;

            case SDL_MOUSEMOTION:
                _mousePos = { event.button.x, event.button.y };
                break;

            default: break;
        }

        if (_handleEventsCall) {
            _handleEventsCall(event);
        }

        _objectManager->handleEvents(event);
    }
}

// UPDATE
void H2DE_Engine::_update() {
    _window->_update();

    _assetLoaderManager->update();

    _timelineManager->update();
    _chronoManager->update();
    _audio->_update();

    if (!_paused) {
        if (_updateCall) {
            _updateCall();
        }
        
        _updateObjects();
        _camera->_update();
    }
}

void H2DE_Engine::_updateObjects() {
    for (H2DE_Object* object : _objects) {
        object->_update();
    }
}

// ACTIONS

// -- assets
void H2DE_Engine::loadAssetsSync(const std::string& directory, bool silentLoad) {
    _assetLoaderManager->silentLoad = silentLoad;
    _assetLoaderManager->loadAssets(directory, nullptr, nullptr, true);
}

void H2DE_Engine::loadAssetsAsync(const std::string& directory, const std::function<void(float)>& progress, const std::function<void()>& completed, bool silentLoad) {
    _assetLoaderManager->silentLoad = silentLoad;
    _assetLoaderManager->loadAssets(directory, progress, completed, false);
}

void H2DE_Engine::loadFont(const std::string& name, const H2DE_Font& font, bool silentLoad) {
    _assetLoaderManager->silentLoad = silentLoad;
    _assetLoaderManager->loadFont(name, font);
}

// -- debug mode
void H2DE_Engine::debugModeNextFrame() {
    if (!_debugModeEnabled) {
        return;
    }

    _audio->_resume();
    _update();
    _renderer->render();
}

void H2DE_Engine::debugModePreviousFrame() {
    if (!_debugModeEnabled) {
        return;
    }

    H2DE_Error::logWarning("Feature not implemented");
}

// -- timeline
H2DE_Timeline* H2DE_Engine::createTimeline(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive) {
    return _timelineManager->create(duration, easing, update, completed, loops, pauseSensitive);
}

// -- delay
H2DE_Delay* H2DE_Engine::createDelay(uint32_t duration, const std::function<void()>& callback, bool pauseSensitive) {
    return _timelineManager->create(duration, H2DE_EASING_LINEAR, nullptr, callback, 1, pauseSensitive);
}

// -- chrono
H2DE_Chrono* H2DE_Engine::createChrono(const H2DE_Time& start, bool increasing, bool pauseSensitive) {
    return _chronoManager->create(start, increasing, pauseSensitive);
}

// -- objects
void H2DE_Engine::_refreshObjectManagerButtons() {
    _objectManager->refreshButtonBuffer(_objects);
}

void H2DE_Engine::_refreshObjectManagerInputs() {
    _objectManager->refreshInputBuffer(_objects);
}

void H2DE_Engine::debugHitboxes(bool state, const std::vector<int>& collisionIndexes) {
    _debugObjectEnabled = state;
    _debugHitboxCollisionIndexes = collisionIndexes;
}

bool H2DE_Engine::destroyObject(H2DE_Object* object) {
    auto it = std::find(_objects.begin(), _objects.end(), object);

    if (it == _objects.end()) {
        return false;
    }

    bool isButton = (dynamic_cast<H2DE_ButtonObject*>(object) != H2DE_NULL_OBJECT);

    _objects.erase(it);
    
    delete object;
    
    if (isButton) {
        _objectManager->refreshButtonBuffer(_objects);
    }

    return true;
}

// -- mouse
const H2DE_Translate H2DE_Engine::_getMousePos(bool absolute) const {
    return _renderer->pixelToLevel(_mousePos, absolute);
}

// GETTER
bool H2DE_Engine::_isPositionGreater(H2DE_Object* a, H2DE_Object* b) {
    const H2DE_Translate aTranslate = a->getTranslate();
    const H2DE_Translate bTranslate = b->getTranslate();

    bool equalsX = (aTranslate.x == bTranslate.x);

    return (equalsX)
        ? (aTranslate.y < bTranslate.y)
        : (aTranslate.x < bTranslate.x);
}

uint32_t H2DE_Engine::getObjectsRenderedNumber() const noexcept {
    return _renderer->objectsRendered;
}

uint32_t H2DE_Engine::getSurfacesRenderedNumber() const noexcept {
    return _renderer->surfacesRendered;
}

uint32_t H2DE_Engine::getHitboxesRenderedNumber() const noexcept {
    return _renderer->hitboxesRendered;
}

const H2DE_PixelSize H2DE_Engine::getTextureSize(const std::string& textureName) const {
    SDL_Texture* texture = _renderer->getTexture(textureName);
    if (texture == nullptr) {
        H2DE_Error::logError("Can't find texture named \"" + textureName + "\"");
    }

    int width = 0;
    int height = 0;

    if (SDL_QueryTexture(texture, nullptr, nullptr, &width, &height) != 0) {
        H2DE_Error::logError("Failed to query texture named \"" + textureName + "\"");
    } 

    return { width, height };
}
