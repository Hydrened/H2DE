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

    H2DE_AbsSize oldSize = { 0, 0 };
    float customRatio = 0.0f;

    void initSDL() const;
    void create();
    void initSettings() const;
    void quitSDL() const;
    SDL_WindowFlags getFlags(bool fullscreen, bool resizable) const;

public:
    H2DE_Window(H2DE_Engine* engine, H2DE_WindowData data);
    ~H2DE_Window();

    void update();

    void saveState() const;
    void fixRatioSize(const H2DE_AbsSize& size);

    friend H2DE_AbsPos H2DE_GetWindowPos(const H2DE_Engine* engine);
    friend H2DE_AbsSize H2DE_GetWindowSize(const H2DE_Engine* engine);

    friend void H2DE_SetWindowPos(const H2DE_Engine* engine, const H2DE_AbsPos& pos);
    friend void H2DE_SetWindowSize(const H2DE_Engine* engine, const H2DE_AbsSize& size);
    friend void H2DE_SetWindowMinimumSize(const H2DE_Engine* engine, const H2DE_AbsSize& minimumSize);
    friend void H2DE_SetWindowMaximumSize(const H2DE_Engine* engine, const H2DE_AbsSize& maximumSize);
    friend void H2DE_SetWindowTitle(const H2DE_Engine* engine, const std::string& title);
    friend void H2DE_SetWindowIcon(const H2DE_Engine* engine, const std::string& textureName);
    friend void H2DE_SetWindowFullscreen(const H2DE_Engine* engine, bool fullscreen);
    friend void H2DE_SetWindowResizable(const H2DE_Engine* engine, bool resizable);
    friend void H2DE_SetWindowGrab(const H2DE_Engine* engine, bool grab);
    friend void H2DE_SetWindowRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio);

    friend class H2DE_Engine;
};

#endif
