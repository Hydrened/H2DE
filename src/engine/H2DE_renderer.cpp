#include "H2DE/engine/H2DE_renderer.h"
#include "H2DE/engine/H2DE_geometry.h"

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) : engine(e), renderer(r), objects(o) {

}

// CLEANUP
H2DE_Renderer::~H2DE_Renderer() {
    destroyTextures();
}

void H2DE_Renderer::destroyTextures() {
    for (const auto& [name, texture] : textures) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    textures.clear();
}

void H2DE_Renderer::resetCounts() {
    objectsRendered = 0;
    surfacesRendered = 0;
    hitboxesRendered = 0;
}

// RENDER
void H2DE_Renderer::render() {
    clearRenderer();
    resetCounts();
    sortObjects();
    renderObjects();
}

void H2DE_Renderer::clearRenderer() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// -- objects
void H2DE_Renderer::sortObjects() {
    std::sort(objects.begin(), objects.end(), [](H2DE_Object* a, H2DE_Object* b) {
        bool absoluteA = a->isAbsolute();
        bool absoluteB = b->isAbsolute();

        if (absoluteA != absoluteB) {
            return absoluteB;
        }

        int indexA = a->getIndex();
        int indexB = b->getIndex();

        if (indexA == indexB) {
            return H2DE_Engine::_isTranslateGreater(a, b);
        }

        return (indexA < indexB);
    });
}

void H2DE_Renderer::renderObjects() {
    bool gridOnTop = engine->_camera->_onTop;

    if (!gridOnTop) {
        renderGrid();
    }

    for (const H2DE_Object* object : objects) {
        renderObject(object);
    }

    renderObjectsHitboxes();
    if (gridOnTop) {
        renderGrid();
    }
    renderCrosshair();

    SDL_RenderPresent(renderer);
}

void H2DE_Renderer::renderObject(const H2DE_Object* object) {
    if (object->isHidden()) {
        return;
    }

    if (!engine->_camera->containsObject(object) && !object->_objectData.absolute) {
        return;
    }

    if (!object->_isGrid) {
        objectsRendered++;
    }

    renderSurfaces(object);

    if (engine->_debugObjectEnabled && !object->_isGrid) {
        renderObjectAddHitboxesToBuffer(object);
    }
}

