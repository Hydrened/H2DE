#pragma once

/**
 * @file H2DE_window.h
 * @brief Internal window management class used by the engine.
 * 
 * Handles the creation and control of the SDL window, including
 * size, position, fullscreen mode, resizability, mouse grabbing,
 * and aspect ratio enforcement.
 */

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

/**
 * @class H2DE_Window
 * @brief Internal class for managing the game window.
 * 
 * This class encapsulates all window-related operations such as
 * positioning, resizing, fullscreen switching, setting titles, icons, etc.
 */
class H2DE_Window {
public:
    /**
     * @brief Get the current internal window data.
     * 
     * Returns a copy of the internal H2DE_WindowData structure containing 
     * size, position, flags, and other attributes.
     * 
     * @return The current H2DE_WindowData.
     */
    constexpr H2DE_WindowData getData() const noexcept {
        return data; 
    }
    /**
     * @brief Get the current position of the window on screen.
     * 
     * @return The top-left pixel position of the window.
     */
    H2DE_PixelPos getPos() const;
    /**
     * @brief Get the current size of the window in pixels.
     * 
     * @return The pixel size (width x height) of the window.
     */
    H2DE_PixelSize getSize() const;

    /**
     * @brief Set the window's position on the screen.
     * 
     * @param pos Pixel position where the top-left corner of the window should be placed.
     */
    void setPos(const H2DE_PixelPos& pos);
    /**
     * @brief Set the window's size in pixels.
     * 
     * @param size The new width and height of the window.
     */
    void setSize(const H2DE_PixelSize& size);
    /**
     * @brief Set the minimum size the window can be resized to.
     * 
     * @param minimumSize Minimum allowed pixel dimensions.
     */
    void setMinimumSize(const H2DE_PixelSize& minimumSize);
    /**
     * @brief Set the maximum size the window can be resized to.
     * 
     * @param maximumSize Maximum allowed pixel dimensions.
     */
    void setMaximumSize(const H2DE_PixelSize& maximumSize);
    /**
     * @brief Set the window's title.
     * 
     * This updates the window title shown in the title bar.
     * 
     * @param title New title string to display.
     */
    void setTitle(const std::string& title);
    /**
     * @brief Set the window's icon.
     * 
     * Loads a texture from the engine's assets using the given name
     * and sets it as the window's icon.
     * 
     * @param textureName Name of the texture asset to use as an icon.
     */
    void setIcon(const std::string& textureName);
    /**
     * @brief Enable or disable fullscreen mode.
     * 
     * @param engine Pointer to the engine instance.
     * @param fullscreen If true, enables fullscreen. If false, returns to windowed mode.
     */
    void setFullscreen(const H2DE_Engine* engine, bool fullscreen);
    /**
     * @brief Enable or disable window resizability.
     * 
     * @param engine Pointer to the engine instance.
     * @param resizable If true, allows user to resize the window.
     */
    void setResizable(const H2DE_Engine* engine, bool resizable);
    /**
     * @brief Set mouse grab mode.
     * 
     * When enabled, the mouse is confined to the window, useful for games that require
     * absolute focus or raw input.
     * 
     * @param engine Pointer to the engine instance.
     * @param grab If true, enables mouse grabbing. If false, disables it.
     */
    void setGrab(const H2DE_Engine* engine, bool grab);
    /**
     * @brief Set the window's aspect ratio mode.
     * 
     * @param engine Pointer to the engine instance.
     * @param ratio The desired window aspect ratio setting.
     */
    void setRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio);
    /**
     * @brief Change the current mouse cursor.
     * 
     * Sets the system cursor to the one specified in the cursor parameter.
     * This affects the cursor appearance immediately in the game window.
     * 
     * @param cursor The desired cursor type, from the ::H2DE_Cursor enumeration.
     */
    void setCursor(H2DE_Cursor cursor);
    
    friend class H2DE_Engine;

private:
    H2DE_Engine* engine;
    H2DE_WindowData data;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_Cursor* cursor = nullptr;
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

    constexpr const SDL_WindowFlags getFlags(bool fullscreen, bool resizable) {
        return (fullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : (resizable) ? SDL_WINDOW_RESIZABLE : SDL_WINDOW_SHOWN;
    }
    static SDL_SystemCursor getSDLCursor(H2DE_Cursor cursor);
};
