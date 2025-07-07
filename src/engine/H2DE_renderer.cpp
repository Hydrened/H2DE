#include "H2DE/engine/H2DE_renderer.h"
#include "H2DE/engine/H2DE_geometry.h"
#undef max

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) noexcept : engine(e), renderer(r), objects(o) {

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

void H2DE_Renderer::resetCounts() noexcept {
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
            return H2DE_Engine::isPositionGreater(a, b);
        }

        return (indexA < indexB);
    });
}

void H2DE_Renderer::renderObjects() {
    bool gridOnTop = engine->camera->onTop;

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

    if (!engine->camera->containsObject(object) && !object->objectData.absolute) {
        return;
    }

    if (!object->isGrid) {
        objectsRendered++;
    }

    renderSurfaces(object);

    if (engine->debugObjectEnabled && !object->isGrid) {
        renderObjectAddHitboxesToBuffer(object);
    }
}

// -- grid
void H2DE_Renderer::renderGrid() {
    if (!engine->camera->getData().grid) {
        return;
    }

    H2DE_Object* grid = engine->camera->grid;

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    for (const auto& [name, hitbox] : grid->hitboxes) {
        if (name.substr(0, 1) == "c") {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(grid, hitbox, xIsInverted, yIsInverted);
        renderHitbox(world_hitboxRect, hitbox.color, grid->isAbsolute());
    }
}

void H2DE_Renderer::renderCrosshair() {
    if (!engine->camera->getData().grid) {
        return;
    }

    H2DE_Object* grid = engine->camera->grid;

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

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
    for (H2DE_Surface* surface : object->surfaceBuffers) {
        if (isSurfaceVisible(surface)) {
            renderSurface(object, surface);
        }
    }
}

void H2DE_Renderer::renderSurface(const H2DE_Object* object, H2DE_Surface* surface) {
    surfacesRendered++;

    bool isColor = (dynamic_cast<H2DE_Color*>(surface) != nullptr);
    bool isBorder = (dynamic_cast<H2DE_Border*>(surface) != nullptr);

    if (isColor) {
        renderColor(object, surface);
    } else if (isBorder) {
        renderBorder(object, surface);
    } else {
        renderTexture(object, surface);
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
    const SDL_ScaleMode scaleMode = R::getScaleMode(surface->surfaceData.scaleMode);
    const SDL_BlendMode blendMode = R::getBlendMode(surface->surfaceData.blendMode);

    uint8_t opacity = H2DE::round((getOpacityBlend(color.a) * getOpacityBlend(object->objectData.opacity)) * static_cast<float>(H2DE_UINT8_MAX));

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

    std::optional<SDL_Rect> possibleSrcRect = renderSurfaceGetPossibleSrcRect(surface);
    SDL_Rect* srcRect = ((possibleSrcRect.has_value()) ? &possibleSrcRect.value() : nullptr);

    SDL_RenderCopyEx(renderer, texture, srcRect, &world_surfaceRect, world_surfaceRotation, &local_surfaceCenter, world_surfaceFlip);
}

// -- -- colors
void H2DE_Renderer::renderColor(const H2DE_Object* object, H2DE_Surface* surface) const {
    renderPolygon(object, surface, false);
}

// -- -- border
void H2DE_Renderer::renderBorder(const H2DE_Object* object, H2DE_Surface* surface) const {
    H2DE_Border* border = dynamic_cast<H2DE_Border*>(surface);

    if (border == nullptr) {
        return;
    }

    if (border->getType() == H2DE_BORDER_TYPE_RECTANGLE) {
        renderRectangle(object, border);
    } else {
        renderCircle(object, border);
    }
}

void H2DE_Renderer::renderRectangle(const H2DE_Object* object, H2DE_Border* border) const {
    renderPolygon(object, border, border->isFilled());
}

void H2DE_Renderer::renderCircle(const H2DE_Object* object, H2DE_Border* border) const {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, border);

    const int halfWidth = static_cast<int>(H2DE::round(world_surfaceRect.w * 0.5f));
    const int halfHeight = static_cast<int>(H2DE::round(world_surfaceRect.h * 0.5f));

    SDL_Point center = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight };

    H2DE_ColorRGB surfaceColor = border->getColor();
    surfaceColor.a = H2DE::round((getOpacityBlend(surfaceColor.a) * getOpacityBlend(object->objectData.opacity)) * static_cast<float>(H2DE_UINT8_MAX));

    if (border->isFilled()) {
        filledEllipseColor(renderer, center.x, center.y, halfWidth, halfHeight, static_cast<Uint32>(surfaceColor));
    } else {
        ellipseColor(renderer, center.x, center.y, halfWidth, halfHeight, static_cast<Uint32>(surfaceColor));
    }
}

