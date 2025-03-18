#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_Engine::H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e) : engine(e) {
}

// CLEANUP
H2DE_Engine::H2DE_Renderer::~H2DE_Renderer() {
    destroyTextures();
    destroySounds();
}

void H2DE_Engine::H2DE_Renderer::destroyTextures() {
    for (const auto& [name, texture] : textures) {
        if (texture) SDL_DestroyTexture(texture);
    }
    textures.clear();
}

void H2DE_Engine::H2DE_Renderer::destroySounds() {
    for (const auto& [name, sound] : sounds) {
        // if (sound) delete sound; // crash -1073740940
    }
    sounds.clear();
}

// RENDER
void H2DE_Engine::H2DE_Renderer::render() {

}

// SETTER
void H2DE_Engine::H2DE_Renderer::setTextures(std::unordered_map<std::string, SDL_Texture*> t) {
    destroyTextures();
    textures = t;
}

void H2DE_Engine::H2DE_Renderer::setSounds(std::unordered_map<std::string, Mix_Chunk*> s) {
    destroySounds();
    sounds = s;
}
