#include "H2DE/objects/H2DE_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"
#include "H2DE/engine/H2DE_geometry.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Object::H2DE_Object(H2DE_Engine* e, const H2DE_ObjectData& od) : engine(e), objectData(od) {

}

// CLEANUP
H2DE_Object::~H2DE_Object() {
    clearSurfaceBuffer();
}

void H2DE_Object::destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    for (const auto& [name, surface] : surfaces) {
        delete surface;
    }

    surfaces.clear();
}

void H2DE_Object::clearSurfaceBuffer() {
    surfaceBuffers.clear();
}

// UPDATE
void H2DE_Object::update() {
    updateCollisions();
}

void H2DE_Object::updateCollisions() {
    if (hidden || objectData.absolute || isGrid) {
        return;
    }

    for (const auto& [name, hitbox] : hitboxes) {
        if (!hitbox.onCollide && !hitbox.snap) {
            continue;
        }

        const H2DE_LevelRect world_hitboxRect = G::getHitboxRect(this, hitbox);
        const int& collisionIndex = hitbox.collisionIndex;

        for (H2DE_Object* otherObject : engine->objects) {
            if (otherObject == this || otherObject->objectData.absolute || otherObject->hidden || otherObject->isGrid) {
                continue;
            }

            const H2DE_ObjectData& otherObjectData = otherObject->objectData;

            for (const auto& [otherName, otherHitbox] : otherObject->hitboxes) {
                if (collisionIndex != otherHitbox.collisionIndex) {
                    continue;
                }

                const H2DE_LevelRect world_otherHitboxRect = G::getHitboxRect(otherObject, otherHitbox);

                if (!world_hitboxRect.collides(world_otherHitboxRect)) {
                    continue;
                }

                const std::optional<H2DE_Face> possibleCollidedFace = world_hitboxRect.getCollidedFace(world_otherHitboxRect);
                if (!possibleCollidedFace.has_value()) {
                    continue;
                }

                if (hitbox.onCollide) {
                    hitbox.onCollide(otherObject, possibleCollidedFace.value());
                }

                if (hitbox.snap) {
                    snap(world_hitboxRect, world_otherHitboxRect, possibleCollidedFace.value());
                }
            }
        }
    }
}

void H2DE_Object::snap(const H2DE_LevelRect& world_hitboxRect, const H2DE_LevelRect& world_otherHitboxRect, H2DE_Face face) {
    const H2DE_Translate hitboxOffset = world_hitboxRect.getTranslate() - objectData.transform.translate;
    const H2DE_Translate hitboxesSizeOffset = (world_otherHitboxRect.getScale() + world_hitboxRect.getScale()) * 0.5f;

    switch (face) {
        case H2DE_FACE_TOP:
            objectData.transform.translate.y = world_otherHitboxRect.y - hitboxOffset.y + hitboxesSizeOffset.y;
            break;

        case H2DE_FACE_BOTTOM:
            objectData.transform.translate.y = world_otherHitboxRect.y - hitboxOffset.y - hitboxesSizeOffset.y;
            break;

        case H2DE_FACE_LEFT:
            objectData.transform.translate.x = world_otherHitboxRect.x - hitboxOffset.x + hitboxesSizeOffset.x;
            break;

        case H2DE_FACE_RIGHT:
            objectData.transform.translate.x = world_otherHitboxRect.x - hitboxOffset.x - hitboxesSizeOffset.x;
            break;

        default: return;
    }

    updateSurfaceBuffers();
}

// ACTIONS

// -- surfaces
void H2DE_Object::removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    auto it = surfaces.find(name);

    if (it != surfaces.end()) {
        surfaces.erase(it);
        updateMaxRadius();
        updateSurfaceBuffers();
    }
}

void H2DE_Object::updateSurfaceBuffers() {
    clearSurfaceBuffer();
}

// -- hitboxes
void H2DE_Object::addHitbox(const std::string& name, const H2DE_Hitbox& hitbox) {
    hitboxes[name] = hitbox;
    updateMaxRadius();
}

void H2DE_Object::removeHitbox(const std::string& name) {
    auto it = hitboxes.find(name);
    
    if (it != hitboxes.end()) {
        hitboxes.erase(it);
        updateMaxRadius();
    }
}

// GETTER
const std::vector<H2DE_Surface*> H2DE_Object::getSortedSurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    std::vector<H2DE_Surface*> res;
    res.reserve(surfaces.size());

    for (const auto& [name, surface] : surfaces) {
        res.push_back(surface);
    }

    std::sort(res.begin(), res.end(), [](H2DE_Surface* a, H2DE_Surface* b) {
        return a->surfaceData.index < b->surfaceData.index;
    });

    return res;
}

const std::array<H2DE_Translate, 4> H2DE_Object::getCorners(const H2DE_Transform& transform) {
    const H2DE_Translate& translate = transform.translate;
    const H2DE_Pivot& pivot = transform.pivot;
    const float& rotation = transform.rotation;

    float offsetX = pivot.x - translate.x;
    float offsetY = pivot.y - translate.y;
    
    float scaledX = translate.x - offsetX;
    float scaledY = translate.y - offsetY;
    
    float halfW = transform.scale.x * 0.5f;
    float halfH = transform.scale.y * 0.5f;
    
    return {
        H2DE_Translate{ scaledX - halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY + halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX - halfW, scaledY + halfH }.rotate(pivot, rotation),
    };
}