void H2DE_Renderer::renderPolygon(const H2DE_Object* object, H2DE_Surface* surface, bool filled) const {
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

    const std::vector<Sint16> vx = {
        static_cast<Sint16>(corners[0].x), static_cast<Sint16>(corners[1].x - 1), static_cast<Sint16>(corners[2].x - 1), static_cast<Sint16>(corners[3].x)
    };
    const std::vector<Sint16> vy = {
        static_cast<Sint16>(corners[0].y), static_cast<Sint16>(corners[1].y), static_cast<Sint16>(corners[2].y - 1), static_cast<Sint16>(corners[3].y - 1)
    };

    H2DE_ColorRGB surfaceColor = surface->getColor();
    surfaceColor.a = H2DE::round((getOpacityBlend(surfaceColor.a) * getOpacityBlend(object->objectData.opacity)) * static_cast<float>(H2DE_UINT8_MAX));

    if (filled) {
        filledPolygonColor(renderer, vx.data(), vy.data(), corners.size(), static_cast<Uint32>(surfaceColor));
    } else {
        polygonColor(renderer, vx.data(), vy.data(), corners.size(), static_cast<Uint32>(surfaceColor));
    }
}

// -- -- getters
SDL_Rect H2DE_Renderer::renderSurfaceGetWorldDestRect(const H2DE_Object* object, H2DE_Surface* surface) const {
    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    const H2DE_LevelRect surfaceRect = G::getSurfaceRect(object, surface, xIsInverted, yIsInverted);
    return static_cast<SDL_Rect>(subPixelToPixelRect(levelToSubPixelRect(surfaceRect, object->objectData.absolute)));
}

float H2DE_Renderer::renderSurfaceGetWorldRotation(const H2DE_Object* object, H2DE_Surface* surface) const noexcept {
    return G::getSurfaceRotation(object, surface);
}

SDL_Point H2DE_Renderer::renderSurfaceGetLocalPivot(const H2DE_Object* object, H2DE_Surface* surface) const {
    bool objIsAbsolute = object->objectData.absolute;
    const H2DE_SubPixelSize pixel_surfaceScale = R::levelToSubPixelSize(surface->surfaceData.transform.scale * 0.5f, objIsAbsolute);

    return static_cast<SDL_Point>(pixel_surfaceScale);
}

SDL_RendererFlip H2DE_Renderer::renderSurfaceGetWorldFlip(const H2DE_Object* object, H2DE_Surface* surface) noexcept {
    H2DE_Flip objFlip = G::getFlipFromScale(object->objectData.transform.scale);
    H2DE_Flip surFlip = G::getFlipFromScale(surface->surfaceData.transform.scale);
    H2DE_Flip addedFlip = G::addFlip(objFlip, surFlip);

    switch (addedFlip) {
        case H2DE_FLIP_X: return SDL_FLIP_HORIZONTAL;
        case H2DE_FLIP_Y: return SDL_FLIP_VERTICAL;
        default: return SDL_FLIP_NONE;
    }
}

std::optional<SDL_Rect> H2DE_Renderer::renderSurfaceGetPossibleSrcRect(H2DE_Surface* surface) {
    const std::optional<H2DE_PixelRect> possibleSrcRect = surface->getSrcRect();

    if (possibleSrcRect.has_value()) {
        H2DE_PixelRect value = possibleSrcRect.value();
        return SDL_Rect{ value.x, value.y, value.w, value.h };
    }

    return std::nullopt;
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

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    for (const auto& [name, hitbox] : object->getHitboxes()) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(object, hitbox, xIsInverted, yIsInverted);
        if (!engine->camera->containsRect(world_hitboxRect) && !objectIsAbsolute) {
            continue;
        }

        if (!object->isGrid) {
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
    float blockSize = engine->window->getSize().x / width;

    if (blockSize <= 0.0f) {
        blockSize = 0.1f;
    }

    return blockSize;
}

const float H2DE_Renderer::getGameBlockSize() const {
    return getBlockSize(engine->camera->getGameWidth());
}

const float H2DE_Renderer::getInterfaceBlockSize() const {
    return getBlockSize(engine->camera->getInterfaceWidth());
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
    return (surfaceIsValid && surface->isVisible());
}

// -- sdl getters
SDL_ScaleMode H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) noexcept {
    switch (scaleMode) {
        case H2DE_SCALE_MODE_BEST: return SDL_ScaleModeBest;
        case H2DE_SCALE_MODE_LINEAR: return SDL_ScaleModeLinear;
        default: return SDL_ScaleModeNearest;
    }
}

SDL_BlendMode H2DE_Renderer::getBlendMode(H2DE_BlendMode blendMode) noexcept {
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

    H2DE_LevelRect world_cameraRect = engine->camera->getWorldRect();
    H2DE_Translate world_translate = world_rect.getTranslate();

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    if (xIsInverted) {
        world_cameraRect.x *= -1;
    }

    if (yIsInverted) {
        world_cameraRect.y *= -1;
    }

    if (absolute) {
        world_translate += engine->camera->getInterfaceScale() * 0.5f;
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

    bool xIsInverted = engine->camera->isXOriginInverted();
    bool yIsInverted = engine->camera->isYOriginInverted();

    const H2DE_Scale cameraHalfScale = (absolute)
        ? engine->camera->getInterfaceScale() * 0.5f
        : engine->camera->getGameScale() * 0.5f;

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
        H2DE_Translate cameraTranslate = engine->camera->getTranslate();

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
