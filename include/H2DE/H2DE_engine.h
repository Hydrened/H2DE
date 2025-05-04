#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

/**
 * @file H2DE_engine.h
 * @brief Main engine header for the H2DE 2D engine.
 * 
 * This file includes all the essential components of the H2DE engine, 
 * such as the core engine class, surface management, object system, 
 * and audio/video handling via SDL2. It serves as the entry point to 
 * initialize and run the engine.
 */

#include <filesystem>
#include <functional>
#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <H2DE/H2DE_utils.h>
#include <H2DE/surfaces/H2DE_surface.h>
#include <H2DE/objects/H2DE_object.h>
#include <H2DE/H2DE_json.h>
class H2DE_BarObject;
class H2DE_ButtonObject;
class H2DE_BasicObject;
class H2DE_TextObject;
class H2DE_Sprite;

/**
 * @brief The core class of the H2DE engine.
 * 
 * This class encapsulates the entire state and functionality of the H2DE engine.
 * It handles the main loop, resource management, rendering, audio, windowing, input,
 * camera behavior, objects, timelines, delays, and settings.
 * 
 * Instances of this class should only be created and managed through the
 * `H2DE_CreateEngine` and `H2DE_DestroyEngine` functions.
 */
class H2DE_Engine {
private:
    H2DE_EngineData data;

    class H2DE_Window;
    class H2DE_Renderer;
    class H2DE_Volume;
    class H2DE_AssetLoader;
    class H2DE_Settings;
    class H2DE_Camera;
    class H2DE_Delay;
    class H2DE_Timeline;

    H2DE_Window* window = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_Volume* volume = nullptr;
    H2DE_AssetLoader* assetLoader = nullptr;
    H2DE_Settings* settings = nullptr;
    H2DE_Camera* camera = nullptr;
    H2DE_Delay* delay = nullptr;
    H2DE_Timeline* timeline = nullptr;

    unsigned int fps;
    unsigned int currentFPS = 0;
    bool isRunning = true;
    bool paused = false;

    bool debugMode = false;

    std::function<void(SDL_Event)> handleEventsCall = nullptr;
    std::function<void()> updateCall = nullptr;

    std::vector<H2DE_Object*> objects = {};

    H2DE_AbsPos mousePos = { 0, 0 };
    H2DE_ButtonObject* mouseDown = nullptr;
    H2DE_ButtonObject* hovered = nullptr;

    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

    void handleEvents(SDL_Event event);
    void handleButtonsEvents(SDL_Event event);
    void handleButtonsMouseDownEvent();
    void handleButtonsMouseUpEvent();
    void handleButtonsBlurEvents();
    void handleButtonsHoverEvents();
    void update();
    void updateObjects();

    static bool isPositionGreater(H2DE_Object* object1, H2DE_Object* object2);
    std::vector<H2DE_ButtonObject*> getValidButtons() const;

public:
    /**
     * @brief Creates and initializes an H2DE engine instance.
     * 
     * This function initializes the engine with the provided data, setting up all necessary
     * components like rendering, audio, input, and other engine subsystems.
     * 
     * @param data A struct containing configuration data to initialize the engine.
     * @return H2DE_Engine* A pointer to the newly created H2DE engine instance.
     */
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    /**
     * @brief Destroys the H2DE engine and frees all resources.
     * 
     * This function cleans up all engine subsystems, releases memory, and ensures
     * that everything is properly shut down before the engine object is destroyed.
     * 
     * @param engine A pointer to the H2DE engine to be destroyed.
     */
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    /**
     * @brief Starts the engine and begins its main loop.
     * 
     * This function initiates the engine's main execution loop, where the engine will
     * continuously update, render, and handle input until the engine is stopped.
     * 
     * @param engine A pointer to the H2DE engine instance that should be run.
     */
    friend void H2DE_RunEngine(H2DE_Engine* engine);
    /**
     * @brief Stops the engine and halts its main loop.
     * 
     * This function stops the engine's execution loop, allowing the engine to cleanly shut down
     * by performing necessary cleanup and releasing resources.
     * 
     * @param engine A pointer to the H2DE engine instance that should be stopped.
     */
    friend void H2DE_StopEngine(H2DE_Engine* engine);

