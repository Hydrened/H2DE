#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

/**
 * @file H2DE_Engine.h
 * @brief Main header file defining the core engine class H2DE_Engine and its dependencies.
 * 
 * This file includes all necessary headers and declares the H2DE_Engine class,
 * which manages the game loop, assets, rendering, timelines, objects, and overall engine state.
 * 
 * The engine relies on SDL2 and integrates multiple subsystems such as windowing, audio,
 * camera, timeline animations, and object management to provide a flexible 2D game framework.
 */

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <vector>
#include <windows.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>

#include <H2DE/utils/H2DE_utils.h>
#include <H2DE/engine/H2DE_settings.h>
#include <H2DE/engine/H2DE_window.h>
#include <H2DE/engine/H2DE_asset_loader_manager.h>
#include <H2DE/engine/H2DE_renderer.h>
#include <H2DE/engine/H2DE_audio.h>
#include <H2DE/engine/H2DE_timeline_manager.h>
#include <H2DE/engine/H2DE_camera.h>
#include <H2DE/engine/H2DE_object_manager.h>
#include <H2DE/engine/H2DE_json.h>
#include <H2DE/surfaces/H2DE_surface.h>
#include <H2DE/objects/H2DE_object.h>

class H2DE_Settings;
class H2DE_Window;
class H2DE_AssetLoaderManager;
class H2DE_Renderer;
class H2DE_Audio;
class H2DE_TimelineManager;
class H2DE_Camera;
class H2DE_ObjectManager;

/**
 * @class H2DE_Engine
 * @brief Central class managing the whole 2D game engine lifecycle and subsystems.
 * 
 * H2DE_Engine is the heart of the H2DE 2D engine. It handles:
 * - The main game loop (event processing, update, rendering)
 * - Asset loading (textures, fonts, sounds)
 * - Timeline animations and delays with easing support
 * - Object creation, destruction, and management
 * - Debug modes and frame stepping
 * - Input handling and FPS/delta time tracking
 * 
 * The class exposes a rich API to create game objects, control timelines, and manage engine state,
 * allowing flexible game development while abstracting SDL2 and other internals.
 * 
 * @note Debug mode disables update and render for precise frame control.
 * @note Timeline loops can be infinite or finite, with pause sensitivity control.
 */
class H2DE_Engine {
public:
    friend H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
    friend void H2DE_DestroyEngine(H2DE_Engine* engine);

    /**
     * @brief Starts the main loop of the engine, processing events, updating, and rendering frames.
     * 
     * This function runs until the engine is stopped via stop() or when an SDL_QUIT event is received.
     * It calculates delta time and FPS, handles events, updates game logic, and renders frames.
     * 
     * @note If debug mode is enabled, the update and render steps are skipped.
     */
    void run();
    /**
     * @brief Stops the main engine loop by setting the running flag to false.
     */
    inline void stop() noexcept {
        isRunning = false;
    }

    /**
     * @brief Loads all assets (textures, sounds) from the specified directory into the engine.
     * 
     * This function verifies that the directory exists and is valid, then imports files.
     * It updates the engine's texture and sound caches, warning if any assets are overridden.
     * 
     * @param directory Path to the folder containing assets to load.
     */
    void loadAssets(const std::string& directory);
    /**
     * @brief Loads or overrides a font with the given name in the engine's font collection.
     * 
     * If a font with the same name already exists, a warning is logged before overriding.
     * 
     * @param name The unique identifier for the font.
     * @param font The font data to load.
     */
    void loadFont(const std::string& name, const H2DE_Font& font);

    /**
     * @brief Enables or disables the debug mode of the engine.
     * 
     * @param state True to enable debug mode, false to disable.
     */
    inline void debugMode(bool state) noexcept {
        debugModeEnabled = state;
    }
    /**
     * @brief Toggles the current state of the debug mode.
     */
    inline void toggleDebugMode() noexcept {
        debugMode(!debugModeEnabled);
    }

    /**
     * @brief Enables or disables debug visualization for objects.
     * 
     * @param state True to enable object debug mode, false to disable.
     */
    inline void debugObjects(bool state) noexcept {
        debugObjectEnabled = state;
    }
    /**
     * @brief Toggles the debug visualization state for objects.
     */
    inline void toggleDebugObject() noexcept {
        debugObjectEnabled = !debugObjectEnabled;
    }

    /**
     * @brief Advances debug mode to the next frame for step-by-step debugging.
     */
    void debugModeNextFrame();
    /**
     * @brief Moves debug mode back to the previous frame.
     */
    void debugModePreviousFrame();

