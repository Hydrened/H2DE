#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <SDL2/SDL.h>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <windows.h>
#include "H2DE_utils.h"
#include "H2DE_asset_loader.h"
#include "H2DE_window.h"
#include "H2DE_camera.h"
class H2DE_Window;
class H2DE_Camera;

class H2DE_Engine {
private:
    H2DE_Window* window = nullptr;
    H2DE_Camera* camera = nullptr;

    H2DE_EngineData data;
    unsigned int fps;
    bool isRunning = true;
    int currentFPS = 0;
    
    int loadedData = 0;
    int dataToLoad = 0;

    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Chunk*> sounds;

    std::function<void(SDL_Event)> handleEvents = NULL;
    std::function<void()> update = NULL;
    std::function<void()> render = NULL;

    static int countFilesToLoad(const std::filesystem::path& dir);
    void importFiles(const std::filesystem::path& dir);
    void importFile(const std::filesystem::path& file);
    void importTexture(const std::filesystem::path& img);
    void importSound(const std::filesystem::path& song);
    void assetImported();

public:
    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

    friend void H2DE_RunEngine(H2DE_Engine* engine);

    static void H2DE_Delay(unsigned int ms, std::function<void()> callback);

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& dir);
    friend void H2DE_LoadAsset(H2DE_Engine* engine, const std::filesystem::path& file);
    friend void H2DE_RemoveAssets(H2DE_Engine* engine);
    friend void H2DE_RemoveAsset(H2DE_Engine* engine, const std::filesystem::path& file);

    friend int H2DE_PlaySound(H2DE_Engine* engine, int channel, std::string sound, int loop);
    friend void H2DE_PauseSound(H2DE_Engine* engine, int channel);
    friend void H2DE_ResumeSound(H2DE_Engine* engine, int channel);

    friend H2DE_Window* H2DE_GetWindow(H2DE_Engine* engine);
    friend int H2DE_GetFps(H2DE_Engine* engine);
    friend int H2DE_GetCurrentFps(H2DE_Engine* engine);

    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);
    friend void H2DE_SetHandleEventCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call);
    friend void H2DE_SetUpdateCall(H2DE_Engine* engine, std::function<void()> call);
    friend void H2DE_SetRenderCall(H2DE_Engine* engine, std::function<void()> call);
};

extern H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data);
extern void H2DE_DestroyEngine(H2DE_Engine* engine);
extern void H2DE_SetVolumeSound(int channel, int volume);

#endif
