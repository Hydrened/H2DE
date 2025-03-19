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
#include <H2DE/H2DE_level_object.h>
class H2DE_LevelObject;

class H2DE_Engine {
private:
    H2DE_EngineData data;

    class H2DE_Window;
    class H2DE_Renderer;
    class H2DE_AssetLoader;
    class H2DE_Settings;

    H2DE_Window* window = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_AssetLoader* assetLoader = nullptr;
    H2DE_Settings* settings = nullptr;

    int fps;
    int currentFPS = 0;
    bool isRunning = true;
    bool paused = false;

    std::function<void(SDL_Event)> handleEvents = NULL;
    std::function<void()> update = NULL;

    std::vector<H2DE_LevelObject*> objects = {};

    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

public:
    friend H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    friend void H2DE_RunEngine(H2DE_Engine* engine);
    friend void H2DE_StopEngine(H2DE_Engine* engine);

    friend void H2DE_SetHandleEventsCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call);
    friend void H2DE_SetUpdateCall(H2DE_Engine* engine, std::function<void()> call);

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);

    friend inline int H2DE_GetCurrentFps(H2DE_Engine* engine);
    friend inline void H2DE_SetFPS(H2DE_Engine* engine, unsigned int fps);

    friend inline void H2DE_Pause(H2DE_Engine* engine);
    friend inline void H2DE_Resume(H2DE_Engine* engine);
    friend inline bool H2DE_IsPaused(H2DE_Engine* engine);

    friend H2DE_AbsPos H2DE_GetWindowPos(H2DE_Engine* engine);
    friend H2DE_AbsSize H2DE_GetWindowSize(H2DE_Engine* engine);
    friend bool H2DE_IsWindowFullscreen(H2DE_Engine* engine);
    friend bool H2DE_IsWindowResizable(H2DE_Engine* engine);

    friend bool H2DE_SettingsAddSection(H2DE_Engine* engine, const std::string& section);
    friend bool H2DE_SettingsAddKey(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend bool H2DE_SettingsSetKeyValue(H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    friend int H2DE_SettingsGetKeyInteger(H2DE_Engine* engine, const std::string& section, const std::string& key, int defaultValue);
};

H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
