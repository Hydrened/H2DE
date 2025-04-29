#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Engine::H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) : engine(e), renderer(r), objects(o) {
    H2DE_Error::checkEngine(engine);
}

// CLEANUP
H2DE_Engine::H2DE_Renderer::~H2DE_Renderer() {
    destroyTextures();
}

void H2DE_Engine::H2DE_Renderer::destroyTextures() {
    for (const auto& [name, texture] : textures) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    textures.clear();
}

// DEBUG
void H2DE_DebugObjects(const H2DE_Engine* engine, bool state) {
    H2DE_Error::checkEngine(engine);
    engine->renderer->debug = state;
}

// RENDER
void H2DE_Engine::H2DE_Renderer::render() {
    clearRenderer();
    sortObjects();
    renderObjects();
}

void H2DE_Engine::H2DE_Renderer::clearRenderer() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

// objects
void H2DE_Engine::H2DE_Renderer::sortObjects() {
    std::sort(objects.begin(), objects.end(), [](H2DE_Object* a, H2DE_Object* b) {
        H2DE_Error::checkObject(a);
        H2DE_Error::checkObject(b);

        const int indexA = H2DE_GetObjectIndex(a);
        const int indexB = H2DE_GetObjectIndex(b);

        if (indexA == indexB) {
            return H2DE_Engine::isPositionGreater(a, b);
        }

        return indexA < indexB;
    });
}

void H2DE_Engine::H2DE_Renderer::renderObjects() {
    for (H2DE_Object* object : objects) {
        H2DE_Error::checkObject(object);
        renderObject(object);
    }

    renderObjectsHitboxes();
    SDL_RenderPresent(renderer);
}

void H2DE_Engine::H2DE_Renderer::renderObject(H2DE_Object* object) {
    renderSurfaces(object);

    if (debug) {
        renderObjectAddHitboxesToBuffer(object);
    }
}

void H2DE_Engine::H2DE_Renderer::renderObjectAddHitboxesToBuffer(H2DE_Object* object) {
    hitboxesBuffer.push_back(object);
}

