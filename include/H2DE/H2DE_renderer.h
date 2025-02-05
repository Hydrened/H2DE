#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <H2DE_engine.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <functional>
#include <unordered_map>
class H2DE_Engine;
struct H2DE_LevelObject;
struct H2DE_Button;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    std::unordered_map<std::string, SDL_Texture*>* textures;
    std::vector<H2DE_LevelObject*>* objects;
    std::vector<H2DE_Button*>* buttons;

    bool debug = false;
    int renderedObjects = 0;

    SDL_RendererFlip getFlip(H2DE_Flip flip);
    SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode);
    H2DE_LevelPos getPosFromParents(H2DE_LevelObjectData data) const;

    static bool isPositionGreater(H2DE_LevelObject* obj1, H2DE_LevelObject* obj2);
    static void whileParent(H2DE_LevelObjectData* data, std::function<void(H2DE_LevelObjectData*)> call);
    
    H2DE_AbsPos lvlToAbs(H2DE_LevelPos pos, bool absolute) const;
    H2DE_AbsSize lvlToAbs(H2DE_LevelSize size) const;

    void renderObject(H2DE_LevelObject* object);
    void renderObjectTexture(H2DE_LevelObjectData data);
    void renderObjectHitboxes(H2DE_LevelObjectData data);
    
    void renderButton(H2DE_Button* button);
    void renderButtonTexture(H2DE_ButtonData data);

public:
    H2DE_Renderer(H2DE_Engine* engine, std::unordered_map<std::string, SDL_Texture*>* textures, std::vector<H2DE_LevelObject*>* objects, std::vector<H2DE_Button*>* buttons);
    ~H2DE_Renderer();

    void debugObjects(bool state);

    void render();
    
    int getBlockSize() const;
};

#endif
