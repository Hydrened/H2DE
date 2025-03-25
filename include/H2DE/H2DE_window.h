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

    void initSDL() const;
    void create();
    void initSettings() const;
    void quitSDL() const;

public:
    H2DE_Window(H2DE_Engine* engine, H2DE_WindowData data);
    ~H2DE_Window();

    void saveState() const;

    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;
};

#endif
