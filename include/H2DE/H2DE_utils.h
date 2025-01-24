#ifndef H2DE_UTILS_H
#define H2DE_UTILS_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <filesystem>
#include <functional>
#include <string>

struct H2DE_2DAVector {
    int x;
    int y;
};

struct H2DE_2DLVector {
    float x;
    float y;
};

struct H2DE_EngineData {
    std::string title = "";
    H2DE_2DAVector pos = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
    H2DE_2DAVector size = { 640, 360 };
    unsigned int fps = 60;
    bool fullscreen = false;
    std::function<void(SDL_Event)> handleEvents = NULL;
    std::function<void()> update = NULL;
    std::function<void()> render = NULL;
};

#endif
