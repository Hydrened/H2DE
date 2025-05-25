#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_geometry.h"
#undef max

// INIT
H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r) : engine(e), renderer(r) {

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
void H2DE_Renderer::render(std::vector<H2DE_Object*>& objects) {
    clearRenderer();
    sortObjects(objects);
    renderObjects(objects);
}

void H2DE_Renderer::clearRenderer() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// -- objects
void H2DE_Renderer::sortObjects(std::vector<H2DE_Object*>& objects) {
    std::sort(objects.begin(), objects.end(), [](H2DE_Object* a, H2DE_Object* b) {
        int indexA = a->getIndex();
        int indexB = b->getIndex();

        if (indexA == indexB) {
            return H2DE_Engine::isPositionGreater(a, b);
        }

        return (indexA < indexB);
    });
}

void H2DE_Renderer::renderObjects(std::vector<H2DE_Object*>& objects) {
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

    if (!engine->camera->containsObject(object)) {
        return;
    }

    renderSurfaces(object);

    if (engine->debugObjectEnabled) {
        renderObjectAddHitboxesToBuffer(object);
    }
}

// -- surfaces
void H2DE_Renderer::renderSurfaces(H2DE_Object* object) {
    // const H2DE_Translate world_objectTranslate = object->getTranslate();
    // bool objectIsAbsolute = object->isAbsolute();

    // for (H2DE_Surface* surface : object->surfaceBuffers) {
    //     const H2DE_Transform& surfaceTransform = surface->surfaceData.transform;

    //     const H2DE_LevelRect local_surfaceRect = surfaceTransform.translate.makeRect(surfaceTransform.scale);
    //     const H2DE_LevelRect world_surfaceRect = local_surfaceRect.addTranslate(world_objectTranslate);

    //     if (engine->camera->containsRect(world_surfaceRect)) {
    //         renderSurface(object, surface);
    //     }
    // }
}

void H2DE_Renderer::renderSurface(H2DE_Object* object, H2DE_Surface* surface) {
    if (!isSurfaceVisible(surface)) {
        return;
    }

    SDL_Texture* texture = textures.find(surface->surfaceData.textureName)->second;
    renderSurfaceSetProperties(surface, texture);
    renderSurfaceRenderTexture(object, surface, texture);
}

void H2DE_Renderer::renderSurfaceSetProperties(H2DE_Surface* surface, SDL_Texture* texture) {
    const H2DE_ColorRGB color = surface->surfaceData.color;
    const SDL_ScaleMode scaleMode = R::getScaleMode(surface->surfaceData.scaleMode);
    const SDL_BlendMode blendMode = R::getBlendMode(surface->surfaceData.blendMode);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureBlendMode(texture, blendMode);
    SDL_SetTextureScaleMode(texture, scaleMode);
}

void H2DE_Renderer::renderSurfaceRenderTexture(H2DE_Object* object, H2DE_Surface* surface, SDL_Texture* texture) {
    // SDL_Rect world_surfaceRect = static_cast<SDL_Rect>(levelToAbsRect(R::renderSurfaceGetWorldDestRect(object, surface), object->objectData.absolute));
    // float world_surfaceRotation = R::renderSurfaceGetWorldRotation(object, surface);
    // SDL_Point local_surfaceCenter = static_cast<SDL_Point>(R::levelToAbsPos(surface->surfaceData.transform.scale * 0.5f));
    // SDL_RendererFlip world_surfaceFlip = R::renderSurfaceGetWorldFlip(object, surface);

    // std::optional<SDL_Rect> possibleSrcRect = renderSurfaceGetPossibleSrcRect(surface);
    // SDL_Rect* srcRect = ((possibleSrcRect.has_value()) ? &possibleSrcRect.value() : nullptr);

    // SDL_RenderCopyEx(renderer, texture, srcRect, &world_surfaceRect, world_surfaceRotation, &local_surfaceCenter, world_surfaceFlip);
}

H2DE_LevelRect H2DE_Renderer::renderSurfaceGetWorldDestRect(H2DE_Object* object, H2DE_Surface* surface) {
    // const H2DE_Transform& objectTransform = object->objectData.transform;
    // const H2DE_Transform& surfaceTransform = surface->surfaceData.transform;

    // const H2DE_LevelRect world_objectRect = objectTransform.translate.makeRect(objectTransform.scale);
    // const H2DE_LevelRect local_surfaceRect = surfaceTransform.translate.makeRect(surfaceTransform.scale);

    // SDL_RendererFlip objectFlip = G::getFlipFromScale(objectTransform.scale);

    // const H2DE_LevelRect world_objectFliped_surfaceRect = G::getWorldFlipedRect(world_objectRect, local_surfaceRect, objectFlip);





    return H2DE_LevelRect();

    
    // return world_objectFliped_surfaceRect;
}

float H2DE_Renderer::renderSurfaceGetWorldRotation(H2DE_Object* object, H2DE_Surface* surface) {
    // float local_objectRotation = object->objectData.transform.rotation;
    // float local_surfaceRotation = surface->surfaceData.transform.rotation;

    // SDL_RendererFlip local_objectFlip = G::getFlipFromScale(object->objectData.transform.scale);
    // SDL_RendererFlip local_surfaceFlip = G::getFlipFromScale(surface->surfaceData.transform.scale);
    // SDL_RendererFlip world_surfaceFlip = G::addFlip(local_objectFlip, local_surfaceFlip);

    // float local_objectFliped_objectRotation = G::flipRotation(local_objectRotation, local_objectFlip);
    // float local_surfaceFliped_objectFliped_surfaceRotation = G::flipRotation(local_surfaceRotation, world_surfaceFlip);

    // float rotationCausedByFlip = ((R::renderSurfaceIstextureRotatedByFlip(object, surface)) ? 180.0f : 0.0f);

    // return local_objectFliped_objectRotation + local_surfaceFliped_objectFliped_surfaceRotation + rotationCausedByFlip;

    return 0.0f;
}

SDL_RendererFlip H2DE_Renderer::renderSurfaceGetWorldFlip(H2DE_Object* object, H2DE_Surface* surface) {
    SDL_RendererFlip local_objectFlip = G::getFlipFromScale(object->objectData.transform.scale);
    SDL_RendererFlip local_surfaceFlip = G::getFlipFromScale(surface->surfaceData.transform.scale);

    return G::addFlip(local_objectFlip, local_surfaceFlip);
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
        if (!engine->camera->containsRect(world_hitboxRect)) {
            continue;
        }

        renderHitbox(world_hitboxRect, hitbox.color, objectIsAbsolute);
    }
}

