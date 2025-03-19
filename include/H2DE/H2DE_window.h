#ifndef H2DE_WINDOW_H
#define H2DE_WINDOW_H

#include <string>
#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Window {
private:
    H2DE_Engine* engine;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    H2DE_WindowData data;

    void initSDL();
    void create();
    void initSettings();

    void quitSDL();

public:
    H2DE_Window(H2DE_Engine* engine, H2DE_WindowData data);
    ~H2DE_Window();

    void saveState();

    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;
};

#endif
