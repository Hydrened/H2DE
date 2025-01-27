#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <SDL2/SDL2_gfxPrimitives.h>
#include "H2DE_engine.h"
class H2DE_Engine;
struct H2DE_LevelObject;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    std::unordered_map<std::string, SDL_Texture*>* textures;
    std::vector<H2DE_LevelObject*>* objects;

    int getBlockSize() const;
    SDL_RendererFlip getFlip(H2DE_Flip flip);
    void whileParent(H2DE_LevelObject* object, std::function<void(H2DE_LevelObject*)> call) const;

    // H2DE_LevelPos getTranslatedPos(H2DE_GraphicObject* object) const;
    // H2DE_LevelPos getScaledPos(H2DE_GraphicObject* object) const;
    
    H2DE_AbsPos lvlToAbs(H2DE_LevelPos pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(H2DE_LevelSize size) const;

    void renderObject(H2DE_LevelObject* object);
    void renderTexture(H2DE_LevelObject* object);
    void renderHitbox(H2DE_LevelObject* object);

public:
    H2DE_Renderer(H2DE_Engine* engine, std::unordered_map<std::string, SDL_Texture*>* textures, std::vector<H2DE_LevelObject*>* objects);
    ~H2DE_Renderer();

    void update();
    void render();
};

#endif
