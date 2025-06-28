#ifndef H2DE_RENDERER_H
#define H2DE_RENDERER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Renderer {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;
    std::vector<H2DE_Object*>& objects;
     
    std::vector<const H2DE_Object*> hitboxesBuffer = {}; 

    std::unordered_map<std::string, SDL_Texture*> textures = {};

    uint32_t objectsRendered = 0;
    uint32_t surfacesRendered = 0;
    uint32_t hitboxesRendered = 0;

    using R = H2DE_Renderer;

    using H2DE_SubPixelPos = H2DE_Vector2D<float>;
    using H2DE_SubPixelSize = H2DE_Vector2D<float>;
    using H2DE_SubPixelRect = H2DE_Rect<float>;

    H2DE_Renderer(H2DE_Engine* engine, SDL_Renderer* renderer, std::vector<H2DE_Object*>& objects) noexcept;
    ~H2DE_Renderer();

    void destroyTextures();

    void render();

    void clearRenderer() const;
    void resetCounts() noexcept;
    void sortObjects();

    void renderGrid();
    void renderCrosshair();

    void renderObjects();
    void renderObject(const H2DE_Object* object);
    inline void renderObjectAddHitboxesToBuffer(const H2DE_Object* object) {
        hitboxesBuffer.push_back(object);
    }

    void renderSurfaces(const H2DE_Object* object);
    void renderSurface(const H2DE_Object* object, H2DE_Surface* surface);

    void renderTexture(const H2DE_Object* object, H2DE_Surface* surface) const;
    void renderTextureSetProperties(const H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) const;
    void renderTextureRenderTexture(const H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) const;

    void renderColor(const H2DE_Object* object, H2DE_Surface* surface) const;

    SDL_Rect renderSurfaceGetWorldDestRect(const H2DE_Object* object, H2DE_Surface* surface) const;
    float renderSurfaceGetWorldRotation(const H2DE_Object* object, H2DE_Surface* surface) const noexcept;
    SDL_Point renderSurfaceGetLocalPivot(const H2DE_Object* object, H2DE_Surface* surface) const;
    static SDL_RendererFlip renderSurfaceGetWorldFlip(const H2DE_Object* object, H2DE_Surface* surface) noexcept;
    static std::optional<SDL_Rect> renderSurfaceGetPossibleSrcRect(H2DE_Surface* surface);

    void renderObjectsHitboxes();
    void renderHitboxes(const H2DE_Object* object);
    void renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute);

    const float getBlockSize(float width) const;
    const float getGameBlockSize() const;
    const float getInterfaceBlockSize() const;
    SDL_Texture* getTexture(const std::string& textureName) const;
    bool isSurfaceVisible(const H2DE_Surface* surface) const;
    static SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode) noexcept;
    static SDL_BlendMode getBlendMode(H2DE_BlendMode blendMode) noexcept;

    static constexpr float getOpacityBlend(uint8_t opacity) noexcept {
        return (static_cast<float>(opacity) / static_cast<float>(H2DE_UINT8_MAX));
    }

    H2DE_SubPixelPos levelToPixelPos(const H2DE_LevelRect& world_rect, bool absolute) const;
    H2DE_SubPixelPos levelToPixelPos(const H2DE_Translate& local_translate, bool absolute) const;
    H2DE_SubPixelSize levelToPixelSize(const H2DE_Scale& world_scale, bool absolute) const;
    H2DE_SubPixelRect levelToPixelRect(const H2DE_LevelRect& world_rect, bool absolute) const;

    H2DE_Translate pixelToLevel(const H2DE_PixelPos& pos, bool absolute) const;

    friend class H2DE_Engine;
    friend class H2DE_AssetLoaderManager;
};

#endif