    /**
     * @brief Sets the callback function for handling SDL events in the engine.
     * 
     * This function allows you to define a custom event handler that will be called
     * every time an SDL event occurs, such as user inputs (keyboard, mouse, etc).
     * The provided callback will be invoked with the event as an argument.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param call A function that will be called with an SDL_Event argument every time an event occurs.
     */
    friend void H2DE_SetHandleEventsCall(H2DE_Engine* engine, const std::function<void(SDL_Event)>& call);
    /**
     * @brief Sets the callback function for the engine's update cycle.
     * 
     * This function allows you to define a custom update function that will be called
     * every frame of the engine. The provided callback will be invoked during the
     * engine's update loop, except when the engine is paused.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param call A function that will be called during each frame, unless the engine is paused.
     */
    friend void H2DE_SetUpdateCall(H2DE_Engine* engine, const std::function<void()>& call);

    /**
     * @brief Loads all assets from the specified directory into the engine.
     * 
     * This function will scan the given directory and load all supported assets (textures and sounds)
     * into the engine. The assets are then available for use during the engine's execution.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param directory The path to the directory containing the assets to load.
     */
    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);
    /**
     * @brief Initializes a font for text rendering in the engine.
     * 
     * This function takes the specified font data and registers it with the engine
     * so that it can be used for rendering text. The font is identified by the given name.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param name The name used to identify the font in the engine.
     * @param font The font data, including texture, character size, and other properties.
     */
    friend void H2DE_InitFont(H2DE_Engine* engine, const std::string& name, const H2DE_Font& font);

    /**
     * @brief Retrieves the current FPS the engine is running at.
     * 
     * This function returns the actual FPS the engine is currently achieving during runtime.
     * It provides a real-time measurement of the engine's performance.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return The current FPS value.
     */
    friend unsigned int H2DE_GetCurrentFps(const H2DE_Engine* engine);
    /**
     * @brief Retrieves the target FPS that the engine is set to run at.
     * 
     * This function returns the target FPS value set by the user. The engine will attempt
     * to run at this FPS, though the actual FPS may vary depending on performance.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return The target FPS value.
     */
    friend unsigned int H2DE_GetFps(const H2DE_Engine* engine);
    /**
     * @brief Sets the target FPS for the engine to run at.
     * 
     * This function allows the user to set the desired FPS that the engine should aim to achieve.
     * The engine will adjust its update cycle to match the target FPS as closely as possible.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param fps The desired target FPS value.
     */
    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);

    /**
     * @brief Checks if the engine is currently paused.
     * 
     * This function returns whether the engine is currently paused or running. When paused,
     * the engine will stop processing updates, and no game logic or rendering will occur.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return `true` if the engine is paused, `false` otherwise.
     */
    friend bool H2DE_IsPaused(const H2DE_Engine* engine);
    /**
     * @brief Pauses the engine.
     * 
     * This function pauses the engine's update and rendering cycles. When paused, the game logic
     * and rendering will stop, effectively freezing the state of the engine until it is resumed.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_Pause(H2DE_Engine* engine);
    /**
     * @brief Resumes the engine after being paused.
     * 
     * This function resumes the engine's update and rendering cycles. When resumed, the engine
     * will continue processing game logic and rendering at the specified frame rate.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_Resume(H2DE_Engine* engine);
    /**
     * @brief Toggles the pause state of the engine.
     * 
     * This function switches the engine's state between paused and running. If the engine is currently
     * paused, calling this function will resume it, and if the engine is running, it will pause it. This allows
     * for a quick way to pause or resume the engine without needing to explicitly check its current state.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_TogglePause(H2DE_Engine* engine);

    /**
     * @brief Retrieves the current position of the engine window.
     * 
     * This function returns the position of the window relative to the screen's top-left corner.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return The current position of the window as an H2DE_AbsPos object.
     */
    friend H2DE_AbsPos H2DE_GetWindowPos(const H2DE_Engine* engine);
    /**
     * @brief Retrieves the current size of the engine window.
     * 
     * This function returns the dimensions (width and height) of the window.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return The current size of the window as an H2DE_AbsSize object.
     */
    friend H2DE_AbsSize H2DE_GetWindowSize(const H2DE_Engine* engine);
    /**
     * @brief Sets the position of the engine window.
     * 
     * This function moves the window to the specified position on the screen.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param pos The desired position of the window as an H2DE_AbsPos object.
     */
    friend void H2DE_SetWindowPos(const H2DE_Engine* engine, const H2DE_AbsPos& pos);
    /**
     * @brief Sets the size of the engine window.
     * 
     * This function resizes the window to the specified width and height.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param size The desired size of the window as an H2DE_AbsSize object.
     */
    friend void H2DE_SetWindowSize(const H2DE_Engine* engine, const H2DE_AbsSize& size);
    /**
     * @brief Sets the minimum size of the engine window.
     * 
     * This function defines the smallest allowed size for the window. The window cannot be resized
     * smaller than this size.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param minimumSize The minimum size of the window as an H2DE_AbsSize object.
     */
    friend void H2DE_SetWindowMinimumSize(const H2DE_Engine* engine, const H2DE_AbsSize& minimumSize);
    /**
     * @brief Sets the maximum size of the engine window.
     * 
     * This function defines the largest allowed size for the window. The window cannot be resized
     * larger than this size.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param maximumSize The maximum size of the window as an H2DE_AbsSize object.
     */
    friend void H2DE_SetWindowMaximumSize(const H2DE_Engine* engine, const H2DE_AbsSize& maximumSize);
    /**
     * @brief Sets the title of the engine window.
     * 
     * This function changes the text displayed in the window's title bar.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param title The new title to display in the window's title bar.
     */
    friend void H2DE_SetWindowTitle(const H2DE_Engine* engine, const std::string& title);
    /**
     * @brief Sets the icon of the engine window.
     * 
     * This function changes the icon that appears in the window's title bar and taskbar.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param textureName The name of the texture to use as the window icon.
     */
    friend void H2DE_SetWindowIcon(const H2DE_Engine* engine, const std::string& textureName);
    /**
     * @brief Sets the engine window to fullscreen mode.
     * 
     * This function switches the window to fullscreen or back to windowed mode based on the provided flag.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param fullscreen `true` to enable fullscreen mode, `false` to return to windowed mode.
     */
    friend void H2DE_SetWindowFullscreen(const H2DE_Engine* engine, bool fullscreen);
    /**
     * @brief Sets the window's resizable property.
     * 
     * This function determines whether the engine window can be resized by the user.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param resizable `true` to allow window resizing, `false` to disable it.
     */
    friend void H2DE_SetWindowResizable(const H2DE_Engine* engine, bool resizable);
    /**
     * @brief Sets the window's grab property.
     * 
     * This function enables or disables the mouse grabbing. When enabled, the mouse will be locked
     * to the window and won't leave it.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param grab `true` to enable mouse grabbing, `false` to disable it.
     */
    friend void H2DE_SetWindowGrab(const H2DE_Engine* engine, bool grab);
    /**
     * @brief Sets the window's aspect ratio.
     * 
     * This function defines a fixed aspect ratio for the window. The window's size will maintain this ratio
     * during resizing.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param ratio The desired aspect ratio to maintain during window resizing.
     */
    friend void H2DE_SetWindowRatio(const H2DE_Engine* engine, H2DE_WindowRatio ratio);

    /**
     * @brief Adds a new section to the settings file.
     * 
     * This function creates a new section in the `settings.ini` file. Sections are used to
     * categorize the settings, allowing for better organization.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The name of the section to add to the settings file.
     * @return `true` if the section was added successfully, `false` otherwise.
     */
    friend bool H2DE_SettingsAddSection(const H2DE_Engine* engine, const std::string& section);
    /**
     * @brief Adds a new key-value pair to a section in the settings file.
     * 
     * This function adds a new key-value pair to an existing section in the `settings.ini` file. If the section
     * does not exist, it will be created first.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The section where the key-value pair should be added.
     * @param key The key to associate with the value.
     * @param value The value to store with the key.
     * @return `true` if the key-value pair was added successfully, `false` otherwise.
     */
    friend bool H2DE_SettingsAddKey(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);
    /**
     * @brief Retrieves an integer value for a specified key from the settings file.
     * 
     * This function retrieves the value associated with a given key in a specified section. If the key is not
     * found, the function returns the provided default value.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The section where the key is located.
     * @param key The key whose value should be retrieved.
     * @param defaultValue The value to return if the key does not exist in the section.
     * @return The integer value associated with the key, or the default value if not found.
     */
    friend int H2DE_SettingsGetKeyInteger(const H2DE_Engine* engine, const std::string& section, const std::string& key, int defaultValue);
    /**
     * @brief Retrieves a string value for a specified key from the settings file.
     * 
     * This function retrieves the value associated with a given key in a specified section. If the key is not
     * found, the function returns the provided default value.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The section where the key is located.
     * @param key The key whose value should be retrieved.
     * @param defaultValue The value to return if the key does not exist in the section.
     * @return The string value associated with the key, or the default value if not found.
     */
    friend std::string H2DE_SettingsGetKeyString(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& defaultValue);
    /**
     * @brief Retrieves a boolean value for a specified key from the settings file.
     * 
     * This function retrieves the value associated with a given key in a specified section. If the key is not
     * found, the function returns the provided default value.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The section where the key is located.
     * @param key The key whose value should be retrieved.
     * @param defaultValue The value to return if the key does not exist in the section.
     * @return The boolean value associated with the key, or the default value if not found.
     */
    friend bool H2DE_SettingsGetKeyBool(const H2DE_Engine* engine, const std::string& section, const std::string& key, bool defaultValue);
    /**
     * @brief Sets the value of a specified key in the settings file.
     * 
     * This function updates the value associated with a given key in a specified section. If the key does not
     * exist, it will be created with the provided value.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param section The section where the key is located.
     * @param key The key whose value should be updated.
     * @param value The new value to associate with the key.
     * @return `true` if the key-value pair was updated successfully, `false` otherwise.
     */
    friend bool H2DE_SettingsSetKeyValue(const H2DE_Engine* engine, const std::string& section, const std::string& key, const std::string& value);

    /**
    * @brief Plays a song track by name.
    * 
    * This function starts playback of a song previously loaded into the engine.
    * 
    * @param engine A pointer to the H2DE engine instance.
    * @param name The name of the song to play.
    * @param loops Number of times to loop the song (-1 for infinite).
    * @param pauseSensitive Whether the song should pause when the engine is paused.
    */
    friend void H2DE_PlaySong(const H2DE_Engine* engine, const std::string& name, int loops, bool pauseSensitive);
    /**
     * @brief Plays a sound effect by name.
     * 
     * This function starts playback of a sound effect previously loaded into the engine.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param name The name of the sound effect to play.
     * @param loops Number of times to loop the sound (-1 for infinite).
     * @param pauseSensitive Whether the sound should pause when the engine is paused.
     * @return The channel ID of the sound effect being played.
     */
    friend int H2DE_PlaySfx(const H2DE_Engine* engine, const std::string& name, int loops, bool pauseSensitive);
    /**
     * @brief Stops the currently playing song.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_StopSong(const H2DE_Engine* engine);
    /**
     * @brief Stops a sound effect on a given channel.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the sound effect channel to stop.
     */
    friend void H2DE_StopSfx(const H2DE_Engine* engine, int id);
    /**
     * @brief Pauses the currently playing song.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_PauseSong(const H2DE_Engine* engine);
    /**
     * @brief Pauses a specific sound effect.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the sound effect channel to pause.
     */
    friend void H2DE_PauseSfx(const H2DE_Engine* engine, int id);
    /**
     * @brief Resumes playback of a paused song.
     * 
     * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_ResumeSong(const H2DE_Engine* engine);
    /**
     * @brief Resumes playback of a paused sound effect.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the sound effect channel to resume.
     */
    friend void H2DE_ResumeSfx(const H2DE_Engine* engine, int id);
    /**
     * @brief Sets the volume of the song.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param volume The new volume (0 to 128).
     */
    friend void H2DE_SetSongVolume(const H2DE_Engine* engine, int volume);
    /**
     * @brief Sets the volume of all sound effects.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param volume The new volume (0 to 128).
     */
    friend void H2DE_SetSfxVolume(const H2DE_Engine* engine, int volume);
    /**
     * @brief Checks if a song is currently playing.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @return `true` if a song is playing, `false` otherwise.
     */
    friend bool H2DE_IsSongPlaying(const H2DE_Engine* engine);
    /**
     * @brief Checks if a sound effect is currently playing on a given channel.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the sound effect channel.
     * @return `true` if the sound effect is playing, `false` otherwise.
     */
    friend bool H2DE_IsSfxPlaying(const H2DE_Engine* engine, int id);

    /**
     * @brief Sets a delay that will trigger a callback after a given time.
     * 
     * This function schedules a callback to be executed after a specified delay in milliseconds.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param ms The delay duration in milliseconds.
     * @param callback The function to call once the delay has elapsed.
     * @param pauseSensitive Whether the delay should be paused when the engine is paused.
     * @return The ID of the delay timer.
     */
    friend unsigned int H2DE_Delay(const H2DE_Engine* engine, unsigned int ms, const std::function<void()>& callback, bool pauseSensitive);
    /**
     * @brief Resets the timer for a delay by ID.
     * 
     * This function resets the countdown for the specified delay without changing its callback or configuration.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the delay to reset.
     */
    friend void H2DE_ResetDelay(const H2DE_Engine* engine, unsigned int id);
    /**
     * @brief Stops a delay by ID, with optional callback execution.
     * 
     * This function cancels an existing delay. If `call` is true, the associated callback will be executed immediately.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the delay to stop.
     * @param call Whether the callback should be executed immediately upon stopping.
     */
    friend void H2DE_StopDelay(const H2DE_Engine* engine, unsigned int id, bool call);

    /**
     * @brief Creates a timeline that interpolates a value over time using an easing function.
     * 
     * This function starts a timeline that progresses over a specified duration, calling the `update` function
     * each frame with the current interpolated value between 0.0 and 1.0, based on the easing function.
     * When the timeline completes, the `completed` callback is called. The timeline can also loop if specified.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param duration The total duration of the timeline in milliseconds.
     * @param easing The easing function to use for interpolation.
     * @param update The callback called every frame with the interpolated progress value.
     * @param completed The callback called when the timeline ends (or at each end if looping).
     * @param loops The number of times the timeline should repeat. `-1` for infinite loops.
     * @param pauseSensitive Whether the timeline should be paused when the engine is paused.
     * @return The ID of the created timeline.
     */
    friend unsigned int H2DE_CreateTimeline(const H2DE_Engine* engine, unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive);
    /**
     * @brief Resets a timeline by ID.
     * 
     * This function resets the timeline's internal progress to the start, without modifying its configuration.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the timeline to reset.
     */
    friend void H2DE_ResetTimeline(const H2DE_Engine* engine, unsigned int id);
    /**
     * @brief Stops a timeline by ID, with optional callback execution.
     * 
     * This function stops a running timeline. If `call` is true, the timeline's `completed` callback is triggered.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param id The ID of the timeline to stop.
     * @param call Whether the `completed` callback should be called when stopping the timeline.
     */
    friend void H2DE_StopTimeline(const H2DE_Engine* engine, unsigned int id, bool call);

    /**
	 * @brief Returns the current position of the camera in level coordinates.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @return The current camera position.
	 */
    friend H2DE_LevelPos H2DE_GetCameraPos(const H2DE_Engine* engine);
    /**
	 * @brief Returns the current size of the camera's view in level coordinates.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @return The current camera size.
	 */
    friend H2DE_LevelSize H2DE_GetCameraSize(const H2DE_Engine* engine);
    /**
	 * @brief Checks if an object is visible within the camera's view.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param object A pointer to the object to check.
	 * @return `true` if the object is at least partially within the view, `false` otherwise.
	 */
    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object);
    /**
	 * @brief Checks if a hitbox of an object is within the camera's view.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param object A pointer to the object containing the hitbox.
	 * @param hitbox The hitbox to check.
	 * @param absolute Whether the hitbox is in absolute coordinates.
	 * @return `true` if the hitbox is visible, `false` otherwise.
	 */
    friend bool H2DE_CameraContainsHitbox(const H2DE_Engine* engine, const H2DE_Object* object, const H2DE_Hitbox& hitbox, bool absolute);
    /**
	 * @brief Checks if a rectangle is within the camera's view.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param rect The rectangle to check.
	 * @param absolute Whether the rectangle is in absolute coordinates.
	 * @return `true` if the rectangle is visible, `false` otherwise.
	 */
    friend bool H2DE_CameraContainsRect(const H2DE_Engine* engine, const H2DE_LevelRect& rect, bool absolute);
    /**
	 * @brief Sets the camera's position in level coordinates.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param pos The new position for the camera.
	 */
    friend void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos);
    /**
	 * @brief Smoothly moves the camera to a new position.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param pos The target position.
	 * @param duration Duration of the transition in milliseconds.
	 * @param easing The easing function used for the transition.
	 * @param pauseSensitive Whether the movement pauses when the engine is paused.
	 */
    friend void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    /**
	 * @brief Sets the width of the camera. Height is calculated based on window aspect ratio.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param width The new width of the camera in level units.
	 */
    friend void H2DE_SetCameraWidth(const H2DE_Engine* engine, float width);
    /**
	 * @brief Sets the smoothing factor for camera following.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param smoothing The smoothing coefficient. Higher means smoother (0.0 - 1.0).
	 */
    friend void H2DE_SetCameraSmoothing(const H2DE_Engine* engine, float smoothing);
    /**
	 * @brief Sets the reference object for the camera to follow.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param object The object to be followed by the camera.
	 */
    friend void H2DE_SetCameraReference(const H2DE_Engine* engine, H2DE_Object* object);
    /**
	 * @brief Enables or disables the camera following the reference object.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param state `true` to enable camera following, `false` to disable it.
	 */
    friend void H2DE_SetCameraLockedToReference(const H2DE_Engine* engine, bool state);
    /**
	 * @brief Sets the padding around the reference object when following it.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param padding The level-space padding to apply.
	 */
    friend void H2DE_SetCameraPadding(const H2DE_Engine* engine, const H2DE_LevelPadding& padding);
    /**
	 * @brief Sets the camera padding relative to the reference object's rect.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param padding The padding to apply from the reference object.
	 */
    friend void H2DE_SetCameraPaddingFromReference(const H2DE_Engine* engine, const H2DE_LevelPadding& padding);

    /**
	 * @brief Gets the current position of the mouse in level or absolute coordinates.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param absolute If `true`, returns the absolute position on the window. If `false`, returns the position in the level.
	 * @return The current position of the mouse.
	 */
    friend H2DE_LevelPos H2DE_GetMousePos(const H2DE_Engine* engine, bool absolute);

    friend class H2DE_Object;
    friend class H2DE_ButtonObject;
    friend class H2DE_TextObject;
    /**
	 * @brief Creates a new bar object and adds it to the engine.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param objectData General object configuration.
	 * @param barObjectData Specific configuration for the bar object.
	 * @return A pointer to the created H2DE_BarObject.
	 */
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    /**
	 * @brief Creates a new basic object and adds it to the engine.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param objectData General object configuration.
	 * @param basicObjectData Specific configuration for the basic object.
	 * @return A pointer to the created H2DE_BasicObject.
	 */
    friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
    /**
	 * @brief Creates a new button object and adds it to the engine.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param objectData General object configuration.
	 * @param buttonObjectData Specific configuration for the button object.
	 * @return A pointer to the created H2DE_ButtonObject.
	 */
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    /**
	 * @brief Creates a new text object and adds it to the engine.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param objectData General object configuration.
	 * @param textObjectData Specific configuration for the text object.
	 * @return A pointer to the created H2DE_TextObject.
	 */
    friend H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_TextObjectData& textObjectData);
    /**
	 * @brief Removes an object from the engine and frees its memory.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param object A pointer to the object to destroy.
	 */
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);
    /**
	 * @brief Enables or disables object debug rendering.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param state `true` to enable debug rendering, `false` to disable it.
	 */
    friend void H2DE_DebugObjects(const H2DE_Engine* engine, bool state);
    /**
     * @brief Toggles the debug mode of the engine.
     * 
     * If debug mode is currently disabled, calling this function enables it.
     * If it's already enabled, the function disables it.
     * 
     * In debug mode, the engine pauses its normal update loop and only
     * progresses when `H2DE_DebugModeNextFrame` is called. This is useful
     * for frame-by-frame debugging of animations or time-dependent logic.
     * 
	 * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_ToggleDebugMode(H2DE_Engine* engine);
    /**
     * @brief Advances the engine by a single frame in debug mode.
     * 
     * This function only has an effect when debug mode is enabled via `H2DE_ToggleDebugMode`.
     * It forces the engine to process one frame, useful for frame-by-frame debugging
     * of animations, transitions, or any time-sensitive logic.
     * 
	 * @param engine A pointer to the H2DE engine instance.
     */
    friend void H2DE_DebugModeNextFrame(H2DE_Engine* engine);
};

H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
