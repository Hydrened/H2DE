#include "H2DE_engine.h"

// INIT
H2DE_Engine::H2DE_Engine(H2DE_EngineData* d) : data(d), fps(data->fps) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("H2DE-101: Error initializing SDL => SDL_Init failed: " + std::string(SDL_GetError()));
    }

    SDL_WindowFlags windowFlag = (data->fullscreen) ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;
    window = SDL_CreateWindow(data->title.c_str(), data->pos.x, data->pos.y, data->size.x, data->size.y, windowFlag);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("H2DE-102: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("H2DE-103: Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }
    run();
}

void H2DE_CreateEngine(H2DE_EngineData* data) {
    try {
        H2DE_Engine* engine = new H2DE_Engine(data);
        delete engine;
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}





// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    delete engine;
}

// RUN
void H2DE_Engine::run() {
    Uint32 now = SDL_GetTicks();
    int frameTime;
    SDL_Event event;
    int timePerFrame = 1000 / fps;

    while (isRunning) {
        now = SDL_GetTicks();

        while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) isRunning = false;

        if (data->handleEvents) data->handleEvents(event);
        if (data->update) data->update();
        if (data->render) data->render();
        
        frameTime = SDL_GetTicks() - now;
        currentFPS = 1000.0f / static_cast<float>((frameTime > 0) ? frameTime : 1);
        if (timePerFrame >= frameTime) SDL_Delay(timePerFrame - frameTime);
    }
}




// LOAD
void H2DE_LoadAssets(H2DE_Engine* engine, std::filesystem::path dir) {

}

void H2DE_LoadAsset(H2DE_Engine* engine, std::filesystem::path file) {

}








// SETTER
void H2DE_SetTitle(H2DE_Engine* engine, std::string title) {
    SDL_SetWindowTitle(engine->window, title.c_str());
}

void H2DE_SetSize(H2DE_Engine* engine, H2DE_2DAVector size) {
    SDL_SetWindowSize(engine->window, size.x, size.y);
}

void H2DE_SetMinimumSize(H2DE_Engine* engine, H2DE_2DAVector minSize) {
    SDL_SetWindowMinimumSize(engine->window, minSize.x, minSize.y);
}

void H2DE_SetMaximumSize(H2DE_Engine* engine, H2DE_2DAVector maxSize) {
    SDL_SetWindowMaximumSize(engine->window, maxSize.x, maxSize.y);
}

void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps) {
    engine->fps = fps;
}
