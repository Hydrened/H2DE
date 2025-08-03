#pragma once

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;
class H2DE_Surface;
class H2DE_Border;

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
    void renderBorder(const H2DE_Object* object, H2DE_Surface* surface) const;
    void renderRectangle(const H2DE_Object* object, H2DE_Border* border) const;
    void renderCircle(const H2DE_Object* object, H2DE_Border* border) const;

    SDL_Rect renderSurfaceGetWorldDestRect(const H2DE_Object* object, H2DE_Surface* surface) const;
    float renderSurfaceGetWorldRotation(const H2DE_Object* object, H2DE_Surface* surface) const noexcept;
    SDL_Point renderSurfaceGetLocalPivot(const H2DE_Object* object, H2DE_Surface* surface) const;
    static SDL_RendererFlip renderSurfaceGetWorldFlip(const H2DE_Object* object, H2DE_Surface* surface) noexcept;
    std::optional<SDL_Rect> renderSurfaceGetPossibleSrcRect(const H2DE_Object* object, H2DE_Surface* surface) const;

    void renderObjectsHitboxes();
    void renderHitboxes(const H2DE_Object* object);
    void renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute);

    void renderPixelRectangle(const H2DE_Object* object, const std::array<H2DE_PixelPos, 4>& corners, const H2DE_ColorRGB& color, bool filled) const;
    void renderPixelCircle(const H2DE_PixelPos& pos, int radiusW, int radiusH, const H2DE_ColorRGB& color, bool filled) const;

    const float getBlockSize(float width) const;
    const float getGameBlockSize() const;
    const float getInterfaceBlockSize() const;

    const std::array<H2DE_PixelPos, 4> getCorners(const H2DE_Object* object, H2DE_Surface* surface) const;

    SDL_Texture* getTexture(const std::string& textureName) const;
    bool isSurfaceVisible(const H2DE_Surface* surface) const;
    static SDL_ScaleMode getScaleMode(H2DE_ScaleMode scaleMode) noexcept;
    static SDL_BlendMode getBlendMode(H2DE_BlendMode blendMode) noexcept;

    static constexpr float getOpacityBlend(uint8_t opacity) noexcept {
        return (static_cast<float>(opacity) / static_cast<float>(H2DE_OPACITY_MAX));
    }

    H2DE_SubPixelPos levelToSubPixelPos(const H2DE_LevelRect& world_rect, bool absolute) const;
    H2DE_SubPixelSize levelToSubPixelSize(const H2DE_Scale& world_scale, bool absolute) const;
    H2DE_SubPixelRect levelToSubPixelRect(const H2DE_LevelRect& world_rect, bool absolute) const;

    H2DE_Translate pixelToLevel(const H2DE_PixelPos& pos, bool absolute) const;

    static H2DE_PixelRect subPixelToPixelRect(const H2DE_SubPixelRect& world_rect);

    friend class H2DE_Engine;
    friend class H2DE_AssetLoaderManager;
};
