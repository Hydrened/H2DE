#ifndef H2DE_LOADER_H
#define H2DE_LOADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/**
 * Contains the methods required by the engine to load files such as:
 * - png
 * - mp3
 * - wav
 * - ttf
 * 
 * \since H2DE-1.0.0
 */
class H2DE_Loader {
public:
    /**
     * Loads a texture from a png file
     * 
     * \param renderer a pointer to the renderer
     * \param file the name of the file
     * 
     * \return a texture
     * 
     * \since H2DE-1.0.0
     */
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file);
    /**
     * Loads a music from a mp3 file
     * 
     * \param file the name of the file
     * 
     * \return a music
     * 
     * \since H2DE-1.0.0
     */
    static Mix_Music* loadSong(const char* file);
    /**
     * Loads a sfx from a wav file
     * 
     * \param file the name of the file
     * 
     * \return a chunk
     * 
     * \since H2DE-1.0.0
     */
    static Mix_Chunk* loadSFX(const char* file);
    /**
     * Loads a font from a ttf file
     * 
     * \param file the name of the file
     * 
     * \return a font
     * 
     * \since H2DE-1.0.0
     */
    static TTF_Font* loadFont(const char* file);
};

#endif
