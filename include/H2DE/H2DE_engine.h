#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <map>
#include <vector>
#include <windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>

#include <H2DE/H2DE_utils.h>
#include <H2DE/H2DE_settings.h>
#include <H2DE/H2DE_window.h>
#include <H2DE/H2DE_asset_loader_manager.h>
#include <H2DE/H2DE_renderer.h>
#include <H2DE/H2DE_volume.h>
#include <H2DE/H2DE_timeline_manager.h>
#include <H2DE/H2DE_camera.h>
#include <H2DE/surfaces/H2DE_surface.h>
#include <H2DE/objects/H2DE_object.h>

class H2DE_Settings;
class H2DE_Window;
class H2DE_AssetLoaderManager;
class H2DE_Renderer;
class H2DE_Volume;
class H2DE_TimelineManager;
class H2DE_Camera;
class H2DE_BarObject;
class H2DE_BasicObject;
class H2DE_ButtonObject;
class H2DE_TextObject;

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

    unsigned int fps = 0;
    unsigned int currentFPS = 0;
    float deltaTime = 0.0f;
    bool isRunning = false;
    bool paused = false;

    bool debugModeEnabled = false;
    bool debugObjectEnabled = false;

    std::function<void(SDL_Event)> handleEventsCall = nullptr;
    std::function<void()> updateCall = nullptr;

    std::vector<H2DE_Object*> objects = {};

    H2DE_Engine(const H2DE_EngineData& data);
    ~H2DE_Engine();

    void handleEvents(SDL_Event event);
    void update();
    void updateObjects();

    void destroy();
    void destroyObjects();

    static bool isPositionGreater(H2DE_Object* a, H2DE_Object* b);

public:
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    void run();

    void loadAssets(const std::string& directory);

    void debugMode(bool state);
    inline void toggleDebugMode() { debugMode(!debugModeEnabled); }
    inline void debugObjects(bool state) { debugObjectEnabled = state; }
    inline void toggleDebugObject() { debugObjectEnabled = !debugObjectEnabled; }

    void debugModeNextFrame();
    void debugModePreviousFrame();

    inline void pause() { paused = true; }
    inline void resume() { paused = false; }
    inline void togglePause() { paused = !paused; }

    unsigned int createTimeline(unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive);
    void resetTimeline(unsigned int id);
    void stopTimeline(unsigned int id, bool callCompleted);

    unsigned int delay(unsigned int duration, const std::function<void()>& callback, bool pauseSensitive);
    void resetDelay(unsigned int id);
    void stopDelay(unsigned int id,  bool callCompleted);

    H2DE_BarObject* createBarObject(const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    H2DE_BasicObject* createBasicObject(const H2DE_ObjectData& objectData);
    H2DE_ButtonObject* createButtonObject(const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    H2DE_TextObject* createTextObject(const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    bool destroyObject(H2DE_Object* object);

    inline H2DE_EngineData getData() const { return data; }
    inline H2DE_Settings* getSettings() const { return settings; }
    inline H2DE_Window* getWindow() const { return window; }
    inline H2DE_Volume* getVolume() const { return volume; }
    inline H2DE_Camera* getCamera() const { return camera; }
    inline unsigned int getFPS() const { return fps; }
    inline unsigned int getCurrentFPS() const { return currentFPS; }
    inline float getDeltaTime() const { return deltaTime; }
    inline bool isPaused() const { return paused; }

    inline void setFPS(unsigned int FPS) { fps = FPS; }
    inline void setHandleEventCall(const std::function<void(SDL_Event)>& call) { handleEventsCall = call; }
    inline void setUpdateCall(const std::function<void()>& call) { updateCall = call; }

    friend class H2DE_Window;
    friend class H2DE_AssetLoaderManager;
    friend class H2DE_Renderer;
    friend class H2DE_Volume;
    friend class H2DE_Camera;
    friend class H2DE_Object;
};

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