    /**
     * @brief Pauses the engine, stopping updates.
     */
    inline void pause() noexcept {
        paused = true;
    }
    /**
     * @brief Resumes the engine from a paused state.
     */
    inline void resume() noexcept {
        paused = false;
    }
    /**
     * @brief Toggles the paused state of the engine.
     */
    inline void togglePause() noexcept {
        paused = !paused;
    }

    /**
     * @brief Creates a new timeline animation with the specified parameters.
     * 
     * @param duration Duration of the timeline in milliseconds.
     * @param easing The easing function type to apply.
     * @param update Callback called every frame with a progress value between 0 and 1.
     * @param completed Callback called at the end of each loop iteration.
     * @param loops Number of times the timeline should loop. Use H2DE_INFINITE_LOOP for infinite looping.
     * @param pauseSensitive Whether the timeline respects the engine's pause state (default true).
     * 
     * @return The unique ID of the created timeline.
     */
    H2DE_TimelineID createTimeline(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);
    /**
     * @brief Pauses the timeline with the given ID.
     * @param id The timeline's unique identifier.
     */
    void pauseTimeline(H2DE_TimelineID id);
    /**
     * @brief Resumes the timeline with the given ID.
     * @param id The timeline's unique identifier.
     */
    void resumeTimeline(H2DE_TimelineID id);
    /**
     * @brief Toggles pause state of the timeline with the given ID.
     * @param id The timeline's unique identifier.
     */
    void togglePauseTimeline(H2DE_TimelineID id);
    /**
     * @brief Resets the timeline with the given ID to its initial state.
     * @param id The timeline's unique identifier.
     */
    void resetTimeline(H2DE_TimelineID id);
    /**
     * @brief Stops the timeline with the given ID.
     * 
     * @param id The timeline's unique identifier.
     * @param callCompleted Whether to call the completed callback on stop.
     */
    void stopTimeline(H2DE_TimelineID id, bool callCompleted);
    /**
     * @brief Checks if the timeline with the given ID is currently paused.
     * @param id The timeline's unique identifier.
     * @return True if paused, false otherwise.
     */
    bool isTimelinePaused(H2DE_TimelineID id) const;
    /**
     * @brief Checks if the timeline with the given ID is currently stoped.
     * @param id The timeline's unique identifier.
     * @return True if stoped, false otherwise.
     */
    bool isTimelineStoped(H2DE_TimelineID id) const;

    /**
     * @brief Creates a delay (a timeline without progress updates) for a specified duration.
     * 
     * @param duration Duration of the delay in milliseconds.
     * @param callback Function to call after the delay finishes.
     * @param pauseSensitive Whether the delay respects the engine's pause state (default true).
     * @return The unique ID of the created delay.
     */
    H2DE_DelayID delay(uint32_t duration, const std::function<void()>& callback, bool pauseSensitive = true);
    /**
     * @brief Pauses a delay by pausing its underlying timeline.
     * @param id The delay's unique identifier.
     */
    inline void pauseDelay(H2DE_DelayID id) {
        pauseTimeline(id);
    }
    /**
     * @brief Resumes a delay by resuming its underlying timeline.
     * @param id The delay's unique identifier.
     */
    inline void resumeDelay(H2DE_DelayID id) {
        resumeTimeline(id);
    }
    /**
     * @brief Toggles pause state of a delay by toggling its underlying timeline.
     * @param id The delay's unique identifier.
     */
    inline void togglePauseDelay(H2DE_DelayID id) {
        togglePauseTimeline(id);
    }
    /**
     * @brief Resets a delay by resetting its underlying timeline.
     * @param id The delay's unique identifier.
     */
    inline void resetDelay(H2DE_DelayID id) {
        resetTimeline(id);
    }
    /**
     * @brief Stops a delay by stopping its underlying timeline.
     * 
     * @param id The delay's unique identifier.
     * @param callCompleted Whether to call the completed callback on stop.
     */
    inline void stopDelay(H2DE_DelayID id,  bool callCompleted) {
        stopTimeline(id, callCompleted);
    }
    /**
     * @brief Checks if the delay with the given ID is currently paused.
     * @param id The delay's unique identifier.
     * @return True if paused, false otherwise.
     */
    inline bool isDelayPaused(H2DE_DelayID id) const {
        return isTimelinePaused(id);
    }
    /**
     * @brief Checks if the delay with the given ID is currently stoped.
     * @param id The delay's unique identifier.
     * @return True if stoped, false otherwise.
     */
    inline bool isDelayStoped(H2DE_DelayID id) const {
        return isTimelinePaused(id);
    }

