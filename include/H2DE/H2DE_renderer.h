#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <map>
#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Renderer {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;
    std::vector<H2DE_Object*>& objects;

    std::unordered_map<std::string, SDL_Texture*> textures = {};
    std::unordered_map<std::string, Mix_Chunk*> sounds = {};

    void clearRenderer() const;
    void sortObjects();
    void renderObjects() const;
    void renderObject(H2DE_Object* object) const;
    void renderHitbox(const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute) const;

    static bool isPositionGreater(H2DE_Object* object1, H2DE_Object* object2);
    static bool isVisible(const H2DE_ColorRGB& color);
    const unsigned int getBlockSize() const;
    H2DE_AbsPos lvlToAbs(const H2DE_LevelPos& pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(const H2DE_LevelSize& size) const;

public:
    H2DE_Renderer(H2DE_Engine* engine, SDL_Renderer* renderer, std::vector<H2DE_Object*>& objects);
    ~H2DE_Renderer();

    void render();

    void destroyTextures();
    void destroySounds();

    void setTextures(std::unordered_map<std::string, SDL_Texture*> textures);
    void setSounds(std::unordered_map<std::string, Mix_Chunk*> sounds);
};

#endif
