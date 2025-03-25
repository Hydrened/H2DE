#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <filesystem>
#include <functional>
#include <future>
#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <H2DE/H2DE_utils.h>
#include <H2DE/H2DE_surface.h>
#include <H2DE/H2DE_object.h>
class H2DE_BarObject;
class H2DE_ButtonObject;
class H2DE_BasicObject;
class H2DE_TextObject;
class H2DE_Sprite;

class H2DE_Engine {
private:
    H2DE_EngineData data;

    class H2DE_Window;
    class H2DE_Renderer;
    class H2DE_AssetLoader;
    class H2DE_Settings;
    class H2DE_Camera;
    class H2DE_DelayManager;

    H2DE_Window* window = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_AssetLoader* assetLoader = nullptr;
    H2DE_Settings* settings = nullptr;
    H2DE_Camera* camera = nullptr;
    H2DE_DelayManager* delayManager = nullptr;

    unsigned int fps;
    unsigned int currentFPS = 0;
    bool isRunning = true;
    bool paused = false;

    std::function<void(SDL_Event)> handleEvents = nullptr;
    std::function<void()> update = nullptr;

    std::vector<H2DE_Object*> objects = {};

    H2DE_AbsPos mousePos = { 0, 0 };
    std::optional<H2DE_AbsPos> click = std::nullopt;

    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

    void updateObjects();

public:
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    friend void H2DE_RunEngine(H2DE_Engine* engine);
    friend void H2DE_StopEngine(H2DE_Engine* engine);

    friend void H2DE_SetHandleEventsCall(H2DE_Engine* engine, const std::function<void(SDL_Event)>& call);
    friend void H2DE_SetUpdateCall(H2DE_Engine* engine, const std::function<void()>& call);

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);

    friend unsigned int H2DE_GetCurrentFps(const H2DE_Engine* engine);
    friend unsigned int H2DE_GetFps(const H2DE_Engine* engine);
    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);

    friend bool H2DE_IsPaused(const H2DE_Engine* engine);
    friend void H2DE_Pause(H2DE_Engine* engine);
    friend void H2DE_Resume(H2DE_Engine* engine);

    friend H2DE_AbsPos H2DE_GetWindowPos(const H2DE_Engine* engine);
    friend H2DE_AbsSize H2DE_GetWindowSize(const H2DE_Engine* engine);
    friend bool H2DE_IsWindowFullscreen(const H2DE_Engine* engine);
    friend bool H2DE_IsWindowResizable(const H2DE_Engine* engine);
    friend void H2DE_SetWindowPos(const H2DE_Engine* engine, const H2DE_AbsPos& pos);
    friend void H2DE_SetWindowSize(const H2DE_Engine* engine, const H2DE_AbsSize& size);
    friend void H2DE_SetWindowMinimumSize(const H2DE_Engine* engine, const H2DE_AbsSize& minimumSize);
    friend void H2DE_SetWindowMaximumSize(const H2DE_Engine* engine, const H2DE_AbsSize& maximumSize);

    friend bool H2DE_SettingsAddSection(const H2DE_Engine* engine, const std::string& section);
    friend bool H2DE_SettingsAddKey(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend int H2DE_SettingsGetKeyInteger(const H2DE_Engine* engine, const std::string& section, const std::string& key, int defaultValue);
    friend std::string H2DE_SettingsGetKeyString(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& defaultValue);
    friend bool H2DE_SettingsGetKeyBool(const H2DE_Engine* engine, const std::string& section, const std::string& key, bool defaultValue);
    friend bool H2DE_SettingsSetKeyValue(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);

    friend unsigned int H2DE_Delay(H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, int loop, bool pauseSensitive);
    friend void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id);
    friend void H2DE_StopDelay(H2DE_Engine* engine, unsigned int id, bool call);

    friend H2DE_LevelPos H2DE_GetCameraPos(const H2DE_Engine* engine);
    friend H2DE_LevelSize H2DE_GetCameraSize(const H2DE_Engine* engine);
    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, H2DE_Object* object);
    friend bool H2DE_CameraContainsHitbox(const H2DE_Engine* engine, const H2DE_Hitbox& hitbox, bool absolute);
    friend void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos);
    friend void H2DE_SetCameraWidth(const H2DE_Engine* engine, float width);
    friend void H2DE_SetCameraSmoothing(const H2DE_Engine* engine, float smoothing);
    friend void H2DE_SetCameraReference(const H2DE_Engine* engine, H2DE_Object* object);
    friend void H2DE_SetCameraLockedToReference(const H2DE_Engine* engine, bool state);
    friend void H2DE_SetCameraPadding(const H2DE_Engine* engine, const H2DE_LevelPadding& padding);

    friend H2DE_LevelPos H2DE_GetMousePos(const H2DE_Engine* engine, bool absolute);

    friend class H2DE_Object;
    friend class H2DE_ButtonObject;
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    friend H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);
};

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
