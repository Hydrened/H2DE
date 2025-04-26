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

        const H2DE_LevelRect rect = (object->od.pos + surfaceBuffer.offset).makeRect(surfaceBuffer.size);

        if (H2DE_CameraContainsRect(engine, rect, absolute) || isText) {
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
    const H2DE_LevelRect destRect = renderSurfaceGetRotatedDestRect(object, surface);

    renderSurfaceRenderTexture(texture, object, surface, destRect, absolute);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceSetTextureProperties(SDL_Texture* texture, const H2DE_Surface* surface) const {
    const H2DE_ColorRGB& color = surface->sd.color;
    const SDL_ScaleMode scaleMode = H2DE_Engine::H2DE_Renderer::getScaleMode(surface->sd.scaleMode);
    const SDL_BlendMode blendMode = H2DE_Engine::H2DE_Renderer::getBlendMode(surface->sd.blendMode);

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureBlendMode(texture, blendMode);
    SDL_SetTextureScaleMode(texture, scaleMode);
}

H2DE_LevelRect H2DE_Engine::H2DE_Renderer::renderSurfaceGetRotatedDestRect(const H2DE_Object* object, const H2DE_Surface* surface) const {
    const H2DE_LevelRect originalDestRect = object->od.pos.makeRect({ 0.0f, 0.0f }) + surface->sd.rect;

    const H2DE_LevelPos flipedObjectPivot = H2DE_Engine::H2DE_Renderer::flipPivot(originalDestRect, object->od.pivot, object->od.flip);
    const float flipedObjectRotation = H2DE_Engine::H2DE_Renderer::flipRotation(object->od.rotation, object->od.flip);
    const H2DE_LevelRect objectRotatedDestRect = H2DE_Engine::H2DE_Renderer::applyRotationOnRect(originalDestRect, flipedObjectPivot, flipedObjectRotation);

    const H2DE_Flip addedFlips = H2DE_AddFlip(object->od.flip, surface->sd.flip);

    const H2DE_LevelPos flipedSurfacePivot = H2DE_Engine::H2DE_Renderer::flipPivot(originalDestRect, surface->sd.pivot, surface->sd.flip);
    const float flipedSurfaceRotation = H2DE_Engine::H2DE_Renderer::flipRotation(surface->sd.rotation, surface->sd.flip);
    const H2DE_LevelPos objectRotatedPivot = H2DE_Engine::H2DE_Renderer::applyRotationOnPivot(originalDestRect, flipedSurfacePivot, flipedSurfaceRotation);

    return H2DE_Engine::H2DE_Renderer::applyRotationOnRect(objectRotatedDestRect, objectRotatedPivot, surface->sd.rotation);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceRenderTexture(SDL_Texture* texture, const H2DE_Object* object, const H2DE_Surface* surface, const H2DE_LevelRect& destRect, bool absolute) const {
    const H2DE_Flip addedFlips = H2DE_AddFlip(object->od.flip, surface->sd.flip);
    const float rotationCausedByFlip = (addedFlips == H2DE_FLIP_XY) ? 180.0f : 0.0f;

    const float flipedObjectRotation = H2DE_Engine::H2DE_Renderer::flipRotation(object->od.rotation, object->od.flip);
    const float flipedSurfaceRotation = H2DE_Engine::H2DE_Renderer::flipRotation(surface->sd.rotation, surface->sd.flip);
    const float rotation = flipedObjectRotation + flipedSurfaceRotation + rotationCausedByFlip;

    const SDL_Rect dst = static_cast<SDL_Rect>(lvlToAbsRect(destRect, absolute));
    const SDL_Point center = static_cast<SDL_Point>(lvlToAbsPivot(surface->sd.rect.getSize().getCenter()));
    const SDL_RendererFlip flip = H2DE_Engine::H2DE_Renderer::getFlip(addedFlips);

    const std::optional<SDL_Rect>& src = surface->getSrcRect();

    if (src.has_value()) {
        SDL_RenderCopyEx(renderer, texture, &src.value(), &dst, rotation, &center, flip);
    } else {
        SDL_RenderCopyEx(renderer, texture, nullptr, &dst, rotation, &center, flip);
    }
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
    const H2DE_LevelPos pos = object->od.pos;
    bool absolute = object->od.absolute;

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        if (H2DE_CameraContainsHitbox(engine, pos, hitbox, absolute)) {
            renderHitbox(object, hitbox, absolute);
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderHitbox(const H2DE_Object* object, const H2DE_Hitbox& hitbox, bool absolute) const {
    const H2DE_LevelRect objRect = object->od.pos.makeRect(object->od.size);
    const H2DE_LevelRect flipedHitboxRect = H2DE_Engine::H2DE_Renderer::flipRect(objRect, hitbox.rect, object->od.flip);

    H2DE_AbsPos absPos = lvlToAbsPos(objRect.getPos() + flipedHitboxRect.getPos(), absolute);
    H2DE_AbsSize absSize = lvlToAbsSize(flipedHitboxRect.getSize());

    Sint16 minX = absPos.x;
    Sint16 maxX = absPos.x + absSize.x - 1;
    Sint16 minY = absPos.y;
    Sint16 maxY = absPos.y + absSize.y - 1;

    std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(hitbox.color));
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

H2DE_LevelRect H2DE_Engine::H2DE_Renderer::applyRotationOnRect(const H2DE_LevelRect& rect, const H2DE_LevelPos& pivot, float rotation) {
    const H2DE_LevelSize size = rect.getSize();
    const H2DE_LevelPos center = rect.getCenter();

    const H2DE_LevelPos pivotWorld = rect.getPos() + pivot;
    const H2DE_LevelPos vecPivot = pivotWorld - center;

    float rad = rotation * static_cast<float>(M_PI / 180.0f);
    const H2DE_LevelPos rotatedVec = {
        vecPivot.x * cosf(rad) - vecPivot.y * sinf(rad),
        vecPivot.x * sinf(rad) + vecPivot.y * cosf(rad),
    };
    const H2DE_LevelPos finalCenter = pivotWorld - rotatedVec;

    return H2DE_LevelPos{ finalCenter - size.getCenter() }.makeRect(size);
}

H2DE_LevelPos H2DE_Engine::H2DE_Renderer::applyRotationOnPivot(const H2DE_LevelRect& rect, const H2DE_LevelPos& pivot, float rotation) {
    const H2DE_LevelPos pivotWorld = rect.getPos() + pivot;
    const H2DE_LevelPos vecPivot = pivotWorld - rect.getCenter();

    float rad = rotation * static_cast<float>(M_PI / 180.0f);
    const H2DE_LevelPos rotatedVec = {
        vecPivot.x * cosf(rad) - vecPivot.y * sinf(rad),
        vecPivot.x * sinf(rad) + vecPivot.y * cosf(rad),
    };
    const H2DE_LevelPos rotatedPivotWorld = rect.getCenter() + rotatedVec;

    return rotatedPivotWorld - rect.getPos();
}

H2DE_LevelRect H2DE_Engine::H2DE_Renderer::flipRect(const H2DE_LevelRect& objRect, const H2DE_LevelRect& rect, H2DE_Flip flip) {
    H2DE_LevelRect res = rect;

    if (flip & H2DE_FLIP_X) {
        res.x = objRect.getSize().x - (rect.getPos().x + rect.getSize().x);
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = objRect.getSize().y - (rect.getPos().y + rect.getSize().y);
    }

    return res;
}

float H2DE_Engine::H2DE_Renderer::flipRotation(float rotation, H2DE_Flip flip) {
    float res = rotation;

    switch (flip) {
        case H2DE_FLIP_X: res = -res; break;
        case H2DE_FLIP_Y: res = 180.0f - res; break;
        case H2DE_FLIP_XY: res = 180.0f + res; break;
        default: break;
    }

    while (res < 0.0f) {
        res += 360.0f;
    }
    while (res >= 360.0f) {
        res -= 360.0f;
    }

    return res;
}

H2DE_LevelPos H2DE_Engine::H2DE_Renderer::flipPivot(const H2DE_LevelRect& rect, const H2DE_LevelPos& pivot, H2DE_Flip flip) {
    H2DE_LevelPos res = H2DE_LevelPos();

    if (flip & H2DE_FLIP_X) {
        res.x = rect.getSize().x - pivot.x;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = rect.getSize().y - pivot.y;
    }

    return res;
}

H2DE_AbsPos H2DE_Engine::H2DE_Renderer::lvlToAbsPos(const H2DE_LevelPos& pos, bool absolute) const {
    const H2DE_LevelPos camPos = H2DE_GetCameraPos(engine);
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(std::round((pos.x - ((absolute) ? 0.0f : camPos.x)) * blockSize)),
        static_cast<int>(std::round((pos.y - ((absolute) ? 0.0f : camPos.y)) * blockSize)),
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