// -- grid
void H2DE_Renderer::renderGrid() {
    if (!engine->_camera->getData().grid) {
        return;
    }

    H2DE_Object* grid = engine->_camera->_grid;

    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    for (const auto& [name, hitbox] : grid->_hitboxes) {
        if (name.substr(0, 1) == "c") {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(grid, hitbox, xIsInverted, yIsInverted);
        renderHitbox(world_hitboxRect, hitbox.color, grid->isAbsolute());
    }
}

void H2DE_Renderer::renderCrosshair() {
    if (!engine->_camera->getData().grid) {
        return;
    }

    H2DE_Object* grid = engine->_camera->_grid;

    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    const H2DE_Hitbox& crosshairHitboxW = grid->getHitbox("cw");
    const H2DE_Hitbox& crosshairHitboxH = grid->getHitbox("ch");

    bool grisIsAbsolute = grid->isAbsolute();

    const H2DE_LevelRect world_crosshairHitboxRectW = G::getHitboxRect(grid, crosshairHitboxW, xIsInverted, yIsInverted);
    const H2DE_LevelRect world_crosshairHitboxRectH = G::getHitboxRect(grid, crosshairHitboxH, xIsInverted, yIsInverted);
    renderHitbox(world_crosshairHitboxRectW, crosshairHitboxW.color, grisIsAbsolute);
    renderHitbox(world_crosshairHitboxRectH, crosshairHitboxH.color, grisIsAbsolute);
}

// -- surfaces
void H2DE_Renderer::renderSurfaces(const H2DE_Object* object) {
    for (H2DE_Surface* surface : object->_surfaceBuffers) {
        if (isSurfaceVisible(surface)) {
            renderSurface(object, surface);
        }
    }
}

void H2DE_Renderer::renderSurface(const H2DE_Object* object, H2DE_Surface* surface) {
    surfacesRendered++;

    bool isColor = (dynamic_cast<H2DE_Color*>(surface) != H2DE_NULL_SURFACE);
    bool isBorder = (dynamic_cast<H2DE_Border*>(surface) != H2DE_NULL_SURFACE);

    if (isColor) {
        renderColor(object, surface);
    } else if (isBorder) {
        renderBorder(object, surface);
    } else {
        renderTexture(object, surface);
    }
}

// -- -- pixel
void H2DE_Renderer::renderPixelRectangle(const H2DE_Object* object, const std::array<H2DE_PixelPos, 4>& corners, const H2DE_ColorRGB& color, bool filled) const {
    const std::vector<Sint16> vx = {
        static_cast<Sint16>(corners[0].x), static_cast<Sint16>(corners[1].x - 1), static_cast<Sint16>(corners[2].x - 1), static_cast<Sint16>(corners[3].x)
    };
    const std::vector<Sint16> vy = {
        static_cast<Sint16>(corners[0].y), static_cast<Sint16>(corners[1].y), static_cast<Sint16>(corners[2].y - 1), static_cast<Sint16>(corners[3].y - 1)
    };

    H2DE_ColorRGB surfaceColor = color;
    surfaceColor.a = H2DE::round((getOpacityBlend(surfaceColor.a) * getOpacityBlend(object->_objectData.opacity)) * static_cast<float>(H2DE_OPACITY_MAX));

    if (filled) {
        filledPolygonColor(renderer, vx.data(), vy.data(), vx.size(), static_cast<Uint32>(surfaceColor));
    } else {
        polygonColor(renderer, vx.data(), vy.data(), vx.size(), static_cast<Uint32>(surfaceColor));
    }
}

void H2DE_Renderer::renderPixelCircle(const H2DE_PixelPos& pos, int radiusW, int radiusH, const H2DE_ColorRGB& color, bool filled) const {
    if (filled) {
        filledEllipseColor(renderer, pos.x, pos.y, radiusW, radiusH, static_cast<Uint32>(color));
    } else {
        ellipseColor(renderer, pos.x, pos.y, radiusW, radiusH, static_cast<Uint32>(color));
    }
}

// -- -- textures
void H2DE_Renderer::renderTexture(const H2DE_Object* object, H2DE_Surface* surface) const {
    SDL_Texture* texture = getTexture(surface->getTextureName());
    if (texture == nullptr) {
        return;
    }
    
    renderTextureSetProperties(object, surface, texture);
    renderTextureRenderTexture(object, surface, texture);
}

void H2DE_Renderer::renderTextureSetProperties(const H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) const {
    const H2DE_ColorRGB color = surface->getColor();
    const SDL_ScaleMode scaleMode = R::getScaleMode(surface->_surfaceData.scaleMode);
    const SDL_BlendMode blendMode = R::getBlendMode(surface->_surfaceData.blendMode);

    uint8_t opacity = H2DE::round((getOpacityBlend(color.a) * getOpacityBlend(object->_objectData.opacity)) * static_cast<float>(H2DE_OPACITY_MAX));

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, opacity);
    SDL_SetTextureBlendMode(texture, blendMode);
    SDL_SetTextureScaleMode(texture, scaleMode);
}

void H2DE_Renderer::renderTextureRenderTexture(const H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) const {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, surface);
    float world_surfaceRotation = R::renderSurfaceGetWorldRotation(object, surface);
    SDL_Point local_surfaceCenter = R::renderSurfaceGetLocalPivot(object, surface);
    SDL_RendererFlip world_surfaceFlip = R::renderSurfaceGetWorldFlip(object, surface);

    std::optional<SDL_Rect> possibleSrcRect = renderSurfaceGetPossibleSrcRect(object, surface);
    SDL_Rect* srcRect = ((possibleSrcRect.has_value()) ? &possibleSrcRect.value() : nullptr);

    SDL_RenderCopyEx(renderer, texture, srcRect, &world_surfaceRect, world_surfaceRotation, &local_surfaceCenter, world_surfaceFlip);
}

// -- -- colors
void H2DE_Renderer::renderColor(const H2DE_Object* object, H2DE_Surface* surface) const {
    renderPixelRectangle(object, getCorners(object, surface), surface->getColor(), true);
}

// -- -- border
void H2DE_Renderer::renderBorder(const H2DE_Object* object, H2DE_Surface* surface) const {
    H2DE_Border* border = dynamic_cast<H2DE_Border*>(surface);

    if (border == H2DE_NULL_SURFACE) {
        return;
    }

    if (border->getType() == H2DE_BORDER_TYPE_RECTANGLE) {
        renderRectangle(object, border);
    } else {
        renderCircle(object, border);
    }
}

