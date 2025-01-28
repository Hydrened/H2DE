#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <H2DE_engine.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <functional>
#include <unordered_map>
class H2DE_Engine;
struct H2DE_LevelObject;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    std::unordered_map<std::string, SDL_Texture*>* textures;
    std::vector<H2DE_LevelObject*>* objects;

    int getBlockSize() const;
    SDL_RendererFlip getFlip(H2DE_Flip flip);
    SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode);
    void whileParent(H2DE_LevelObjectData data, std::function<void(H2DE_LevelObjectData)> call) const;
    
    H2DE_AbsPos lvlToAbs(H2DE_LevelPos pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(H2DE_LevelSize size) const;

    void renderObject(H2DE_LevelObject* object);
    void renderTexture(H2DE_LevelObjectData data);
    void renderHitboxes(H2DE_LevelObjectData data);

public:
    H2DE_Renderer(H2DE_Engine* engine, std::unordered_map<std::string, SDL_Texture*>* textures, std::vector<H2DE_LevelObject*>* objects);
    ~H2DE_Renderer();

    void update();
    void render();
};

#endif