void H2DE_Renderer::renderHitbox(const H2DE_LevelRect& world_hitboxRect, const H2DE_ColorRGB& color, bool absolute) {
    const H2DE_PixelRect absRect = levelToAbsRect(world_hitboxRect, absolute);

    Sint16 minX = absRect.x;
    Sint16 maxX = absRect.x + absRect.w - 1;
    Sint16 minY = absRect.y;
    Sint16 maxY = absRect.y + absRect.h - 1;

    const std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    const std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(color));
}

// GETTER
const unsigned int H2DE_Renderer::getBlockSize() const {
    int blockSize = std::round(engine->window->getSize().x / engine->camera->getScale().x);

    if (blockSize == 0) {
        blockSize = 1;
    }

    return blockSize;
}

bool H2DE_Renderer::isSurfaceVisible(H2DE_Surface* surface) {
    bool surfaceIsValid = (textures.find(surface->getTextureName()) != textures.end());
    bool surfaceIsNotHidden = !(surface->isHidden());
    bool surfaceIsVisible = (surface->getColor().isVisible());
    
    return (surfaceIsValid && surfaceIsNotHidden && surfaceIsVisible);
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

// -- level to abs
H2DE_PixelPos H2DE_Renderer::levelToAbsPos(const H2DE_LevelRect& world_rect, bool absolute) const {
    const unsigned int blockSize = getBlockSize(); 

    bool xIsInverted = (engine->camera->getXOrigin() == H2DE_FACE_RIGHT);
    bool yIsInverted = (engine->camera->getYOrigin() == H2DE_FACE_BOTTOM);

    const H2DE_LevelRect world_cameraRect = engine->camera->getWorldRect();
    H2DE_Translate world_translate = world_rect.getTranslate();
    world_translate -= world_rect.getScale() * 0.5;

    if (!absolute) {
        world_translate -= world_cameraRect.getTranslate();
    }

    if (xIsInverted) {
        world_translate.x = world_cameraRect.w - world_translate.x - world_rect.w;
    }

    if (yIsInverted) {
        world_translate.y = world_cameraRect.h - world_translate.y - world_rect.h;
    }

    return {
        static_cast<int>(std::round(world_translate.x * blockSize)),
        static_cast<int>(std::round(world_translate.y * blockSize)),
    };
}

H2DE_PixelPos H2DE_Renderer::levelToAbsPos(const H2DE_Translate& local_translate) const {
    return R::levelToAbsSize({ local_translate.x, local_translate.y });
}

H2DE_PixelSize H2DE_Renderer::levelToAbsSize(const H2DE_Scale& world_scale) const {
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(std::round(world_scale.x * blockSize)),
        static_cast<int>(std::round(world_scale.y * blockSize)),
    };
}

H2DE_PixelRect H2DE_Renderer::levelToAbsRect(const H2DE_LevelRect& world_rect, bool absolute) const {
    const H2DE_PixelPos pos = levelToAbsPos(world_rect, absolute);
    const H2DE_PixelSize size = levelToAbsSize(world_rect.getScale());
    return H2DE_PixelRect{ pos.x, pos.y, size.x, size.y };
}
