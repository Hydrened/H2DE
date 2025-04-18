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

    if (isSurfaceValid(surface)) {
        const H2DE_AbsRect destRect = renderSurfaceGetDestRect(object, surfaceBuffer, absolute);
        
        SDL_Texture* tempTexture = renderSurfaceCreateTempTexture(destRect);
        renderSurfaceSetTextureProperties(tempTexture, surface);
        
        renderSurfaceRenderTextureToTarget(object, surfaceBuffer);
        renderSurfaceRenderFinalTexture(object, surface, tempTexture, destRect);
    }
}

const H2DE_AbsRect H2DE_Engine::H2DE_Renderer::renderSurfaceGetDestRect(const H2DE_Object* object, const H2DE_SurfaceBuffer& surfaceBuffer, bool absolute) const {
    const H2DE_LevelRect surfraceRect = (object->od.pos + surfaceBuffer.offset).makeRect(surfaceBuffer.size);
    const H2DE_LevelRect objRect = object->od.pos.makeRect(object->od.size);

    H2DE_LevelRect relativeSurfaceRect = surfraceRect;
    relativeSurfaceRect.substractPos(object->od.pos);

    H2DE_LevelRect flipedRect = H2DE_Engine::H2DE_Renderer::flipRect(objRect, relativeSurfaceRect, object->od.flip);
    flipedRect = flipedRect.addPos(object->od.pos);

    return lvlToAbsRect(flipedRect, absolute);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceSetTextureProperties(SDL_Texture* texture, const H2DE_Surface* surface) const {
    const H2DE_ColorRGB& color = surface->sd.color;
    const H2DE_ScaleMode& scaleMode = surface->sd.scaleMode;

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureScaleMode(texture, H2DE_Engine::H2DE_Renderer::getScaleMode(scaleMode));
}

SDL_Texture* H2DE_Engine::H2DE_Renderer::renderSurfaceCreateTempTexture(const SDL_Rect& destRect) const {
    SDL_Texture* tempTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, destRect.w, destRect.h);
    if (!tempTexture) {
        throw std::runtime_error("H2DE => \033[31mERROR\033[0m: Failed to create render target: " + std::string(SDL_GetError()));
    }

    SDL_SetTextureBlendMode(tempTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tempTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    return tempTexture;
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceRenderTextureToTarget(const H2DE_Object* object, const H2DE_SurfaceBuffer& surfaceBuffer) const {
    const H2DE_Surface* surface = surfaceBuffer.surface;
    SDL_Texture* texture = textures.find(surface->sd.textureName)->second;

    const std::optional<SDL_Rect>& srcRect = surface->getSrcRect();

    const H2DE_Flip addedFlips = H2DE_AddFlip(surface->sd.flip, object->od.flip);
    const float rotation = (addedFlips == H2DE_FLIP_XY) ? 180.0f : 0.0f;

    const SDL_Point center = lvlToAbsPivot(surfaceBuffer.size.getCenter());
    const SDL_RendererFlip flip = H2DE_Engine::H2DE_Renderer::getFlip(addedFlips);

    if (srcRect.has_value()) {
        SDL_RenderCopyEx(renderer, texture, &srcRect.value(), nullptr, rotation, &center, flip);
    } else {
        SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, rotation, &center, flip);
    }
    SDL_SetRenderTarget(renderer, nullptr);
}

void H2DE_Engine::H2DE_Renderer::renderSurfaceRenderFinalTexture(const H2DE_Object* object, const H2DE_Surface* surface, SDL_Texture* tempTexture, const SDL_Rect& destRect) const {
    const float surfaceRotation = H2DE_Engine::H2DE_Renderer::flipRotation(surface->sd.rotation, object->od.flip);
    const float objectRotation = H2DE_Engine::H2DE_Renderer::flipRotation(object->od.rotation, object->od.flip);
    const float rotation = surfaceRotation + objectRotation;
    const SDL_Point pivot = lvlToAbsPivot(H2DE_Engine::H2DE_Renderer::flipPivot(surface->sd.rect, surface->sd.pivot, object->od.flip));

    SDL_RenderCopyEx(renderer, tempTexture, nullptr, &destRect, rotation, &pivot, SDL_FLIP_NONE);
    SDL_DestroyTexture(tempTexture);
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
