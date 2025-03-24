#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <map>
#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Renderer {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;
    std::vector<H2DE_Object*>& objects;

    std::unordered_map<std::string, SDL_Texture*> textures;
    std::unordered_map<std::string, Mix_Chunk*> sounds;

    void destroyTextures();
    void destroySounds();
    
    void clearRenderer() const;
    void sortObjects();

    void renderObjects() const;
    void renderObject(H2DE_Object* object) const;
    void renderSurface(const H2DE_Surface* surface, const H2DE_LevelRect& rect, bool absolute) const;
    void renderHitbox(const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute) const;

    static bool isPositionGreater(H2DE_Object* object1, H2DE_Object* object2);
    static bool isVisible(const H2DE_ColorRGB& color);
    const unsigned int getBlockSize() const;
    SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode) const;
    SDL_RendererFlip getFlip(H2DE_Flip flip) const;
    H2DE_AbsPos lvlToAbs(const H2DE_LevelPos& pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(const H2DE_LevelSize& size) const;
    H2DE_AbsRect lvlToAbs(const H2DE_LevelRect& rect, bool absolute) const;

public:
    H2DE_Renderer(H2DE_Engine* engine, SDL_Renderer* renderer, std::vector<H2DE_Object*>& objects);
    ~H2DE_Renderer();

    void render();

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);
    H2DE_LevelPos absToLvl(const H2DE_AbsPos& pos, bool absolute) const;
};

#endif
