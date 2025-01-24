#ifndef H2DE_ASSET_LOADER_H
#define H2DE_ASSET_LOADER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class H2DE_AssetLoader {
public:
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* file);
    static Mix_Chunk* loadSound(const char* file);
};

#endif