void H2DE_Renderer::renderRectangle(const H2DE_Object* object, H2DE_Border* border) const {
    std::array<H2DE_PixelPos, 4> corners = getCorners(object, border);

    const uint16_t& thickness = border->getThickness();

    const H2DE_BorderPlacement& placement = border->getPlacement();
    int placementOffset = (placement == H2DE_BORDER_PLACEMENT_CENTER) ? H2DE::round(thickness * 0.5f) :
        (placement == H2DE_BORDER_PLACEMENT_OUTER) ? thickness : 0;

    corners[0] += { -placementOffset, -placementOffset };
    corners[1] += { placementOffset, -placementOffset };
    corners[2] += { placementOffset, placementOffset };
    corners[3] += { -placementOffset, placementOffset };

    for (int i = 0; i < thickness; i++) {
        corners[0] += { 1, 1 };
        corners[1] += { -1, 1 };
        corners[2] += { -1, -1 };
        corners[3] += { 1, -1 };

        if (corners.at(0).x > corners.at(1).x) {
            break;
        }

        if (corners.at(0).y > corners.at(3).y) {
            break;
        }

        renderPixelRectangle(object, corners, border->getColor(), border->isFilled());
    }
}

void H2DE_Renderer::renderCircle(const H2DE_Object* object, H2DE_Border* border) const {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, border);

    const int halfWidth = static_cast<int>(H2DE::round(world_surfaceRect.w * 0.5f));
    const int halfHeight = static_cast<int>(H2DE::round(world_surfaceRect.h * 0.5f));

    const H2DE_PixelPos center = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight };

    H2DE_ColorRGB surfaceColor = border->getColor();
    surfaceColor.a = H2DE::round((getOpacityBlend(surfaceColor.a) * getOpacityBlend(object->_objectData.opacity)) * static_cast<float>(H2DE_OPACITY_MAX));

    bool isFilled = border->isFilled();

    const uint16_t& thickness = border->getThickness();

    const H2DE_BorderPlacement& placement = border->getPlacement();
    int placementOffset = (placement == H2DE_BORDER_PLACEMENT_CENTER) ? H2DE::round(thickness * 0.5f) :
        (placement == H2DE_BORDER_PLACEMENT_OUTER) ? thickness : 0;

    int radiusW = halfWidth + placementOffset;
    int radiusH = halfHeight + placementOffset;

    for (int i = 0; i < thickness; i++) {
        radiusW--;
        radiusH--;
        
        renderPixelCircle(center, radiusW, radiusH, surfaceColor, isFilled);
    }
}

// -- -- getters
SDL_Rect H2DE_Renderer::renderSurfaceGetWorldDestRect(const H2DE_Object* object, H2DE_Surface* surface) const {
    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    H2DE_LevelRect surfaceRect = G::getSurfaceRect(object, surface, xIsInverted, yIsInverted);
    const H2DE_BarObject* bar = dynamic_cast<const H2DE_BarObject*>(object);

    if (bar != H2DE_NULL_OBJECT) {
        if (bar->_isSurfaceFill(surface)) {
            float fillBarBlend = bar->_getFillBlend();
            surfaceRect.x = (surfaceRect.w - surfaceRect.multiplyW(fillBarBlend).w) * -0.5f;
            surfaceRect = surfaceRect.multiplyW(fillBarBlend);
        }
    }

    return static_cast<SDL_Rect>(subPixelToPixelRect(levelToSubPixelRect(surfaceRect, object->_objectData.absolute)));
}

float H2DE_Renderer::renderSurfaceGetWorldRotation(const H2DE_Object* object, H2DE_Surface* surface) const {
    return G::getSurfaceRotation(object, surface);
}

SDL_Point H2DE_Renderer::renderSurfaceGetLocalPivot(const H2DE_Object* object, H2DE_Surface* surface) const {
    bool objIsAbsolute = object->_objectData.absolute;
    const H2DE_SubPixelSize pixel_surfaceScale = R::levelToSubPixelSize(surface->_surfaceData.transform.scale * 0.5f, objIsAbsolute);

    return static_cast<SDL_Point>(pixel_surfaceScale);
}

SDL_RendererFlip H2DE_Renderer::renderSurfaceGetWorldFlip(const H2DE_Object* object, H2DE_Surface* surface) {
    H2DE_Flip objFlip = G::getFlipFromScale(object->_objectData.transform.scale);
    H2DE_Flip surFlip = G::getFlipFromScale(surface->_surfaceData.transform.scale);
    H2DE_Flip addedFlip = G::addFlip(objFlip, surFlip);

    switch (addedFlip) {
        case H2DE_FLIP_X: return SDL_FLIP_HORIZONTAL;
        case H2DE_FLIP_Y: return SDL_FLIP_VERTICAL;
        default: return SDL_FLIP_NONE;
    }
}