    /**
     * @brief Creates a new object of type H2DE_Object_T and adds it to the engine's object list.
     * 
     * @tparam H2DE_Object_T The class type of the object to create.
     * @param objectData Common initialization data for the object.
     * @return Pointer to the newly created object.
     */
    template<typename H2DE_Object_T>
    H2DE_Object_T* createObject(const H2DE_ObjectData& objectData) {
        H2DE_Object_T* object = new H2DE_Object_T(this, objectData);
        objects.push_back(object);
        return object;
    }
    /**
     * @brief Creates a new object of type H2DE_Object_T with specific additional data, and adds it to the engine's object list.
     * 
     * @tparam H2DE_Object_T The class type of the object to create.
     * @param objectData Common initialization data for the object.
     * @param specificObjectData Additional data specific to the object type.
     * @return Pointer to the newly created object.
     * 
     * @note If the created object is of type H2DE_ButtonObject, the object manager is refreshed.
     */
    template<typename H2DE_Object_T>
    H2DE_Object_T* createObject(const H2DE_ObjectData& objectData, const typename H2DE_Object_T::H2DE_DataType& specificObjectData) {
        H2DE_Object_T* object = new H2DE_Object_T(this, objectData, specificObjectData);
        objects.push_back(object);

        if constexpr (std::is_same_v<H2DE_Object_T, H2DE_ButtonObject>) {
            refreshObjectManager();
        }

        return object;
    }
    /**
     * @brief Destroys and removes the specified object from the engine.
     * 
     * @param object Pointer to the object to destroy.
     * @return True if the object was found and destroyed, false otherwise.
     */
    bool destroyObject(H2DE_Object* object);

    /**
     * @brief Retrieves the engine initialization data.
     * @return A copy of the H2DE_EngineData struct used to initialize the engine.
     */
    inline H2DE_EngineData getData() const noexcept {
        return data;
    }
    /**
     * @brief Gets a pointer to the engine settings.
     * @return Pointer to the H2DE_Settings instance.
     */
    inline H2DE_Settings* getSettings() const noexcept {
        return settings;
    }
    /**
     * @brief Gets a pointer to the engine window.
     * @return Pointer to the H2DE_Window instance.
     */
    inline H2DE_Window* getWindow() const noexcept {
        return window;
    }
    /**
     * @brief Gets a pointer to the engine audio manager.
     * @return Pointer to the H2DE_Audio instance.
     */
    inline H2DE_Audio* getAudio() const noexcept {
        return audio;
    }
    /**
     * @brief Gets a pointer to the engine camera.
     * @return Pointer to the H2DE_Camera instance.
     */
    inline H2DE_Camera* getCamera() const noexcept {
        return camera;
    }
    /**
     * @brief Gets the target frames per second (FPS) of the engine.
     * @return The FPS value as a 16-bit unsigned integer.
     */
    constexpr uint16_t getFPS() const noexcept {
        return fps;
    }
    /**
     * @brief Gets the current frame rate.
     * @param round Whether to round the FPS value (default true).
     * @return The current FPS as a float, optionally rounded.
     */
    inline float getCurrentFPS(bool round = true) const noexcept {
        return (round) ? H2DE::round(currentFPS) : currentFPS; 
    }
    /**
     * @brief Gets the delta time between the last two frames in seconds.
     * @return Delta time as a float.
     */
    constexpr float getDeltaTime() const noexcept {
        return deltaTime;
    }
    /**
     * @brief Gets the fixed delta time based on the target FPS.
     * @return Fixed delta time as a float.
     */
    constexpr float getFixedDeltaTime() const noexcept {
        return 1.0f / static_cast<float>(fps);
    }
    /**
     * @brief Checks if the engine is currently paused.
     * @return True if paused, false otherwise.
     */
    constexpr bool isPaused() const noexcept {
        return paused;
    }

    /**
     * @brief Gets the number of objects rendered in the current frame.
     * @return The count of rendered objects as a 32-bit unsigned integer.
     */
    uint32_t getObjectsRenderedNumber() const noexcept;
    /**
     * @brief Gets the number of surfaces rendered in the current frame.
     * @return The count of rendered surfaces as a 32-bit unsigned integer.
     */
    uint32_t getSurfacesRenderedNumber() const noexcept;
    /**
     * @brief Gets the number of hitboxes rendered in the current frame.
     * @return The count of rendered hitboxes as a 32-bit unsigned integer.
     */
    uint32_t getHitboxesRenderedNumber() const noexcept;

