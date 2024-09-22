#ifndef H2DE_ENGINE_H
#define H2DE_ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <H2DE/H2DE_loader.h>
#include <H2DE/H2DE_calculator.h>
#include <H2DE/H2DE_types.h>
#include <H2DE/H2DE_graphic.h>
#include <H2DE/H2DE_effect.h>
#include <H2DE/H2DE_json.h>
#include <algorithm>
#include <climits>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

/**
 * The type used to identify an engine
 * 
 * \since H2DE-1.0.0
 */
class H2DE_Engine {
private:
    int fps;
    H2DE_Size size;
    H2DE_Size maxSize = { -1, -1 };
    bool isRunning = true;
    SDL_Renderer* renderer;
    int dataToLoad, loadedData = 0;
    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Music*> songs;
    std::unordered_map<std::string, Mix_Chunk*> sfxs;
    std::unordered_map<std::string, TTF_Font*> fonts;
    std::vector<H2DE_GraphicObject*> graphicObjects;
    
    /**
     * Counts the number of file to be loaded from a parent directory
     * 
     * \param dir parent directory
     * 
     * \return the number of files
     * 
     * \since H2DE-1.0.0
     */
    static int countFiles(const fs::path& dir);
    /**
     * Import the files
     * 
     * \param dir parent directory
     * 
     * \since H2DE-1.0.0
     */
    void importFiles(const fs::path& dir);
    /**
     * Import a texture from a png file
     * 
     * \param img png file
     * 
     * \since H2DE-1.0.0
     */
    void importTexture(const fs::path& img);
    /**
     * Import a song from a mp3 file
     * 
     * \param song mp3 file
     * 
     * \since H2DE-1.0.0
     */
    void importSong(const fs::path& song);
    /**
     * Import a SFX from a wav file
     * 
     * \param sfx wav file
     * 
     * \since H2DE-1.0.0
     */
    void importSFX(const fs::path& sfx);
    /**
     * Import a font from a ttf file
     * 
     * \param font ttf file
     * 
     * \since H2DE-1.0.0
     */
    void importFont(const fs::path& font);
    /**
     * Indicate that an asset has been loaded
     * 
     * \since H2DE-1.0.0
     */
    void assetImported();
    
    /**
     * Duplicate `H2DE_GraphicObject` which have the `repeatX` property
     * 
     * \return a graphic object vector of the duplicated graphic objects
     * 
     * \since H2DE-1.0.0
     */
    std::vector<H2DE_GraphicObject*> getRepeatXGraphicObjects();
    /**
     * Duplicate `H2DE_GraphicObject` which have the `repeatY` property
     * 
     * \return a graphic object vector of the duplicated graphic objects
     * 
     * \since H2DE-1.0.0
     */
    std::vector<H2DE_GraphicObject*> getRepeatYGraphicObjects();

    /**
     * Renders a pixel on the renderer
     * 
     * \param pos position of the pixel
     * \param color color of the pixel
     * 
     * \since H2DE-1.0.0
     */
    void renderPixel(H2DE_Pos pos, H2DE_Color color);
    /**
     * Renders an image on the renderer
     * 
     * \param g the image to render
     * 
     * \since H2DE-1.0.0
     */
    void renderImage(H2DE_GraphicObject* g);
    /**
     * Renders a polygon on the renderer
     * 
     * \param g the polygon to render
     * 
     * \since H2DE-1.0.0
     */
    void renderPolygon(H2DE_GraphicObject* g);
    /**
     * Renders a circle on the renderer
     * 
     * \param g the circle to render
     * 
     * \since H2DE-1.0.0
     */
    void renderCircle(H2DE_GraphicObject* g);
    /**
     * Renders a text on the renderer
     * 
     * \param g the text to render
     * 
     * \since H2DE-1.0.0
     */
    void renderText(H2DE_GraphicObject* g);

public:
    H2DE_Engine(SDL_Renderer* renderer, int w, int h, int fps);
    ~H2DE_Engine();

    /**
     * Render all graphic objects added during this frame
     * 
     * \since H2DE-1.0.0
     */
    void render();
    /**
     * Loads every assets from a parent directory
     * 
     * \param dir parent directory
     * 
     * \since H2DE-1.0.0
     */
    void loadAssets(const fs::path& dir);
    /**
     * Add a graphic object to be rendered this frame
     * 
     * \param g the graphic object
     * 
     * \since H2DE-1.0.0
     */
    void addGraphicObject(H2DE_GraphicObject* g);

    /**
     * Gets the size of the engine
     * 
     * \param engine pointer to the engine
     * 
     * \return the size of the engine
     * 
     * \since H2DE-1.0.4
     */
    friend H2DE_Size H2DE_GetEngineSize(H2DE_Engine* engine);
    /**
     * Sets a new size for the engine
     * 
     * \param engine pointer to the engine
     * \param size the new size
     * 
     * \since H2DE-1.0.4
     */
    friend void H2DE_SetEngineSize(H2DE_Engine* engine, int w, int h);
    /**
     * Sets the maximum size for the specified engine
     * 
     * \param engine pointer to the engine
     * \param w maxmum width for the engine
     * \param h maxmum height for the engine
     * 
     * \since H2DE-1.0.5
     */
    friend void H2DE_SetEngineMaximumSize(H2DE_Engine* engine, int w, int h);
    /**
     * Gets the maximum size for the specified engine
     * 
     * \param engine pointer to the engine
     * 
     * \return the maximum size of the engine
     * 
     * \since H2DE-1.0.5
     */
    friend H2DE_Size H2DE_GetEngineMaximumSize(H2DE_Engine* engine);

    /**
     * Sets the song volume
     * 
     * \param volume the volume (0-100)
     * 
     * \since H2DE-1.0.0
     */
    void setSongVolume(int volume);
    /**
     * Plays a song
     * 
     * \param song the name of the loaded song
     * \param loop number of loop (-1 = infinite)
     * 
     * \since H2DE-1.0.0
     */
    void playSong(std::string song, int loop);
    /**
     * Pauses the current song
     * 
     * \since H2DE-1.0.0
     */
    void pauseSong();
    /**
     * Resumes the current song
     * 
     * \since H2DE-1.0.0
     */
    void resumeSong();

    /**
     * Sets the volume for a specific sfx
     * 
     * \param volume the volume (0-100)
     * 
     * \since H2DE-1.0.0
     */
    void setSFXVolume(int channel, int volume);
    /**
     * Plays a sfx
     * 
     * \param song the name of the loaded sfx
     * \param loop number of loop (-1 = infinite)
     * 
     * \return the channel of the sfx
     * 
     * \since H2DE-1.0.0
     */
    int playSFX(std::string sfx, int loop);
    /**
     * Pauses a sfx
     * 
     * \param channel the channel of the target
     * 
     * \since H2DE-1.0.0
     */
    void pauseSFX(int channel);
    /**
     * Resumes a sfx
     * 
     * \param channel the channel of the target
     * 
     * \since H2DE-1.0.0
     */
    void resumeSFX(int channel);
};

/**
 * Creates an engine
 * 
 * \param renderer a pointer to the renderer
 * \param w the width of the engine
 * \param h this heigth of the engine
 * \param fps the fps the window is running at
 * 
 * \return a pointer to an engine
 * 
 * \since H2DE-1.0.0
 */
extern H2DE_Engine* H2DE_CreateEngine(SDL_Renderer* renderer, int w, int h, int fps);
/**
 * Destroys an engine
 * 
 * \param engine the engine to destroy
 * 
 * \since H2DE-1.0.0
 */
extern void H2DE_DestroyEngine(H2DE_Engine* engine);

#endif
