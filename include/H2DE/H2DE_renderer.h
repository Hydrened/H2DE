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
    std::unordered_map<std::string, H2DE_Font> fonts;
    bool debug = false;

    void destroyTextures();
    
    void clearRenderer() const;
    void sortObjects();

    void renderObjects() const;
    void renderObject(H2DE_Object* object) const;

    void renderSurfaces(H2DE_Object* object) const;
    void renderSurface(const H2DE_SurfaceBuffer& surfaceBuffer, const H2DE_LevelRect& rect, bool absolute) const;
    void rs_setTextureProperties(SDL_Texture* texture, const H2DE_ColorRGB& color, H2DE_ScaleMode scaleMode) const;
    SDL_Texture* rs_createTempTexture(const SDL_Rect& destRect) const;
    void rs_renderTempTexture(SDL_Texture* texture, const std::optional<SDL_Rect>& srcRect, float rotation, const SDL_Point* center, SDL_RendererFlip flip) const;
    void rs_renderFinalTexture(SDL_Texture* tempTexture, const SDL_Rect* destRect, float rotation, const SDL_Point* pivot) const;

    void renderHitboxes(const H2DE_Object* object) const;
    void renderHitbox(const H2DE_Object* object, const H2DE_LevelRect& objRect, const H2DE_Hitbox& hitbox, bool absolute) const;

    const bool isSurfaceValid(const H2DE_Surface* surface) const;

    const unsigned int getBlockSize() const;
    SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode) const;
    SDL_RendererFlip getFlip(H2DE_Flip flip) const;

    H2DE_LevelRect flipHitbox(const H2DE_LevelRect& objRect, const H2DE_LevelRect& hitboxRect, H2DE_Flip flip) const;

    H2DE_AbsPos lvlToAbsPos(const H2DE_LevelPos& pos, bool absolute) const;
    H2DE_AbsPos lvlToAbsPivot(const H2DE_LevelPos& pos) const;
    H2DE_AbsSize lvlToAbsSize(const H2DE_LevelSize& size) const;
    H2DE_AbsRect lvlToAbsRect(const H2DE_LevelRect& rect, bool absolute) const;

public:
    H2DE_Renderer(H2DE_Engine* engine, SDL_Renderer* renderer, std::vector<H2DE_Object*>& objects);
    ~H2DE_Renderer();

    void render();

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);
    friend void H2DE_InitFont(H2DE_Engine* engine, const std::string& name, const H2DE_Font& font);
    friend void H2DE_DebugObjects(const H2DE_Engine* engine, bool state);

    H2DE_LevelPos absToLvl(const H2DE_AbsPos& pos, bool absolute) const;

    const std::unordered_map<std::string, H2DE_Font>& getFonts() const;
};

#endif