    /**
     * @brief Gets the mouse position in the game world coordinates.
     * 
     * This represents the mouse position relative to the game world (taking camera, zoom, etc. into account).
     * 
     * @return Mouse position as H2DE_Translate in game world space.
     */
    inline const H2DE_Translate getMouseGamePos() const {
        return getMousePos(false);
    }
    /**
     * @brief Gets the mouse position in the user interface coordinates.
     * 
     * This represents the absolute mouse position on the UI layer (screen coordinates).
     * 
     * @return Mouse position as H2DE_Translate in UI space.
     */
    inline const H2DE_Translate getMouseInterfacePos() const {
        return getMousePos(true);
    }

    /**
     * @brief Retrieves the size (width and height) of a texture by its name.
     * 
     * Queries the renderer for the texture and returns its pixel dimensions.
     * Logs an error if the texture is not found or if querying fails.
     * 
     * @param textureName The name of the texture to get the size of.
     * @return H2DE_PixelSize struct containing width and height in pixels.
     */
    const H2DE_PixelSize getTextureSize(const std::string& textureName) const;

    /**
     * @brief Sets the target frames per second (FPS) for the engine.
     * @param FPS The desired FPS value as a 16-bit unsigned integer.
     */
    inline void setFPS(uint16_t FPS) noexcept {
        fps = FPS;
    }
    /**
     * @brief Sets a custom callback function to handle SDL events.
     * @param call A std::function taking SDL_Event as parameter.
     */
    inline void setHandleEventCall(const std::function<void(SDL_Event)>& call) noexcept{
        handleEventsCall = call;
    }
    /**
     * @brief Sets a custom callback function to be called every update frame.
     * @param call A std::function with no parameters.
     */
    inline void setUpdateCall(const std::function<void()>& call) noexcept {
        updateCall = call;
    }

    friend class H2DE_Window;
    friend class H2DE_AssetLoaderManager;
    friend class H2DE_Renderer;
    friend class H2DE_Audio;
    friend class H2DE_Camera;
    friend class H2DE_ObjectManager;
    friend class H2DE_Object;
    friend class H2DE_TextObject;
    
private:
    H2DE_EngineData data;

    H2DE_Settings* settings = nullptr;
    H2DE_Window* window = nullptr;
    H2DE_AssetLoaderManager* assetLoaderManager = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_Audio* audio = nullptr;
    H2DE_TimelineManager* timelineManager = nullptr;
    H2DE_Camera* camera = nullptr;
    H2DE_ObjectManager* objectManager = nullptr;

    uint16_t fps = 0;
    float currentFPS = 0;
    float deltaTime = 0.0f;
    bool isRunning = false;
    bool paused = false;

    bool debugModeEnabled = false;
    bool debugObjectEnabled = false;

    std::function<void(SDL_Event)> handleEventsCall = nullptr;
    std::function<void()> updateCall = nullptr;

    std::unordered_map<std::string, H2DE_Font> fonts = {};
    std::vector<H2DE_Object*> objects = {};

    H2DE_PixelPos mousePos = { 0, 0 };

    H2DE_Engine(const H2DE_EngineData& data);
    ~H2DE_Engine();

    void handleEvents(SDL_Event event);
    void update();
    void updateObjects();

    void refreshObjectManager();

    void destroy();
    void destroyObjects();

    const H2DE_Translate getMousePos(bool absolute) const;

    static bool isPositionGreater(H2DE_Object* a, H2DE_Object* b);
};

/**
 * @brief Creates and initializes the H2DE engine singleton instance using the provided configuration data.
 * 
 * This function can only be called once during the application lifetime. It initializes all core components of the engine,
 * including the window, renderer, asset manager, camera, object manager, audio, timeline manager, and settings.
 * 
 * @param data Constant reference to H2DE_EngineData structure containing engine initialization parameters like window and camera settings.
 * 
 * @return Pointer to the created H2DE_Engine instance. Returns nullptr if creation fails or if the engine already exists.
 * 
 * @throws Throws a runtime error if an attempt is made to create more than one engine instance.
 *         Exceptions are caught internally and reported via a MessageBox.
 */
H2DE_Engine* H2DE_CreateEngine(const H2DE_EngineData& data);
/**
 * @brief Destroys the H2DE engine instance and releases all allocated resources.
 * 
 * Deletes all core components (window, renderer, asset loader, camera, timeline manager, etc.) 
 * and clears all objects managed by the engine.
 * 
 * @param engine Pointer to the H2DE_Engine instance to destroy.
 */
void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