H2DE_Hitbox H2DE_Object::getHitbox(const std::string& name) const {
    auto it = hitboxes.find(name);
    if (it == hitboxes.end()) {
        H2DE_Error::logError("Hitbox named \"" + name + "\" not found");
    }
        
    return it->second;
}

float H2DE_Object::getMaxHitboxRadius() const {
    float res = 0.0f;
    const H2DE_Translate world_objectTranslate = objectData.transform.translate;

    for (const auto& [name, hitbox] : hitboxes) {
        for (const H2DE_Translate& corner : H2DE_Object::getCorners(hitbox.transform)) {

            const H2DE_Translate world_hitboxCorner = corner + world_objectTranslate;
            float distance = std::abs(world_objectTranslate.getDistanceSquared(world_hitboxCorner));

            if (distance > res) {
                res = distance;
            }
        }
    }

    return std::sqrt(res);
}

float H2DE_Object::getMaxSurfaceRadius(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) const {
    float res = 0.0f;
    const H2DE_Translate world_objectTranslate = objectData.transform.translate;

    for (const auto& [name, surface] : surfaces) {
        for (const H2DE_Translate& corner : H2DE_Object::getCorners(surface->getTransform())) {

            const H2DE_Translate world_hitboxCorner = corner + world_objectTranslate;
            float distance = std::abs(world_objectTranslate.getDistanceSquared(world_hitboxCorner));

            if (distance > res) {
                res = distance;
            }
        }
    }

    return std::sqrt(res);
}

H2DE_Surface* H2DE_Object::getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    auto it = surfaces.find(name);
    if (it == surfaces.end()) {
        H2DE_Error::throwError("Surface named \"" + name + "\" not found");
    }

    return it->second;
}

// SETTER

// -- non lerp
void H2DE_Object::setTranslate(const H2DE_Translate& translate) {
    objectData.transform.translate = translate;
    updateSurfaceBuffers();
}

void H2DE_Object::setScale(const H2DE_Scale& scale) {
    objectData.transform.scale = scale;
    updateSurfaceBuffers();
}

void H2DE_Object::setRotation(float rotation) {
    objectData.transform.rotation = rotation;
    updateSurfaceBuffers();
}

void H2DE_Object::setPivot(const H2DE_Pivot& pivot) {
    objectData.transform.pivot = pivot;
    updateSurfaceBuffers();
}

void H2DE_Object::setOpacity(Uint8 opacity) {
    objectData.opacity = opacity;
    updateSurfaceBuffers();
}

void H2DE_Object::setAbsolute(bool absolute) {
    objectData.absolute = absolute;
    updateSurfaceBuffers();
}

void H2DE_Object::setIndex(int index) {
    objectData.index = index;
    updateSurfaceBuffers();
}

void H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.translate = translate;
        updateMaxRadius();
    }
}

void H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.scale = scale;
        updateMaxRadius();
    }
}

void H2DE_Object::setHitboxRotation(const std::string& name, float rotation) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.rotation = rotation;
        updateMaxRadius();
    }
}

void H2DE_Object::setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot) {
    if (hasHitbox(name)) {
        hitboxes[name].transform.pivot = pivot;
        updateMaxRadius();
    }
}

void H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color) {
    if (hasHitbox(name)) {
        hitboxes[name].color = color;
    }
}

void H2DE_Object::setHitboxCollisionIndex(const std::string& name, int collisionIndex) {
    if (hasHitbox(name)) {
        hitboxes[name].collisionIndex = collisionIndex;
    }
}

void H2DE_Object::setHitboxSnap(const std::string& name, bool snap) {
    if (hasHitbox(name)) {
        hitboxes[name].snap = snap;
    }
}

void H2DE_Object::setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide) {
    if (hasHitbox(name)) {
        hitboxes[name].onCollide = onCollide;
    }
}

// -- lerp
unsigned int H2DE_Object::setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, objectData.transform.translate, translate, duration, easing, [this](H2DE_Translate iv) {
        setTranslate(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setScale(const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, objectData.transform.scale, scale, duration, easing, [this](H2DE_Scale iv) {
        setScale(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setRotation(float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, objectData.transform.rotation, rotation, duration, easing, [this](float iv) {
        setRotation(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setOpacity(Uint8 opacity, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<Uint8>(engine, objectData.opacity, opacity, duration, easing, [this](Uint8 iv) {
        setRotation(iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Translate>(engine, getHitbox(name).transform.translate, translate, duration, easing, [this, name](H2DE_Translate iv) {
        setHitboxTranslate(name, iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setHitboxScale(const std::string& name, const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<H2DE_Scale>(engine, getHitbox(name).transform.scale, scale, duration, easing, [this, name](H2DE_Scale iv) {
        setHitboxScale(name, iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setHitboxRotation(const std::string& name, float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp<float>(engine, getHitbox(name).transform.rotation, rotation, duration, easing, [this, name](float iv) {
        setHitboxRotation(name, iv);
    }, completed, pauseSensitive);
}

unsigned int H2DE_Object::setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    return H2DE_LerpManager::lerp(engine, getHitbox(name).color, color, duration, easing, [this, name](H2DE_ColorRGB iv) {
        setHitboxColor(name, iv);
    }, completed, pauseSensitive);
}
