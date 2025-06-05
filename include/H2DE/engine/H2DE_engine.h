#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <vector>
#include <windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>

#include <H2DE/utils/H2DE_utils.h>
#include <H2DE/engine/H2DE_settings.h>
#include <H2DE/engine/H2DE_window.h>
#include <H2DE/engine/H2DE_asset_loader_manager.h>
#include <H2DE/engine/H2DE_renderer.h>
#include <H2DE/engine/H2DE_volume.h>
#include <H2DE/engine/H2DE_timeline_manager.h>
#include <H2DE/engine/H2DE_camera.h>
#include <H2DE/engine/H2DE_object_manager.h>
#include <H2DE/surfaces/H2DE_surface.h>
#include <H2DE/objects/H2DE_object.h>

class H2DE_Settings;
class H2DE_Window;
class H2DE_AssetLoaderManager;
class H2DE_Renderer;
class H2DE_Volume;
class H2DE_TimelineManager;
class H2DE_Camera;
class H2DE_ObjectManager;

class H2DE_Engine {
private:
    H2DE_EngineData data;

    H2DE_Settings* settings = nullptr;
    H2DE_Window* window = nullptr;
    H2DE_AssetLoaderManager* assetLoaderManager = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_Volume* volume = nullptr;
    H2DE_TimelineManager* timelineManager = nullptr;
    H2DE_Camera* camera = nullptr;
    H2DE_ObjectManager* objectManager = nullptr;

    uint16_t fps = 0;
    float currentFPS = 0;
    float deltaTime = 0.0f;
    bool isRunning = false;
    bool paused = false;

    bool debugModeEnabled = false;
    bool debugObjectEnabled = false;

    std::function<void(SDL_Event)> handleEventsCall = nullptr;
    std::function<void()> updateCall = nullptr;

    std::unordered_map<std::string, H2DE_Font> fonts = {};
    std::vector<H2DE_Object*> objects = {};

    H2DE_PixelPos mousePos = { 0, 0 };

    H2DE_Engine(const H2DE_EngineData& data);
    ~H2DE_Engine();

    void handleEvents(SDL_Event event);
    void update();
    void updateObjects();

    void refreshObjectManager();

    void destroy();
    void destroyObjects();

    const H2DE_Translate getMousePos(bool absolute) const;

    static bool isPositionGreater(H2DE_Object* a, H2DE_Object* b);

public:
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    void run();

    void loadAssets(const std::string& directory);
    void loadFont(const std::string& name, const H2DE_Font& font);

    void debugMode(bool state);
    inline void toggleDebugMode() { debugMode(!debugModeEnabled); }

    inline void debugObjects(bool state) { debugObjectEnabled = state; }
    inline void toggleDebugObject() { debugObjectEnabled = !debugObjectEnabled; }

    void debugModeNextFrame();
    void debugModePreviousFrame();

    inline void pause() { paused = true; }
    inline void resume() { paused = false; }
    inline void togglePause() { paused = !paused; }

    H2DE_TimelineID createTimeline(H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);
    void pauseTimeline(H2DE_TimelineID id);
    void resumeTimeline(H2DE_TimelineID id);
    void togglePauseTimeline(H2DE_TimelineID id);
    void resetTimeline(H2DE_TimelineID id);
    void stopTimeline(H2DE_TimelineID id, bool callCompleted);
    bool isTimelinePaused(H2DE_TimelineID id) const;

    H2DE_TimelineID delay(H2DE_TimelineID duration, const std::function<void()>& callback, bool pauseSensitive = true);
    inline void pauseDelay(H2DE_TimelineID id) { pauseTimeline(id); }
    inline void resumeDelay(H2DE_TimelineID id) { resumeTimeline(id); }
    inline void togglePauseDelay(H2DE_TimelineID id) { togglePauseTimeline(id); }
    inline void resetDelay(H2DE_TimelineID id) { resetTimeline(id); }
    inline void stopDelay(H2DE_TimelineID id,  bool callCompleted) { stopTimeline(id, callCompleted); }
    inline bool isDelayPaused(H2DE_TimelineID id) const { return isTimelinePaused(id); }

    template<typename H2DE_Object_T>
    H2DE_Object_T* createObject(const H2DE_ObjectData& objectData) {
        H2DE_Object_T* object = new H2DE_Object_T(this, objectData);
        objects.push_back(object);
        return object;
    }
    template<typename H2DE_Object_T>
    H2DE_Object_T* createObject(const H2DE_ObjectData& objectData, const typename H2DE_Object_T::H2DE_DataType& specificObjectData) {
        H2DE_Object_T* object = new H2DE_Object_T(this, objectData, specificObjectData);
        objects.push_back(object);

        if constexpr (std::is_same_v<H2DE_Object_T, H2DE_ButtonObject>) {
            refreshObjectManager();
        }

        return object;
    }
    bool destroyObject(H2DE_Object* object);

    inline H2DE_EngineData getData() const { return data; }
    inline H2DE_Settings* getSettings() const { return settings; }
    inline H2DE_Window* getWindow() const { return window; }
    inline H2DE_Volume* getVolume() const { return volume; }
    inline H2DE_Camera* getCamera() const { return camera; }
    inline uint16_t getFPS() const { return fps; }
    inline float getCurrentFPS(bool round = true) const { return (round) ? std::round(currentFPS) : currentFPS; }
    inline float getDeltaTime() const { return deltaTime; }
    inline bool isPaused() const { return paused; }

    uint32_t getObjectsRenderedNumber() const;
    uint32_t getSurfacesRenderedNumber() const;
    uint32_t getHitboxesRenderedNumber() const;

    inline const H2DE_Translate getMouseGamePos() const { return getMousePos(false); }
    inline const H2DE_Translate getMouseInterfacePos() const { return getMousePos(true); }

    inline void setFPS(uint16_t FPS) { fps = FPS; }
    inline void setHandleEventCall(const std::function<void(SDL_Event)>& call) { handleEventsCall = call; }
    inline void setUpdateCall(const std::function<void()>& call) { updateCall = call; }

    friend class H2DE_Window;
    friend class H2DE_AssetLoaderManager;
    friend class H2DE_Renderer;
    friend class H2DE_Volume;
    friend class H2DE_Camera;
    friend class H2DE_ObjectManager;
    friend class H2DE_Object;
    friend class H2DE_TextObject;
};

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
