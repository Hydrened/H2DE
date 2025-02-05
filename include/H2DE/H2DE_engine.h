#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#undef min

#include <H2DE_json.h>
#include <H2DE_utils.h>
#include <H2DE_asset_loader.h>
#include <H2DE_window.h>
#include <H2DE_renderer.h>
#include <H2DE_settings.h>
#include <H2DE_camera.h>
#include <H2DE_object.h>
#include <H2DE_button.h>
#include <H2DE_timeline.h>
#include <H2DE_game_data.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <Windows.h>
struct H2DE_GameData;
class H2DE_Window;
class H2DE_Renderer;
class H2DE_Settings;
class H2DE_Camera;
class H2DE_Timeline;
class H2DE_LevelObject;
class H2DE_Button;

/**
 * Type used to identify an engine
 * \since H2DE-1.0.0
 */
class H2DE_Engine {
private:
    H2DE_Window* window = nullptr;
    H2DE_Renderer* renderer = nullptr;
    H2DE_Settings* settings = nullptr;
    H2DE_Camera* camera = nullptr;

    H2DE_GameData* gameData = new H2DE_GameData();
    H2DE_EngineData data;
    unsigned int fps;
    bool isRunning = true;
    int currentFPS = 0;
    
    int loadedData = 0;
    int dataToLoad = 0;

    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Chunk*> sounds;

    std::vector<H2DE_LevelObject*> objects;
    std::vector<H2DE_Button*> buttons;
    std::vector<H2DE_Timeline*> loops;

    std::function<void(SDL_Event)> handleEvents = NULL;
    std::function<void()> update = NULL;

    static int countFilesToLoad(const std::filesystem::path& dir);
    void importFiles(const std::filesystem::path& dir);
    void importFile(const std::filesystem::path& file);
    void importTexture(const std::filesystem::path& img);
    void importSound(const std::filesystem::path& song);
    void assetImported();

    void updateLevelObjects();

    void click(int x, int y);

public:
    H2DE_Engine(H2DE_EngineData data);
    ~H2DE_Engine();

    /**
     * Runs the engine's game loop
     * \param engine pointer to the engine
     * \since H2DE-2.0.0
     */
    friend void H2DE_RunEngine(H2DE_Engine* engine);

    /**
     * Debugs the number of object rendered on by an engine
     * \param engine pointer to the engine
     * \param state debug state
     * \since H2DE-1.3.4
     */
    friend void H2DE_DebugEngine(H2DE_Engine* engine, bool state);

    /**
     * Delays a function call
     * \param ms time duration in milliseconds
     * \param callback function called after the delay
     * \since H2DE-2.0.1
     */
    static void H2DE_Delay(unsigned int ms, std::function<void()> callback);