std::optional<SDL_Rect> H2DE_Renderer::renderSurfaceGetPossibleSrcRect(const H2DE_Object* object, H2DE_Surface* surface) const {
    const H2DE_BarObject* bar = dynamic_cast<const H2DE_BarObject*>(object);
    std::optional<H2DE_PixelRect> possibleSrcRect = surface->getSrcRect();

    if (bar == H2DE_NULL_OBJECT && !possibleSrcRect) {
        return std::nullopt;
    }

    float blend = 1.0f;
    if (bar != H2DE_NULL_OBJECT) {
        if (bar->_isSurfaceFill(surface)) {
            blend = bar->_getFillBlend();
        }
    }

    H2DE_PixelRect src = !possibleSrcRect.has_value()
        ? H2DE_PixelPos{ 0, 0 }.makeRect(engine->getTextureSize(surface->getTextureName()))
        : possibleSrcRect.value();

    src = src.multiplyW(blend);
    return SDL_Rect{ src.x, src.y, src.w, src.h };
}

// -- hitboxes
void H2DE_Renderer::renderObjectsHitboxes() {
    for (const H2DE_Object* object : hitboxesBuffer) {
        if (object->getHitboxes().size() != 0) {
            renderHitboxes(object);
        }
    }
        
    hitboxesBuffer.clear();
}

void H2DE_Renderer::renderHitboxes(const H2DE_Object* object) {
    const H2DE_Translate objectTranslate = object->getTranslate();
    bool objectIsAbsolute = object->isAbsolute();

    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    const std::vector<int>& collisionIndexesToDebug = engine->_debugHitboxCollisionIndexes;

    for (const auto& [name, hitbox] : object->getHitboxes()) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(object, hitbox, xIsInverted, yIsInverted);
        if (!engine->_camera->containsRect(world_hitboxRect) && !objectIsAbsolute) {
            continue;
        }

        bool debugHitbox = (std::ranges::find(collisionIndexesToDebug, hitbox.collisionIndex) != collisionIndexesToDebug.end() || collisionIndexesToDebug.empty());
        if (!debugHitbox) {
            continue;
        }

        if (!object->_isGrid) {
            hitboxesRendered++;
        }

        renderHitbox(world_hitboxRect, hitbox.color, objectIsAbsolute);
    }
}

void H2DE_Renderer::renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute) {
    SDL_Rect absRect = static_cast<SDL_Rect>(subPixelToPixelRect(levelToSubPixelRect(world_hitboxRect, absolute)));

    Sint16 minX = absRect.x;
    Sint16 maxX = absRect.x + absRect.w - 1;
    Sint16 minY = absRect.y;
    Sint16 maxY = absRect.y + absRect.h - 1;

    const std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    const std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(color));
}

// GETTER
const float H2DE_Renderer::getBlockSize(float width) const {
    float blockSize = engine->_window->getSize().x / width;

    if (blockSize <= 0.0f) {
        blockSize = 0.1f;
    }

    return blockSize;
}

const float H2DE_Renderer::getGameBlockSize() const {
    return getBlockSize(engine->_camera->getGameWidth());
}

const float H2DE_Renderer::getInterfaceBlockSize() const {
    return getBlockSize(engine->_camera->getInterfaceWidth());
}

const std::array<H2DE_PixelPos, 4> H2DE_Renderer::getCorners(const H2DE_Object* object, H2DE_Surface* surface) const {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, surface);
    float world_surfaceRotation = R::renderSurfaceGetWorldRotation(object, surface);
    
    const int halfWidth = static_cast<int>(H2DE::round(world_surfaceRect.w * 0.5f));
    const int halfHeight = static_cast<int>(H2DE::round(world_surfaceRect.h * 0.5f));

    const H2DE_PixelRect world_pixel_surfaceRect = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight, world_surfaceRect.w, world_surfaceRect.h };
    const H2DE_PixelPivot world_pixel_pivot = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight };

    std::array<H2DE_PixelPos, 4> corners = world_pixel_surfaceRect.getCorners();
    for (H2DE_PixelPos& corner : corners) {
        corner = corner.rotate(world_pixel_pivot, world_surfaceRotation);
    }

    return corners;
}

SDL_Texture* H2DE_Renderer::getTexture(const std::string& textureName) const {
    auto it = (textures.find(textureName));
    if (it != textures.end()) {
        return it->second;    
    }

    return nullptr;
}

