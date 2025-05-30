#include "H2DE/engine/H2DE_renderer.h"
#include "H2DE/engine/H2DE_geometry.h"
#undef max

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

// RENDER
void H2DE_Renderer::render() {
    clearRenderer();
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
        if (a->isGrid) {
            return true;
        }
        if (b->isGrid) {
            return false;
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
    for (H2DE_Object* object : objects) {
        renderObject(object);
    }

    renderObjectsHitboxes();
    SDL_RenderPresent(renderer);
}

void H2DE_Renderer::renderObject(H2DE_Object* object) {
    if (object->isHidden()) {
        return;
    }

    if (!engine->camera->containsObject(object) && !object->objectData.absolute) {
        return;
    }

    renderSurfaces(object);

    bool isNormalObjectAndNotRenderGrid = (engine->debugObjectEnabled && !object->isGrid);
    bool isGridAndRenderGrid = (object->isGrid && engine->camera->getData().grid);

    if (isNormalObjectAndNotRenderGrid || isGridAndRenderGrid) {
        renderObjectAddHitboxesToBuffer(object);
    }
}

// -- surfaces
void H2DE_Renderer::renderSurfaces(H2DE_Object* object) {
    for (H2DE_Surface* surface : object->surfaceBuffers) {
        if (isSurfaceVisible(surface)) {
            renderSurface(object, surface);
        }
    }
}

void H2DE_Renderer::renderSurface(H2DE_Object* object, H2DE_Surface* surface) {
    bool isTexture = (dynamic_cast<H2DE_Color*>(surface) == nullptr);

    if (isTexture) {
        renderTexture(object, surface);
    } else {
        renderColor(object, surface);
    }
}

// -- -- textures
void H2DE_Renderer::renderTexture(H2DE_Object* object, H2DE_Surface* surface) {
    SDL_Texture* texture = textures.find(surface->getTextureName())->second;
    renderTextureSetProperties(surface, texture);
    renderTextureRenderTexture(object, surface, texture);
}

void H2DE_Renderer::renderTextureSetProperties(H2DE_Surface* surface, SDL_Texture* texture) {
    const H2DE_ColorRGB color = surface->getColor();
    const SDL_ScaleMode scaleMode = R::getScaleMode(surface->surfaceData.scaleMode);
    const SDL_BlendMode blendMode = R::getBlendMode(surface->surfaceData.blendMode);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureBlendMode(texture, blendMode);
    SDL_SetTextureScaleMode(texture, scaleMode);
}

void H2DE_Renderer::renderTextureRenderTexture(H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, surface);
    float world_surfaceRotation = R::renderSurfaceGetWorldRotation(object, surface);
    SDL_Point local_surfaceCenter = R::renderSurfaceGetLocalPivot(object, surface);
    SDL_RendererFlip world_surfaceFlip = R::renderSurfaceGetWorldFlip(object, surface);

    std::optional<SDL_Rect> possibleSrcRect = renderSurfaceGetPossibleSrcRect(surface);
    SDL_Rect* srcRect = ((possibleSrcRect.has_value()) ? &possibleSrcRect.value() : nullptr);

    SDL_RenderCopyEx(renderer, texture, srcRect, &world_surfaceRect, world_surfaceRotation, &local_surfaceCenter, world_surfaceFlip);
}

// -- -- colors
void H2DE_Renderer::renderColor(H2DE_Object* object, H2DE_Surface* surface) {
    SDL_Rect world_surfaceRect = R::renderSurfaceGetWorldDestRect(object, surface);
    float world_surfaceRotation = R::renderSurfaceGetWorldRotation(object, surface);
    
    const int halfWidth = static_cast<int>(std::round(world_surfaceRect.w * 0.5f));
    const int halfHeight = static_cast<int>(std::round(world_surfaceRect.h * 0.5f));

    const H2DE_PixelRect world_pixel_surfaceRect = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight, world_surfaceRect.w, world_surfaceRect.h };
    const H2DE_PixelPivot world_pixel_pivot = { world_surfaceRect.x + halfWidth, world_surfaceRect.y + halfHeight };

    std::array<H2DE_PixelPos, 4> corners = world_pixel_surfaceRect.getCorners();
    for (H2DE_PixelPos& corner : corners) {
        corner = corner.rotate(world_pixel_pivot, world_surfaceRotation);
    }

    const std::vector<Sint16> vx = {
        static_cast<Sint16>(corners[0].x), static_cast<Sint16>(corners[1].x), static_cast<Sint16>(corners[2].x), static_cast<Sint16>(corners[3].x)
    };
    const std::vector<Sint16> vy = {
        static_cast<Sint16>(corners[0].y), static_cast<Sint16>(corners[1].y), static_cast<Sint16>(corners[2].y), static_cast<Sint16>(corners[3].y)
    };

    filledPolygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(surface->getColor()));
}

// -- -- getters
SDL_Rect H2DE_Renderer::renderSurfaceGetWorldDestRect(H2DE_Object* object, H2DE_Surface* surface) {
    return static_cast<SDL_Rect>(levelToPixelRect(G::getSurfaceRect(object, surface), object->objectData.absolute));
}

float H2DE_Renderer::renderSurfaceGetWorldRotation(H2DE_Object* object, H2DE_Surface* surface) {
    bool xIsInverted = (engine->camera->getXOrigin() == H2DE_FACE_RIGHT);
    bool yIsInverted = (engine->camera->getYOrigin() == H2DE_FACE_BOTTOM);

    float res = G::getSurfaceRotation(object, surface);

    if (xIsInverted) {
        res = G::flipRotation(res, H2DE_FLIP_X);
    }

    if (yIsInverted) {
        res = G::flipRotation(res, H2DE_FLIP_Y);
    }

    return res;
}

