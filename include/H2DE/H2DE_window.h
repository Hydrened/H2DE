#ifndef H2DE_WINDOW_H
#define H2DE_WINDOW_H

#include <H2DE_engine.h>
#include <cmath>
#include <algorithm>
class H2DE_Engine;

class H2DE_Window {
private:
    H2DE_Engine* engine;
    SDL_Window* window;
    SDL_Renderer* renderer;
    H2DE_WindowData data;

    void initSDL();
    void quitSDL();
    void createWindow();
    void loadData();
    void saveData();

public:
    H2DE_Window(H2DE_Engine* engine, H2DE_WindowData data);
    ~H2DE_Window();

    /**
     * Gets the window's renderer
     * \param window pointer to a window
     * \returns the window's renderer
     * \since H2DE-2.0.3
     */
    friend SDL_Renderer* H2DE_GetWindowsRenderer(H2DE_Window* window);
    /**
     * Gets the window's size
     * \param window pointer to a window
     * \returns the window's size
     * \since H2DE-2.0.1
     */
    friend H2DE_AbsSize H2DE_GetWindowSize(H2DE_Window* window);
    /**
     * Gets the window's minimum size
     * \param window pointer to a window
     * \returns the window's minimum size
     * \since H2DE-2.0.1
     */
    friend H2DE_AbsSize H2DE_GetWindowMinimumSize(H2DE_Window* window);
    /**
     * Gets the window's maximum size
     * \param window pointer to a window
     * \returns the window's maximum size
     * \since H2DE-2.0.1
     */
    friend H2DE_AbsSize H2DE_GetWindowMaximumSize(H2DE_Window* window);

    /**
     * Sets the window's title
     * \param window pointer to a window
     * \param title new title of the window
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetWindowTitle(H2DE_Window* window, std::string title);
    /**
     * Sets the window's size
     * \param window pointer to a window
     * \param size new size of the window
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetWindowSize(H2DE_Window* window, H2DE_AbsSize size);
    /**
     * Sets the window's minimum size
     * \param window pointer to a window
     * \param minSize new minimum size of the window
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetWindowMinimumSize(H2DE_Window* window, H2DE_AbsSize minSize);
    /**
     * Sets the window's maximum size
     * \param window pointer to a window
     * \param minSize new maximum size of the window
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetWindowMaximumSize(H2DE_Window* window, H2DE_AbsSize maxSize);
    /**
     * Sets the window's fullscreen state
     * \param window pointer to a window
     * \param state boolean
     * \since H2DE-2.0.2
     */
    friend void H2DE_SetWindowFullscreen(H2DE_Window* window, bool state);
    /**
     * Sets the window's grad state
     * \param window pointer to a window
     * \param state boolean
     * \since H2DE-2.0.2
     */
    friend void H2DE_SetWindowGrab(H2DE_Window* window, bool state);
    /**
     * Sets the window's resizable state
     * \param window pointer to a window
     * \param state boolean
     * \since H2DE-2.0.2
     */
    friend void H2DE_SetWindowResizable(H2DE_Window* window, bool state);
};

#endif
