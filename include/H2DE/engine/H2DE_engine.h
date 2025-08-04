#pragma once

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

#include <map>
#include <filesystem>

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
#include <H2DE/engine/H2DE_chrono_manager.h>
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
class H2DE_ChronoManager;
class H2DE_Chrono;
class H2DE_Camera;
class H2DE_ObjectManager;

using H2DE_Delay = H2DE_Timeline;

namespace H2DE {
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
    H2DE_Engine* createEngine(const H2DE_EngineData& data);
    /**
     * @brief Destroys the H2DE engine instance and releases all allocated resources.
     * 
     * Deletes all core components (window, renderer, asset loader, camera, timeline manager, etc.) 
     * and clears all objects managed by the engine.
     * 
     * @param engine Pointer to the H2DE_Engine instance to destroy.
     */
    void destroyEngine(H2DE_Engine* engine);
};

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
    friend H2DE_Engine* H2DE::createEngine(const H2DE_EngineData& data);
    friend void H2DE::destroyEngine(H2DE_Engine* engine);

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
     * @brief Loads all supported assets (textures and sounds) from a given directory.
     * 
     * This function scans the provided directory (recursively) and loads all valid image and audio files
     * into the engine. It also updates the engine's internal caches for textures and sounds.
     * 
     * If a file with the same name already exists in the cache, it will be overridden (a warning is logged).
     * 
     * @param directory Path to the folder containing the assets to load.
     * @param silentLoad If true, suppresses override warnings and disables the progress bar display.
     */
    void loadAssetsSync(const std::string& directory, bool silentLoad = false);
    /**
     * @brief Loads all supported assets asynchronously from a given directory.
     * 
     * This function works the same as `loadAssetsSync` but performs the operation on a separate thread.
     * It allows you to provide progress feedback (0.0f to 1.0f) and a callback when loading is complete.
     * 
     * The engine remains responsive during the loading process.
     * 
     * @param directory Path to the folder containing the assets to load.
     * @param progress Callback function that receives the current progress as a float from 0.0f to 1.0f.
     * @param completed Callback function called once all assets are loaded and stored in buffers.
     * @param silentLoad If true, suppresses override warnings.
     */
    void loadAssetsAsync(const std::string& directory, const std::function<void(float)>& progress, const std::function<void()>& completed, bool silentLoad = false);
    /**
     * @brief Loads or overrides a font with the given name in the engine's font collection.
     * 
     * If a font with the same name already exists, a warning is logged before overriding.
     * 
     * @param name The unique identifier for the font.
     * @param font The font data to load.
     * @param silentLoad If true, suppresses override warnings.
     */
    void loadFont(const std::string& name, const H2DE_Font& font, bool silentLoad = false);

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
    H2DE_Timeline* createTimeline(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);

    /**
     * @brief Creates a delay (a timeline without progress updates) for a specified duration.
     * 
     * @param duration Duration of the delay in milliseconds.
     * @param callback Function to call after the delay finishes.
     * @param pauseSensitive Whether the delay respects the engine's pause state (default true).
     * @return The unique ID of the created delay.
     */
    H2DE_Delay* createDelay(uint32_t duration, const std::function<void()>& callback, bool pauseSensitive = true);

    /**
     * @brief Creates a chrono with an optional start time and direction.
     * 
     * @param start The initial time value of the chrono.
     * @param increasing Whether the chrono counts up (`true`) or down (`false`). Default is `true`.
     * @param pauseSensitive Whether the chrono respects the engine's pause state. Default is `true`.
     * @return The unique ID of the created chrono.
     */
    H2DE_Chrono* createChrono(const H2DE_Time& start, bool increasing = true, bool pauseSensitive = true);

    /**
     * @brief Creates a new object of type H2DE_Object_T and adds it to the engine's object list.
     * 
     * This function supports both generic objects that require only `H2DE_ObjectData`, 
     * and specialized objects that require an additional specific data parameter (usually a struct).
     * 
     * @tparam H2DE_Object_T The type of the object to create.
     * @tparam H2DE_SpecificObjectData_T Variadic template for additional object-specific data types (optional).
     * 
     * @param objectData Common initialization data for the object (shared among all object types).
     * @param specificObjectData Optional object-specific data used only if the object type declares a nested `H2DE_DataType`.
     * 
     * @return Pointer to the newly created object.
     * 
     * @warning Compilation will fail if `H2DE_Object_T` declares a `H2DE_DataType`, but no or more than one extra argument is provided.
     */
    template<typename H2DE_Object_T, typename... H2DE_SpecificObjectData_T>
    H2DE_Object_T* createObject(const H2DE_ObjectData& objectData, H2DE_SpecificObjectData_T&&... specificObjectData);

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
    template<typename, typename = void>
    struct has_H2DE_DataType : std::false_type {};

    template<typename T>
    struct has_H2DE_DataType<T, std::void_t<typename T::H2DE_DataType>> : std::true_type {};

    H2DE_EngineData data;

    H2DE_Settings* settings = nullptr;
    H2DE_Window* window = nullptr;
    H2DE_AssetLoaderManager* assetLoaderManager = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_Audio* audio = nullptr;
    H2DE_TimelineManager* timelineManager = nullptr;
    H2DE_ChronoManager* chronoManager = nullptr;
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

#include <H2DE/engine/H2DE_engine.inl>
