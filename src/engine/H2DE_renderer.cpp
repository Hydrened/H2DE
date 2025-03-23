#include "H2DE/H2DE_renderer.h"
#include "H2DE/H2DE_bar_object.h"
#include "H2DE/H2DE_basic_object.h"
#include "H2DE/H2DE_button_object.h"
#include "H2DE/H2DE_text_object.h"
#include "H2DE/H2DE_texture.h"
#include "H2DE/H2DE_sprite.h"

// INIT
H2DE_Engine::H2DE_Renderer::H2DE_Renderer(H2DE_Engine* e, SDL_Renderer* r, std::vector<H2DE_Object*>& o) : engine(e), renderer(r), objects(o) {
    
}

// CLEANUP
H2DE_Engine::H2DE_Renderer::~H2DE_Renderer() {
    destroyTextures();
    destroySounds();
}

void H2DE_Engine::H2DE_Renderer::destroyTextures() {
    for (const auto& [name, texture] : textures) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    textures.clear();
}

void H2DE_Engine::H2DE_Renderer::destroySounds() {
    for (const auto& [name, sound] : sounds) {
        if (sound) {
            // delete sound; // crash -1073740940
        }
    }

    sounds.clear();
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
    if (object->od.size.x != 0.0f && object->od.size.y != 0.0f) {
        if (H2DE_CameraContainsObject(engine, object)) {
            for (const H2DE_Surface* surface : object->getSurfaces()) {
                if (surface) {
                    renderSurface(surface, object->od.pos.makeRect(object->od.size), object->od.absolute);
                }
            }
        }
    }

    H2DE_LevelPos pos = H2DE_GetObjectPos(object);
    bool absolute = H2DE_IsObjectAbsolute(object);

    for (const auto& [name, hitbox] : H2DE_GetObjectHitboxes(object)) {
        if (isVisible(hitbox.color)) {
            if (H2DE_CameraContainsHitbox(engine, hitbox, absolute)) {
                renderHitbox(pos, hitbox, absolute);
            }
        }
    }
}

void H2DE_Engine::H2DE_Renderer::renderSurface(const H2DE_Surface* surface, const H2DE_LevelRect& rect, bool absolute) const {
    std::string textureName = surface->sd.textureName;
    
    auto it = textures.find(textureName);
    if (it == textures.end()) {
        return;
    }

    SDL_Rect destRect = lvlToAbs(rect, absolute);
    std::optional<SDL_Rect> srcRect = surface->getSrcRect();
    int rotation = 0; // temp
    SDL_Point pivot = { 0, 0 }; // temp
    SDL_RendererFlip flip = getFlip(H2DE_FLIP_NONE); // temp

    SDL_Texture* texture = it->second;
    H2DE_ColorRGB color = surface->sd.color;

    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    SDL_SetTextureScaleMode(texture, getScaleMode(surface->sd.scaleMode));

    if (srcRect.has_value()) {
        SDL_RenderCopyEx(renderer, texture, &srcRect.value(), &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, &pivot, flip);
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

bool H2DE_Engine::H2DE_Renderer::isVisible(const H2DE_ColorRGB& color) {
    return color.a != 0;
}

const unsigned int H2DE_Engine::H2DE_Renderer::getBlockSize() const {
    return H2DE_GetWindowSize(engine).x / H2DE_GetCameraSize(engine).x;
}

SDL_ScaleMode H2DE_Engine::H2DE_Renderer::getScaleMode(H2DE_ScaleMode scaleMode) const {
    return (scaleMode == H2DE_SCALE_MODE_LINEAR) ? SDL_ScaleModeLinear : (scaleMode == H2DE_SCALE_MODE_NEAREST) ? SDL_ScaleModeNearest : SDL_ScaleModeBest;
}

SDL_RendererFlip H2DE_Engine::H2DE_Renderer::getFlip(H2DE_Flip flip) const {
    return (flip == H2DE_FLIP_NONE) ? SDL_FLIP_NONE : (flip == H2DE_FLIP_HORIZONTAL) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL;
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
