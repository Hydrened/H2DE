#ifndef H2DE_WINDOW_H
#define H2DE_WINDOW_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Window {
public:
    inline H2DE_WindowData getData() const { return data; }
    H2DE_PixelPos getPos() const;
    H2DE_PixelSize getSize() const;

    void setPos(const H2DE_PixelPos& pos);
    void setSize(const H2DE_PixelSize& size);
    void setMinimumSize(const H2DE_PixelSize& minimumSize);
    void setMaximumSize(const H2DE_PixelSize& maximumSize);
    void setTitle(const std::string& title);
    void setIcon(const std::string& textureName);
    void setFullscreen(const H2DE_Engine* engine, bool fullscreen);
    void setResizable(const H2DE_Engine* engine, bool resizable);
    void setGrab(const H2DE_Engine* engine, bool grab);
    void setRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio);
    
    friend class H2DE_Engine;

private:
    H2DE_Engine* engine;
    H2DE_WindowData data;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    H2DE_PixelSize oldSize = { 0, 0 };
    float customRatio = 0.0f;

    H2DE_Window(H2DE_Engine* engine, const H2DE_WindowData& data);
    ~H2DE_Window();

    void initSDL() const;
    void initSettings() const;
    void create();

    void saveState() const;
    void quitSDL() const;

    void update();

    void fixRatioSize(const H2DE_PixelSize& size);

    inline constexpr const SDL_WindowFlags getFlags(bool fullscreen, bool resizable) {
        return (fullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : (resizable) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_SHOWN;
    }
};

#endif
