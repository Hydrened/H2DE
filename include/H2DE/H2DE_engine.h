#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <SDL2/SDL.h>
#include <iostream>
#include <windows.h>
#include "H2DE_utils.h"

class H2DE_Engine {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    H2DE_EngineData* data;
    unsigned int fps;
    bool isRunning = true;
    int currentFPS = 0;

    void run();

public:
    H2DE_Engine(H2DE_EngineData* data);
    ~H2DE_Engine();

    friend void H2DE_LoadAssets(H2DE_Engine* engine, std::filesystem::path dir);
    friend void H2DE_LoadAsset(H2DE_Engine* engine, std::filesystem::path file);

    friend void H2DE_SetTitle(H2DE_Engine* engine, std::string title);
    friend void H2DE_SetSize(H2DE_Engine* engine, H2DE_2DAVector size);
    friend void H2DE_SetMinimumSize(H2DE_Engine* engine, H2DE_2DAVector minSize);
    friend void H2DE_SetMaximumSize(H2DE_Engine* engine, H2DE_2DAVector maxSize);
    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);
};

extern void H2DE_CreateEngine(H2DE_EngineData* data);
extern void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