// surfaces
void H2DE_Engine::H2DE_Renderer::renderSurfaces(H2DE_Object* object) const {
    const bool isText = dynamic_cast<H2DE_TextObject*>(object) != nullptr;
    const bool absolute = H2DE_IsObjectAbsolute(object);

    for (const H2DE_SurfaceBuffer surfaceBuffer : object->getSurfaceBuffers()) {
        if (!surfaceBuffer.surface) {
            continue;
        }

        const H2DE_LevelRect rect = (object->od.rect.getPos() + surfaceBuffer.offset).makeRect(surfaceBuffer.size);
        bool rotationIsNotNull = surfaceBuffer.surface->sd.rotation != 0.0f || object->od.rotation != 0.0f;

        if (H2DE_CameraContainsRect(engine, rect, absolute) || isText || rotationIsNotNull) {
            renderSurface(object, surfaceBuffer, object->od.absolute);
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderSurface(const H2DE_Object* object, const H2DE_SurfaceBuffer& surfaceBuffer, bool absolute) const {
    const H2DE_Surface* surface = surfaceBuffer.surface;

    if (!isSurfaceValid(surface)) {
        return;
    }

    SDL_Texture* texture = textures.find(surface->sd.textureName)->second;
    renderSurfaceSetTextureProperties(texture, surface);
    renderSurfaceRenderTexture(texture, object, surface, absolute);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceSetTextureProperties(SDL_Texture* texture, const H2DE_Surface* surface) const {
    const H2DE_ColorRGB& color = surface->sd.color;
    const SDL_ScaleMode scaleMode = R::getScaleMode(surface->sd.scaleMode);
    const SDL_BlendMode blendMode = R::getBlendMode(surface->sd.blendMode);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureBlendMode(texture, blendMode);
    SDL_SetTextureScaleMode(texture, scaleMode);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceRenderTexture(SDL_Texture* texture, const H2DE_Object* object, const H2DE_Surface* surface, bool absolute) const {
    const SDL_Rect W_destRect = static_cast<SDL_Rect>(lvlToAbsRect(R::renderSurfaceGetWorldDestRect(object, surface), absolute));
    const float W_rotation = R::renderSurfaceGetWorldRotation(object, surface);
    const SDL_Point L_center = static_cast<SDL_Point>(lvlToAbsPivot(surface->sd.rect.getSize().getCenter()));
    const SDL_RendererFlip W_flip = R::getFlip(H2DE_AddFlip(object->od.flip, surface->sd.flip));

    const std::optional<SDL_Rect>& src = surface->getSrcRect();
    SDL_RenderCopyEx(renderer, texture, (src.has_value()) ? &src.value() : nullptr, &W_destRect, W_rotation, &L_center, W_flip);
}

const H2DE_LevelRect H2DE_Engine::H2DE_Renderer::renderSurfaceGetWorldDestRect(const H2DE_Object* object, const H2DE_Surface* surface) {
    const H2DE_LevelRect& W_objectRect = object->od.rect;
    const H2DE_LevelRect& L_surfaceRect = surface->sd.rect;

    const H2DE_LevelPos& L_objectPivot = object->od.pivot;
    const H2DE_LevelPos& L_surfacePivot = surface->sd.pivot;

    const float& L_objectRotation = object->od.rotation;
    const float& L_surfaceRotation = surface->sd.rotation;

    const H2DE_Flip& L_objectFlip = object->od.flip;

    const H2DE_LevelRect W_objectFliped_objectRect = W_objectRect;
    const H2DE_LevelRect W_objectFliped_surfaceRect = E::flipRect(W_objectFliped_objectRect, L_surfaceRect, L_objectFlip);

    const H2DE_LevelPos W_objectFliped_objectPivot = E::flipPivot(W_objectFliped_objectRect, L_objectPivot, L_objectFlip);
    const H2DE_LevelPos W_surfaceFliped_surfacePivot = E::flipPivot(W_objectFliped_surfaceRect, L_surfacePivot, L_objectFlip);

    const float L_objectFliped_objectRotation = L_objectRotation * (E::isRotationInverted(L_objectFlip) ? -1.0f : 1.0f);
    const float L_objectFliped_surfaceRotation = L_surfaceRotation * (E::isRotationInverted(L_objectFlip) ? -1.0f : 1.0f);

    const H2DE_LevelRect W_objectFliped_objectRotated_surfaceRect = R::applyRotationOnRect(W_objectFliped_surfaceRect, W_objectFliped_objectPivot, L_objectFliped_objectRotation);
    const H2DE_LevelPos W_objectFliped_objectRotated_surfacePivot = R::applyRotationOnPos(W_surfaceFliped_surfacePivot, W_objectFliped_objectPivot, L_objectFliped_objectRotation);
    return R::applyRotationOnRect(W_objectFliped_objectRotated_surfaceRect, W_objectFliped_objectRotated_surfacePivot, L_objectFliped_surfaceRotation);
}

const float H2DE_Engine::H2DE_Renderer::renderSurfaceGetWorldRotation(const H2DE_Object* object, const H2DE_Surface* surface) {
    const H2DE_Flip W_flip = H2DE_AddFlip(object->od.flip, surface->sd.flip);
    const float rotationCausedByFlip = (W_flip == H2DE_FLIP_XY) ? 180.0f : 0.0f;

    const float flipedObjectRotation = E::flipRotation(object->od.rotation, object->od.flip);
    const float flipedSurfaceRotation = E::flipRotation(surface->sd.rotation, W_flip);
    return flipedObjectRotation + flipedSurfaceRotation + rotationCausedByFlip;
}

// hitboxes
void H2DE_Engine::H2DE_Renderer::renderObjectsHitboxes() {
    for (const H2DE_Object* object : hitboxesBuffer) {
        if (object->od.hitboxes.size() != 0) {
            renderHitboxes(object);
        }
    }
        
    hitboxesBuffer.clear();
}

void H2DE_Engine::H2DE_Renderer::renderHitboxes(const H2DE_Object* object) const {
    const H2DE_LevelPos pos = object->od.rect.getPos();
    bool absolute = object->od.absolute;

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        const H2DE_LevelRect& W_hitboxRect = object->od.rect;
        const H2DE_LevelRect W_fliped_hitboxRect = E::flipRect(W_hitboxRect, hitbox.rect, object->od.flip);

        if (H2DE_CameraContainsRect(engine, W_fliped_hitboxRect, absolute)) {
            renderHitbox(W_fliped_hitboxRect, hitbox.color, absolute);
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderHitbox(const H2DE_LevelRect& rect, const H2DE_ColorRGB& color, bool absolute) const {
    const H2DE_AbsRect absRect = lvlToAbsRect(rect, absolute);

    const Sint16 minX = absRect.x;
    const Sint16 maxX = absRect.x + absRect.w - 1;
    const Sint16 minY = absRect.y;
    const Sint16 maxY = absRect.y + absRect.h - 1;

    const std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    const std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(color));
}

// GETTER
const bool H2DE_Engine::H2DE_Renderer::isSurfaceValid(const H2DE_Surface* surface) const {
    H2DE_Error::checkSurface(surface);

    const auto it = textures.find(surface->sd.textureName);
    return it != textures.end();
}

const unsigned int H2DE_Engine::H2DE_Renderer::getBlockSize() const {
    int blockSize = H2DE_GetWindowSize(engine).x / H2DE_GetCameraSize(engine).x;
    if (blockSize <= 0) {
        blockSize = 1;
    }
    return blockSize;
}

SDL_ScaleMode H2DE_Engine::H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) {
    if (scaleMode == H2DE_SCALE_MODE_LINEAR) {
        return SDL_ScaleModeLinear;
    }
    if (scaleMode == H2DE_SCALE_MODE_NEAREST) {
        return SDL_ScaleModeNearest;
    }
    return SDL_ScaleModeBest;
}

SDL_BlendMode H2DE_Engine::H2DE_Renderer::getBlendMode(H2DE_BlendMode blendMode) {
    switch (blendMode) {
        case H2DE_BLEND_MODE_BLEND: return SDL_BLENDMODE_BLEND;
        case H2DE_BLEND_MODE_ADD: return SDL_BLENDMODE_ADD;
        case H2DE_BLEND_MODE_MOD: return SDL_BLENDMODE_MOD;
        case H2DE_BLEND_MODE_MUL: return SDL_BLENDMODE_MUL;
        case H2DE_BLEND_MODE_INVALID: return SDL_BLENDMODE_INVALID;
        default: return SDL_BLENDMODE_NONE;
    }
}

SDL_RendererFlip H2DE_Engine::H2DE_Renderer::getFlip(H2DE_Flip flip) {
    if (flip == H2DE_FLIP_XY) {
        return SDL_FLIP_NONE;
    }
    if (flip == H2DE_FLIP_NONE) {
        return SDL_FLIP_NONE;
    }
    if (flip == H2DE_FLIP_X) {
        return SDL_FLIP_HORIZONTAL;
    }
    return SDL_FLIP_VERTICAL;
}

H2DE_LevelPos H2DE_Engine::H2DE_Renderer::applyRotationOnPos(const H2DE_LevelPos& W_pos, const H2DE_LevelPos& W_pivot, float rotation) {
    const H2DE_LevelPos W_vecToPivot = W_pos - W_pivot;

    const float rad = rotation * static_cast<float>(M_PI / 180.0f);
    H2DE_LevelPos W_rotatedVec = {
        W_vecToPivot.x * cosf(rad) - W_vecToPivot.y * sinf(rad),
        W_vecToPivot.x * sinf(rad) + W_vecToPivot.y * cosf(rad),
    };

    return W_rotatedVec + W_pivot;
}

H2DE_LevelRect H2DE_Engine::H2DE_Renderer::applyRotationOnRect(const H2DE_LevelRect& W_rect, const H2DE_LevelPos& W_pivot, float rotation) {
    const H2DE_LevelSize rectSize = W_rect.getSize();
    const H2DE_LevelPos W_rectCenter = W_rect.getCenter();

    const H2DE_LevelPos W_vecPivot = W_pivot - W_rectCenter;

    const float rad = rotation * static_cast<float>(M_PI / 180.0f);
    const H2DE_LevelPos W_rotatedVec = {
        W_vecPivot.x * cosf(rad) - W_vecPivot.y * sinf(rad),
        W_vecPivot.x * sinf(rad) + W_vecPivot.y * cosf(rad),
    };

    const H2DE_LevelPos W_finalRectCenter = W_pivot - W_rotatedVec;

    return H2DE_LevelPos{ W_finalRectCenter - rectSize.getCenter() }.makeRect(rectSize);
}

H2DE_AbsPos H2DE_Engine::H2DE_Renderer::lvlToAbsPos(const H2DE_LevelPos& pos, bool absolute) const {
    const unsigned int blockSize = getBlockSize(); 
    const H2DE_LevelPos offset = (absolute) ? H2DE_LevelPos{ 0.0f, 0.0f } : H2DE_GetCameraPos(engine);

    return {
        static_cast<int>(std::round((pos.x - (offset.x)) * blockSize)),
        static_cast<int>(std::round((pos.y - (offset.y)) * blockSize)),
    };
}

H2DE_AbsPos H2DE_Engine::H2DE_Renderer::lvlToAbsPivot(const H2DE_LevelPos& pos) const {
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(std::round(pos.x * blockSize)),
        static_cast<int>(std::round(pos.y * blockSize)),
    };
}

H2DE_AbsSize H2DE_Engine::H2DE_Renderer::lvlToAbsSize(const H2DE_LevelSize& size) const {
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(std::round(size.x * blockSize)),
        static_cast<int>(std::round(size.y * blockSize)),
    };
}

H2DE_AbsRect H2DE_Engine::H2DE_Renderer::lvlToAbsRect(const H2DE_LevelRect& rect, bool absolute) const {
    const H2DE_AbsPos pos = lvlToAbsPos(rect.getPos(), absolute);
    const H2DE_AbsSize size = lvlToAbsSize(rect.getSize());
    
    return {
        pos.x,
        pos.y,
        size.x,
        size.y,
    };
}

H2DE_LevelPos H2DE_Engine::H2DE_Renderer::absToLvl(const H2DE_AbsPos& pos, bool absolute) const {
    const unsigned int blockSize = getBlockSize(); 

    const H2DE_LevelPos levelCamPos = H2DE_GetCameraPos(engine);
    const H2DE_AbsPos absCamPos = {
        static_cast<int>(levelCamPos.x * blockSize),
        static_cast<int>(levelCamPos.y * blockSize),
    };

    return {
        (static_cast<float>(pos.x) + ((absolute) ? 0.0f : absCamPos.x)) / static_cast<float>(blockSize),
        (static_cast<float>(pos.y) + ((absolute) ? 0.0f : absCamPos.y)) / static_cast<float>(blockSize),
    };
}

const std::unordered_map<std::string, H2DE_Font>& H2DE_Engine::H2DE_Renderer::getFonts() const {
    return fonts;
}