SDL_Point H2DE_Renderer::renderSurfaceGetLocalPivot(H2DE_Object* object, H2DE_Surface* surface) {
    bool objIsAbsolute = object->objectData.absolute;
    const H2DE_PixelSize pixel_surfaceScale = R::levelToPixelSize(surface->surfaceData.transform.scale * 0.5f, objIsAbsolute);

    return static_cast<SDL_Point>(pixel_surfaceScale);
}

SDL_RendererFlip H2DE_Renderer::renderSurfaceGetWorldFlip(H2DE_Object* object, H2DE_Surface* surface) {
    H2DE_Flip objFlip = G::getFlipFromScale(object->objectData.transform.scale);
    H2DE_Flip surFlip = G::getFlipFromScale(surface->surfaceData.transform.scale);
    H2DE_Flip addedFlip = G::addFlip(objFlip, surFlip);

    switch (addedFlip) {
        case H2DE_FLIP_X: return SDL_FLIP_HORIZONTAL;
        case H2DE_FLIP_Y: return SDL_FLIP_VERTICAL;
        default: return SDL_FLIP_NONE;
    }
}

std::optional<H2DE_PixelRect> H2DE_Renderer::renderSurfaceGetPossibleSrcRect(H2DE_Surface* surface) {
    const std::optional<H2DE_PixelRect> possibleSrcRect = surface->getSrcRect();

    if (possibleSrcRect.has_value()) {
        return possibleSrcRect.value();
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

    for (const auto& [name, hitbox] : object->getHitboxes()) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(object, hitbox);
        if (!engine->camera->containsRect(world_hitboxRect) && !objectIsAbsolute) {
            continue;
        }

        renderHitbox(world_hitboxRect, hitbox.color, objectIsAbsolute);
    }
}

void H2DE_Renderer::renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute) {
    SDL_Rect absRect = static_cast<SDL_Rect>(levelToPixelRect(world_hitboxRect, absolute));

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

bool H2DE_Renderer::isSurfaceVisible(H2DE_Surface* surface) {
    bool surfaceIsValid = (textures.find(surface->getTextureName()) != textures.end() || surface->getTextureName() == "/");
    return (surfaceIsValid && surface->isVisible());
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

// -- level to pixel
H2DE_PixelPos H2DE_Renderer::levelToPixelPos(const H2DE_LevelRect& world_rect, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();

    bool xIsInverted = (engine->camera->getXOrigin() == H2DE_FACE_RIGHT);
    bool yIsInverted = (engine->camera->getYOrigin() == H2DE_FACE_BOTTOM);

    H2DE_LevelRect world_cameraRect = engine->camera->getWorldRect();
    H2DE_Translate world_translate = world_rect.getTranslate();

    if (xIsInverted) {
        world_translate.x *= -1;
        world_cameraRect.x *= -1;
    }

    if (yIsInverted) {
        world_translate.y *= -1;
        world_cameraRect.y *= -1;
    }

    if (absolute) {
        world_translate += engine->camera->getInterfaceScale() * 0.5f;
    } else {
        world_translate += world_cameraRect.getScale() * 0.5f - world_cameraRect.getTranslate();
    }
    
    return {
        static_cast<int>(std::round(world_translate.x * blockSize)),
        static_cast<int>(std::round(world_translate.y * blockSize)),
    };
}

H2DE_PixelPos H2DE_Renderer::levelToPixelPos(const H2DE_Translate& local_translate, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();

    return {
        static_cast<int>(std::round(local_translate.x * blockSize)),
        static_cast<int>(std::round(local_translate.y * blockSize)),
    };
}

H2DE_PixelSize H2DE_Renderer::levelToPixelSize(const H2DE_Scale& world_scale, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();

    return {
        static_cast<int>(std::round(std::abs(world_scale.x) * blockSize)),
        static_cast<int>(std::round(std::abs(world_scale.y) * blockSize)),
    };
}

H2DE_PixelRect H2DE_Renderer::levelToPixelRect(const H2DE_LevelRect& world_rect, bool absolute) const {
    const H2DE_PixelPos pos = levelToPixelPos(world_rect, absolute);
    const H2DE_PixelSize size = levelToPixelSize(world_rect.getScale(), absolute);
    return H2DE_PixelRect{ pos.x, pos.y, size.x, size.y };
}

// -- pixel to level
H2DE_Translate H2DE_Renderer::pixelToLevel(const H2DE_PixelPos& pos, bool absolute) const {
    const float blockSize = (absolute) ? getInterfaceBlockSize() : getGameBlockSize();
    H2DE_Translate res = H2DE_Translate{ static_cast<float>(pos.x), static_cast<float>(pos.y) };

    const H2DE_Scale cameraHalfScale = (absolute)
        ? engine->getCamera()->getInterfaceScale() * 0.5f
        : engine->getCamera()->getGameScale() * 0.5f;

    res /= blockSize;
    res -= cameraHalfScale;

    bool xIsInverted = (engine->camera->getXOrigin() == H2DE_FACE_RIGHT);
    bool yIsInverted = (engine->camera->getYOrigin() == H2DE_FACE_BOTTOM);

    if (xIsInverted) {
        res.x *= -1;
    }

    if (yIsInverted) {
        res.y *= -1;
    }

    if (!absolute) {
        res += engine->getCamera()->getTranslate();
    }

    return res;
}
