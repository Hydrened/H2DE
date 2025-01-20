#include <H2DE/H2DE_loader.h>

SDL_Texture* H2DE_Loader::loadTexture(SDL_Renderer* renderer, const char* file) {
    SDL_Surface* surface = IMG_Load(file);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Mix_Chunk* H2DE_Loader::loadSound(const char* file) {
    return Mix_LoadWAV(file);
}
