#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include "H2DE_engine.h"
class H2DE_Engine;
struct H2DE_GraphicObject;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    std::unordered_map<std::string, SDL_Texture*>* textures;
    std::unordered_map<std::string, Mix_Chunk*>* sounds;
    std::vector<H2DE_GraphicObject*>* objects;

    void renderImage(H2DE_GraphicObject* object);
    void renderPolygon(H2DE_GraphicObject* object);
    void renderCircle(H2DE_GraphicObject* object);

    int getBlockSize() const;
    H2DE_LevelPos getPos(H2DE_GraphicObject* object) const;
    SDL_RendererFlip getFlip(H2DE_GraphicObject* object) const;
    void whileParent(H2DE_GraphicObject* object, std::function<void(H2DE_GraphicObject*)> call) const;

    H2DE_LevelPos getTranslatedPos(H2DE_GraphicObject* object) const;
    H2DE_LevelPos getScaledPos(H2DE_GraphicObject* object) const;
    
    H2DE_AbsPos lvlToAbs(H2DE_LevelPos pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(H2DE_LevelSize size) const;

public:
    H2DE_Renderer(H2DE_Engine* engine, std::unordered_map<std::string, SDL_Texture*>* textures, std::unordered_map<std::string, Mix_Chunk*>* sounds, std::vector<H2DE_GraphicObject*>* objects);
    ~H2DE_Renderer();

    void update();
    void render();
};

#endif