    /**
     * Loads all assets from a directory
     * \param engine pointer to the engine
     * \param dir directory containing the assets
     * \since H2DE-1.0.0
     */
    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& dir);
    /**
     * Loads a specific asset
     * \param engine pointer to the engine
     * \param file asset to load
     * \since H2DE-1.3.10
     */
    friend void H2DE_LoadAsset(H2DE_Engine* engine, const std::filesystem::path& file);
    /**
     * Removes all assets
     * \param engine pointer to the engine
     * \since H2DE-1.3.11
     */
    friend void H2DE_RemoveAssets(H2DE_Engine* engine);
    /**
     * Removes a specific asset
     * \param engine pointer to the engine
     * \param file asset to remove
     * \since H2DE-1.3.11
     */
    friend void H2DE_RemoveAsset(H2DE_Engine* engine, const std::filesystem::path& file);

    /**
     * Creates a level object
     * \param engine pointer to the engine
     * \param data level object's data
     * \returns a pointer to the new level object
     * \since H2DE-2.0.4
     */
    friend H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, H2DE_LevelObjectData data);
    /**
     * Destroys a level object
     * \param engine pointer to the engine
     * \param object pointer to the object
     * \since H2DE-2.0.4
     */
    friend void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* object);

    /**
     * Creates a button
     * \param engine pointer to the engine
     * \param data button's data
     * \returns a pointer to the new button
     * \since H2DE-2.0.10
     */
    friend H2DE_Button* H2DE_CreateButton(H2DE_Engine* engine, H2DE_ButtonData data);
    /**
     * Destroys a button
     * \param engine pointer to the engine
     * \param button pointer to the button
     * \since H2DE-2.0.10
     */
    friend void H2DE_DestroyButton(H2DE_Engine* engine, H2DE_Button* button);

    /**
     * Plays a sound
     * \param engine pointer to the engine
     * \param channel target's channel (-1 for all)
     * \param sound name of the sound file
     * \param loop number of loops (-1 = infinite)
     * \returns the channel of the playing sound
     * \since H2DE-1.0.0
     */
    friend int H2DE_PlaySound(H2DE_Engine* engine, int channel, std::string sound, int loop);
    /**
     * Pauses a sound
     * \param engine pointer to the engine
     * \param channel target's channel (-1 for all)
     * \since H2DE-1.0.0
     */
    friend void H2DE_PauseSound(H2DE_Engine* engine, int channel);
    /**
     * Resumes a sound
     * \param engine pointer to the engine
     * \param channel target's channel (-1 for all)
     * \since H2DE-1.0.0
     */
    friend void H2DE_ResumeSound(H2DE_Engine* engine, int channel);

    /**
     * Gets the engine's window
     * \param engine pointer to the engine
     * \returns a pointer to the engine's window
     * \since H2DE-2.0.0
     */
    friend H2DE_Window* H2DE_GetWindow(H2DE_Engine* engine);
    /**
     * Gets the FPS limit set for an engine
     * \param engine pointer to the engine
     * \returns the fps limit
     * \since H2DE-1.0.9
     */
    friend int H2DE_GetFps(H2DE_Engine* engine);
    /**
     * Gets the number of frames for a time in ms
     * \param engine pointer to the engine
     * \param ms time in ms
     * \returns the number of frame
     * \since H2DE-2.0.9
     */
    friend int H2DE_GetSteps(H2DE_Engine* engine, unsigned int ms);
    /**
     * Gets the current FPS the engine is running at
     * \param engine pointer to the engine
     * \returns the current fps
     * \since H2DE-2.0.1
     */
    friend int H2DE_GetCurrentFps(H2DE_Engine* engine);
    /**
     * Gets the game's data
     * \param engine pointer to the engine
     * \returns the game's data
     * \since H2DE-2.0.5
     */
    friend H2DE_GameData* H2DE_GetGameData(H2DE_Engine* engine);
    /**
     * Gets the engine's settings
     * \param engine pointer to the engine
     * \returns the engine's settings
     * \since H2DE-2.0.8
     */
    friend H2DE_Settings* H2DE_GetSettings(H2DE_Engine* engine);
    /**
     * Gets the engine's camera
     * \param engine pointer to the engine
     * \returns the engine's camera
     * \since H2DE-2.0.2
     */
    friend H2DE_Camera* H2DE_GetCamera(H2DE_Engine* engine);

    /**
     * Sets the FPS limit of an engine
     * \param engine pointer to the engine
     * \param fps new FPS limit
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps);
    /**
     * Set the function call each frame in the game loop for events
     * \param engine pointer to the engine
     * \param call function called for events
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetGameHandleEventCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call);
    /**
     * Set the function call each frame in the game loop for updates
     * \param engine pointer to the engine
     * \param call function called for
     * \since H2DE-2.0.0
     */
    friend void H2DE_SetGameUpdateCall(H2DE_Engine* engine, std::function<void()> call);
};

/**
 * Creates an engine
 * \param data engine's data
 * \returns a pointer to new new engine
 * \since H2DE-1.0.0
 */
extern H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data);
/**
 * Destroys an engine
 * \param engine pointer to the engine
 * \since H2DE-1.0.0
 */
extern void H2DE_DestroyEngine(H2DE_Engine* engine);
/**
 * Sets volume's sound
 * \param channel target's channel (-1 for all)
 * \param volume sound's volume (0-100)
 * \since H2DE-1.0.0
 */
extern void H2DE_SetVolumeSound(int channel, int volume);

#endif
