#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_Engine::H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) : engine(e), renderer(r), objects(o) {
    
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

void H2DE_Engine::H2DE_Renderer::sortObjects() {
    std::sort(objects.begin(), objects.end(), [](H2DE_Object* a, H2DE_Object* b) {
        int indexA = H2DE_GetObjectIndex(a);
        int indexB = H2DE_GetObjectIndex(b);

        if (indexA == indexB) {
            return H2DE_Renderer::isPositionGreater(a, b);
        }

        return indexA < indexB;
    });
}

void H2DE_Engine::H2DE_Renderer::renderObjects() const {
    for (H2DE_Object* object : objects) {
        renderObject(object);
    }
    SDL_RenderPresent(renderer);
}

void H2DE_Engine::H2DE_Renderer::renderObject(H2DE_Object* object) const {
    renderSurfaces(object);

    if (debug) {
        renderHitboxes(object);
    }
}

void H2DE_Engine::H2DE_Renderer::renderSurfaces(H2DE_Object* object) const {
    bool isText = dynamic_cast<H2DE_TextObject*>(object) != nullptr;
    bool isSizeNull = object->od.size.x == 0.0f || object->od.size.y == 0.0f;
    bool absolute = H2DE_IsObjectAbsolute(object);

    if (isSizeNull && !isText) {
        return;
    }

    for (const H2DE_SurfaceBuffer surfaceBuffer : object->getSurfaceBuffers()) {
        if (!surfaceBuffer.surface) {
            continue;
        }

        const H2DE_LevelRect rect = (object->od.pos + surfaceBuffer.offset).makeRect(surfaceBuffer.size);

        if (H2DE_CameraContainsRect(engine, rect, absolute) || isText) {
            renderSurface(surfaceBuffer, rect, object->od.absolute);
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderSurface(const H2DE_SurfaceBuffer& surfaceBuffer, const H2DE_LevelRect& rect, bool absolute) const {
    const H2DE_Surface* surface = surfaceBuffer.surface;
    if (!isSurfaceValid(surface)) {
        return;
    }

    SDL_Texture* texture = textures.find(surface->sd.textureName)->second;
    SDL_Rect destRect = lvlToAbs(rect, absolute);

    rs_setTextureProperties(texture, surface->sd.color, surface->sd.scaleMode);
    
    SDL_Texture* tempTexture = rs_createTempTexture(destRect);
    if (!tempTexture) {
        return;
    }

    H2DE_Flip addedFlips = H2DE_AddFlip(surface->sd.flip, surfaceBuffer.flip);
    float flipRotation = (addedFlips == H2DE_FLIP_XY) ? 180.0f : 0.0f;
    SDL_Point flipRotationPivot = lvlToAbs(surfaceBuffer.size.getCenter(), absolute);
    SDL_RendererFlip flip = getFlip(addedFlips);
    rs_renderTempTexture(texture, surface->getSrcRect(), flipRotation, &flipRotationPivot, flip);

    float rotation = surface->sd.rotation + surfaceBuffer.rotation;
    SDL_Point pivot = lvlToAbs(surface->sd.pivot, absolute);
    rs_renderFinalTexture(tempTexture, &destRect, rotation, &pivot);
}

void H2DE_Engine::H2DE_Renderer::rs_setTextureProperties(SDL_Texture* texture, const H2DE_ColorRGB& color, H2DE_ScaleMode scaleMode) const {
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureScaleMode(texture, getScaleMode(scaleMode));
}

SDL_Texture* H2DE_Engine::H2DE_Renderer::rs_createTempTexture(const SDL_Rect& destRect) const {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
    if (!texture) {
        SDL_Log("Failed to create render target: %s", SDL_GetError());
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderClear(renderer);

    return texture;
}

void H2DE_Engine::H2DE_Renderer::rs_renderTempTexture(SDL_Texture* texture, const std::optional<SDL_Rect>& srcRect, float rotation, const SDL_Point* center, SDL_RendererFlip flip) const {
    if (srcRect.has_value()) {
        SDL_RenderCopyEx(renderer, texture, &srcRect.value(), nullptr, rotation, center, flip);
    } else {
        SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, rotation, center, flip);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void H2DE_Engine::H2DE_Renderer::rs_renderFinalTexture(SDL_Texture* tempTexture, const SDL_Rect* destRect, float rotation, const SDL_Point* center) const {
    SDL_RenderCopyEx(renderer, tempTexture, nullptr, destRect, rotation, center, SDL_FLIP_NONE);
    SDL_DestroyTexture(tempTexture);
}

void H2DE_Engine::H2DE_Renderer::renderHitboxes(H2DE_Object* object) const {
    H2DE_LevelPos pos = H2DE_GetObjectPos(object);
    bool absolute = H2DE_IsObjectAbsolute(object);

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (!hitbox.color.isVisible()) {
            continue;
        }

        if (H2DE_CameraContainsHitbox(engine, pos, hitbox, absolute)) {
            renderHitbox(pos, hitbox, absolute);
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderHitbox(const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute) const {
    H2DE_AbsPos absPos = lvlToAbs(pos + hitbox.rect.getPos(), absolute);
    H2DE_AbsSize absSize = lvlToAbs(hitbox.rect.getSize());

    Sint16 minX = absPos.x;
    Sint16 maxX = absPos.x + absSize.x;
    Sint16 minY = absPos.y;
    Sint16 maxY = absPos.y + absSize.y;

    std::vector<Sint16> vx = { minX, maxX, maxX, minX };
    std::vector<Sint16> vy = { minY, minY, maxY, maxY };

    polygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(hitbox.color));
}

// GETTER
bool H2DE_Engine::H2DE_Renderer::isPositionGreater(H2DE_Object* object1, H2DE_Object* object2) {
    H2DE_LevelPos object1Pos = H2DE_GetObjectPos(object1);
    H2DE_LevelPos object2Pos = H2DE_GetObjectPos(object2);

    bool equalsX = object1Pos.x == object2Pos.x;
    return (equalsX) ? object1Pos.y < object2Pos.y : object1Pos.x < object2Pos.x;
}

const bool H2DE_Engine::H2DE_Renderer::isSurfaceValid(const H2DE_Surface* surface) const {
    auto it = textures.find(surface->sd.textureName);
    return it != textures.end();
}

const unsigned int H2DE_Engine::H2DE_Renderer::getBlockSize() const {
    int blockSize = H2DE_GetWindowSize(engine).x / H2DE_GetCameraSize(engine).x;
    if (blockSize <= 0) {
        blockSize = 1;
    }
    return blockSize;
}

SDL_ScaleMode H2DE_Engine::H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) const {
    return (scaleMode == H2DE_SCALE_MODE_LINEAR) ? SDL_ScaleModeLinear : (scaleMode == H2DE_SCALE_MODE_NEAREST) ? SDL_ScaleModeNearest : SDL_ScaleModeBest;
}

SDL_RendererFlip H2DE_Engine::H2DE_Renderer::getFlip(H2DE_Flip flip) const {
    return (flip == H2DE_FLIP_XY)
        ? SDL_FLIP_NONE
        : (flip == H2DE_FLIP_NONE)
            ? SDL_FLIP_NONE
            : (flip == H2DE_FLIP_X)
                ? SDL_FLIP_HORIZONTAL
                : SDL_FLIP_VERTICAL;
}

H2DE_AbsPos H2DE_Engine::H2DE_Renderer::lvlToAbs(const H2DE_LevelPos& pos, bool absolute) const {
    H2DE_LevelPos camPos = H2DE_GetCameraPos(engine);
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>((pos.x - ((absolute) ? 0.0f : camPos.x)) * blockSize),
        static_cast<int>((pos.y - ((absolute) ? 0.0f : camPos.y)) * blockSize),
    };
}

H2DE_AbsSize H2DE_Engine::H2DE_Renderer::lvlToAbs(const H2DE_LevelSize& size) const {
    const unsigned int blockSize = getBlockSize(); 

    return {
        static_cast<int>(size.x * blockSize),
        static_cast<int>(size.y * blockSize),
    };
}

H2DE_AbsRect H2DE_Engine::H2DE_Renderer::lvlToAbs(const H2DE_LevelRect& rect, bool absolute) const {
    H2DE_AbsPos pos = lvlToAbs(rect.getPos(), absolute);
    H2DE_AbsSize size = lvlToAbs(rect.getSize());
    
    return {
        pos.x,
        pos.y,
        size.x,
        size.y,
    };
}

H2DE_LevelPos H2DE_Engine::H2DE_Renderer::absToLvl(const H2DE_AbsPos& pos, bool absolute) const {
    const unsigned int blockSize = getBlockSize(); 

    H2DE_LevelPos levelCamPos = H2DE_GetCameraPos(engine);
    H2DE_AbsPos absCamPos = {
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