bool H2DE_Renderer::isSurfaceVisible(const H2DE_Surface* surface) const {
    bool surfaceIsValid = (textures.find(surface->getTextureName()) != textures.end() || surface->getTextureName() == "/");
    return (surfaceIsValid && surface->_isVisible());
}

// -- sdl getters
SDL_ScaleMode H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) {
    switch (scaleMode) {
        case H2DE_SCALE_MODE_BEST: return SDL_ScaleModeBest;
        case H2DE_SCALE_MODE_LINEAR: return SDL_ScaleModeLinear;
        default: return SDL_ScaleModeNearest;
    }
}

SDL_BlendMode H2DE_Renderer::getBlendMode(H2DE_BlendMode blendMode) {
    switch (blendMode) {
        case H2DE_BLEND_MODE_BLEND: return SDL_BLENDMODE_BLEND;
        case H2DE_BLEND_MODE_ADD: return SDL_BLENDMODE_ADD;
        case H2DE_BLEND_MODE_MOD: return SDL_BLENDMODE_MOD;
        case H2DE_BLEND_MODE_MUL: return SDL_BLENDMODE_MUL;
        case H2DE_BLEND_MODE_INVALID: return SDL_BLENDMODE_INVALID;
        default: return SDL_BLENDMODE_NONE;
    }
}

// -- level to sub pixel
H2DE_Renderer::H2DE_SubPixelPos H2DE_Renderer::levelToSubPixelPos(const H2DE_LevelRect& world_rect, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();

    H2DE_LevelRect world_cameraRect = engine->_camera->getGameWorldRect();
    H2DE_Translate world_translate = world_rect.getTranslate();

    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    if (xIsInverted) {
        world_cameraRect.x *= -1;
    }

    if (yIsInverted) {
        world_cameraRect.y *= -1;
    }

    if (absolute) {
        world_translate += engine->_camera->getInterfaceScale() * 0.5f;
    } else {
        world_translate += world_cameraRect.getScale() * 0.5f - world_cameraRect.getTranslate();
    }
    
    return {
        world_translate.x * blockSize,
        world_translate.y * blockSize,
    };
}

H2DE_Renderer::H2DE_SubPixelSize H2DE_Renderer::levelToSubPixelSize(const H2DE_Scale& world_scale, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();

    return {
        H2DE::abs(world_scale.x) * blockSize,
        H2DE::abs(world_scale.y) * blockSize,
    };
}

H2DE_Renderer::H2DE_SubPixelRect H2DE_Renderer::levelToSubPixelRect(const H2DE_LevelRect& world_rect, bool absolute) const {
    const H2DE_SubPixelPos pos = levelToSubPixelPos(world_rect, absolute);
    const H2DE_SubPixelSize size = levelToSubPixelSize(world_rect.getScale(), absolute);
    return H2DE_SubPixelRect{ pos.x, pos.y, size.x, size.y };
}

// -- pixel to level
H2DE_Translate H2DE_Renderer::pixelToLevel(const H2DE_PixelPos& pos, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();
    H2DE_Translate res = H2DE_Translate{ static_cast<float>(pos.x), static_cast<float>(pos.y) };

    bool xIsInverted = engine->_camera->isXOriginInverted();
    bool yIsInverted = engine->_camera->isYOriginInverted();

    const H2DE_Scale cameraHalfScale = (absolute)
        ? engine->_camera->getInterfaceScale() * 0.5f
        : engine->_camera->getGameScale() * 0.5f;

    res /= blockSize;
    res -= cameraHalfScale;

    if (absolute) {
        if (xIsInverted) {
            res.x *= -1.0f;
        }

        if (yIsInverted) {
            res.y *= -1.0f;
        }

    } else {
        H2DE_Translate cameraTranslate = engine->_camera->getTranslate();

        if (xIsInverted) {
            cameraTranslate.x *= -1.0f;
        }

        if (yIsInverted) {
            cameraTranslate.y *= -1.0f;
        }

        res += cameraTranslate;

        if (xIsInverted) {
            res.x *= -1.0f;
        }

        if (yIsInverted) {
            res.y *= -1.0f;
        }
    }

    return res;
}

// -- sub pixel to pixel
H2DE_PixelRect H2DE_Renderer::subPixelToPixelRect(const H2DE_SubPixelRect& world_rect) {
    return { H2DE::round(world_rect.x), H2DE::round(world_rect.y), H2DE::round(world_rect.w), H2DE::round(world_rect.h) };
}
