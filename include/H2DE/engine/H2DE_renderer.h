#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;
    std::vector<H2DE_Object*>& objects;
     
    std::vector<H2DE_Object*> hitboxesBuffer = {}; 

    std::unordered_map<std::string, SDL_Texture*> textures = {};
    std::unordered_map<std::string, H2DE_Font> fonts = {};

    using R = H2DE_Renderer;

    H2DE_Renderer(H2DE_Engine* engine, SDL_Renderer* renderer, std::vector<H2DE_Object*>& objects);
    ~H2DE_Renderer();

    void destroyTextures();

    void render();

    void clearRenderer() const;
    void sortObjects();

    void renderObjects();
    void renderObject(H2DE_Object* object);
    inline void renderObjectAddHitboxesToBuffer(H2DE_Object* object) { hitboxesBuffer.push_back(object); }

    void renderSurfaces(H2DE_Object* object);
    void renderSurface(H2DE_Object* object, H2DE_Surface* surface);
    void renderSurfaceSetProperties(H2DE_Surface* surface, SDL_Texture* texture);
    void renderSurfaceRenderTexture(H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture);
    static H2DE_LevelRect renderSurfaceGetWorldDestRect(H2DE_Object* object, H2DE_Surface* surface);
    static float renderSurfaceGetWorldRotation(H2DE_Object* object, H2DE_Surface* surface);
    static SDL_RendererFlip renderSurfaceGetWorldFlip(H2DE_Object* object, H2DE_Surface* surface);
    static std::optional<H2DE_PixelRect> renderSurfaceGetPossibleSrcRect(H2DE_Surface* surface);

    void renderObjectsHitboxes();
    void renderHitboxes(const H2DE_Object* object);
    void renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute);

    const float getBlockSize(float width) const;
    const float getGameBlockSize() const;
    const float getInterfaceBlockSize() const;
    bool isSurfaceVisible(H2DE_Surface* surface);
    static SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode);
    static SDL_BlendMode getBlendMode(H2DE_BlendMode blendMode);

    H2DE_PixelPos levelToPixelPos(const H2DE_LevelRect& world_rect, bool absolute) const;
    H2DE_PixelPos levelToPixelPos(const H2DE_Translate& local_translate, bool absolute) const;
    H2DE_PixelSize levelToPixelSize(const H2DE_Scale& world_scale, bool absolute) const;
    H2DE_PixelRect levelToPixelRect(const H2DE_LevelRect& world_rect, bool absolute) const;

    H2DE_Translate pixelToLevel(const H2DE_PixelPos& pos, bool absolute) const;

public:
    friend class H2DE_Engine;
    friend class H2DE_AssetLoaderManager;
};

#endif
