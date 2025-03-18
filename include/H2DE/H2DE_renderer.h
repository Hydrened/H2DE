#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Renderer {
private:
    H2DE_Engine* engine;

    std::unordered_map<std::string, SDL_Texture*> textures = {};
    std::unordered_map<std::string, Mix_Chunk*> sounds = {};

public:
    H2DE_Renderer(H2DE_Engine* engine);
    ~H2DE_Renderer();

    void render();

    void destroyTextures();
    void destroySounds();

    void setTextures(std::unordered_map<std::string, SDL_Texture*> textures);
    void setSounds(std::unordered_map<std::string, Mix_Chunk*> sounds);
};

#endif
