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
#include <H2DE/H2DE_object.h>
class H2DE_LevelObject;
class H2DE_InterfaceObject;
class H2DE_InterfaceObjectBar;
class H2DE_InterfaceObjectButton;
class H2DE_InterfaceObjectImage;
class H2DE_InterfaceObjectText;

class H2DE_Engine {
private:
    H2DE_EngineData data;

    class H2DE_Window;
    class H2DE_Renderer;
    class H2DE_AssetLoader;
    class H2DE_Settings;
    class H2DE_Camera;

    H2DE_Window* window = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_AssetLoader* assetLoader = nullptr;
    H2DE_Settings* settings = nullptr;
    H2DE_Camera* camera = nullptr;

    unsigned int fps;
    unsigned int currentFPS = 0;
    bool isRunning = true;
    bool paused = false;

    std::function<void(SDL_Event)> handleEvents = NULL;
    std::function<void()> update = NULL;

    std::vector<H2DE_LevelObject*> levelObjects = {};
    std::vector<H2DE_InterfaceObject*> interfaceObjects = {};

    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

    void addLevelObject(H2DE_LevelObject* obj);
    void destroyLevelObject(H2DE_LevelObject* obj);
    void addInterfaceObject(H2DE_InterfaceObject* obj);
    void destroyInterfaceObject(H2DE_InterfaceObject* obj);

public:
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    friend void H2DE_RunEngine(H2DE_Engine* engine);
    friend void H2DE_StopEngine(H2DE_Engine* engine);

    friend void H2DE_SetHandleEventsCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call);
    friend void H2DE_SetUpdateCall(H2DE_Engine* engine, std::function<void()> call);

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);

    friend unsigned int H2DE_GetCurrentFps(H2DE_Engine* engine);
    friend unsigned int H2DE_GetFps(H2DE_Engine* engine);
    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);

    friend void H2DE_Pause(H2DE_Engine* engine);
    friend void H2DE_Resume(H2DE_Engine* engine);
    friend bool H2DE_IsPaused(H2DE_Engine* engine);

    friend H2DE_AbsPos H2DE_GetWindowPos(H2DE_Engine* engine);
    friend H2DE_AbsSize H2DE_GetWindowSize(H2DE_Engine* engine);
    friend bool H2DE_IsWindowFullscreen(H2DE_Engine* engine);
    friend bool H2DE_IsWindowResizable(H2DE_Engine* engine);

    friend bool H2DE_SettingsAddSection(H2DE_Engine* engine, const std::string& section);
    friend bool H2DE_SettingsAddKey(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend bool H2DE_SettingsSetKeyValue(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend int H2DE_SettingsGetKeyInteger(H2DE_Engine* engine, const std::string& section, const std::string& key, const int& defaultValue);
    friend std::string H2DE_SettingsGetKeyString(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& defaultValue);
    friend bool H2DE_SettingsGetKeyBool(H2DE_Engine* engine, const std::string& section, const std::string& key, const bool& defaultValue);

    friend H2DE_LevelPos H2DE_GetCameraPos(H2DE_Engine* engine);
    friend H2DE_LevelSize H2DE_GetCameraSize(H2DE_Engine* engine);
    friend bool H2DE_CameraContains(H2DE_Engine* engine, const H2DE_Hitbox& hitbox);
    friend void H2DE_SetCameraPos(H2DE_Engine* engine, const H2DE_LevelPos& pos);
    friend void H2DE_SetCameraWidth(H2DE_Engine* engine, const float& width);
    friend void H2DE_SetCameraSmoothing(H2DE_Engine* engine, const float& smoothing);
    friend void H2DE_SetCameraReference(H2DE_Engine* engine, H2DE_LevelObject* object);
    friend void H2DE_SetCameraLockedToReference(H2DE_Engine* engine, const bool& state);
    friend void H2DE_SetCameraPadding(H2DE_Engine* engine, const H2DE_LevelPadding& padding);

    friend H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_LevelObjectData& lod);
    friend H2DE_InterfaceObjectBar* H2DE_CreateInterfaceObjectBar(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectBarData& iod);
    friend H2DE_InterfaceObjectButton* H2DE_CreateInterfaceObjectButton(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectButtonData& iod);
    friend H2DE_InterfaceObjectImage* H2DE_CreateInterfaceObjectImage(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectImageData& iod);
    friend H2DE_InterfaceObjectText* H2DE_CreateInterfaceObjectText(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectTextData& iod);
    friend void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* obj);
    friend void H2DE_DestroyInterfaceObject(H2DE_Engine* engine, H2DE_InterfaceObject* obj);
};

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
