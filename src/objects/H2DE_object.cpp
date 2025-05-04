#include "H2DE/objects/H2DE_object.h"
#include "H2DE/H2DE_transform.h"
#include "H2DE/H2DE_error.h"

// INIT
H2DE_Object::H2DE_Object(H2DE_Engine* e, H2DE_ObjectData d) : engine(e), od(d) {
    
}

// CLEANUP
H2DE_Object::~H2DE_Object() {
    
}

void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object) {
    if (object != nullptr) {
        H2DE_Error::checkEngine(engine);

        const auto it = std::find(engine->objects.begin(), engine->objects.end(), object);

        if (it != engine->objects.end()) {
            engine->objects.erase(it); 
            delete object;
            object = nullptr;
        }
    }
}

void H2DE_Object::destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    for (auto [name, surface] : surfaces) {
        if (surface) {
            delete surface;
            surface = nullptr;
        }
    }
    surfaces.clear();
}

// HITBOXES
void H2DE_AddHitboxToObject(H2DE_Object* object, const H2DE_Hitbox& hitbox, const std::string& hitboxName) {
    H2DE_Error::checkObject(object);

    if (object->od.hitboxes.find(hitboxName) != object->od.hitboxes.end()) {
        H2DE_Error::logWarning("Hitbox \"" + hitboxName + "\" has been overridden");
    }

    object->od.hitboxes[hitboxName] = hitbox;
}

void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& hitboxName) {
    H2DE_Error::checkObject(object);

    const auto it = object->od.hitboxes.find(hitboxName);

    if (it != object->od.hitboxes.end()) {
        object->od.hitboxes.erase(it);
    }
}

// SURFACES
void H2DE_Object::addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, H2DE_Surface* surface, const std::string& name) {
    H2DE_Error::checkSurface(surface);

    if (surfaces.find(name) != surfaces.end()) {
        H2DE_Error::logWarning("Surface \"" + name + "\" has been overridden");
    }

    surfaces[name] = surface;
    resetSurfaceBuffers();
}

void H2DE_Object::removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    const auto it = surfaces.find(name);

    if (it != surfaces.end()) {
        surfaces.erase(it);
        resetSurfaceBuffers();
    }
}

// UPDATE
void H2DE_Object::update() {
    updateCollision();
}

void H2DE_Object::updateCollision() {
    if (hidden) {
        return;
    }

    using T = H2DE_Transform;

    for (const auto& [name, hitbox] : od.hitboxes) {
        if (!hitbox.onCollide && !hitbox.snap) {
            continue;
        }

        const H2DE_LevelRect rect = T::getHitboxWorldDestRect(od.rect, od.pivot, od.rotation, od.flip, hitbox.rect);

        for (H2DE_Object* otherObject : engine->objects) {
            H2DE_Error::checkObject(otherObject);

            if (otherObject == this) {
                continue;
            }

            for (const auto& [otherName, otherHitbox] : otherObject->od.hitboxes) {
                if (otherHitbox.collisionIndex != hitbox.collisionIndex) {
                    continue;
                }

                const H2DE_ObjectData& otherOd = otherObject->od;
                const H2DE_LevelRect otherRect = T::getHitboxWorldDestRect(otherOd.rect, otherOd.pivot, otherOd.rotation, otherOd.flip, otherHitbox.rect);

                if (!rect.collides(otherRect)) {
                    continue;
                }

                const std::optional<H2DE_Face> face = rect.getCollidedFace(otherRect);

                if (!face.has_value()) {
                    continue;
                }

                if (hitbox.onCollide) {
                    hitbox.onCollide(otherObject, face.value());
                }

                if (hitbox.snap) {
                    snap(rect, otherRect, face.value());
                }
            }
        }
    }
}

void H2DE_Object::snap(const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face) {
    const H2DE_LevelPos& W_objPos = od.rect.getPos();
    const H2DE_LevelPos offset = W_objPos - rect.getPos();

    switch (face) {
        case H2DE_FACE_TOP:
            od.rect.y = otherRect.y + otherRect.h + offset.y;
            break;

        case H2DE_FACE_RIGHT:
            od.rect.x = otherRect.x - rect.w + offset.x;
            break;

        case H2DE_FACE_BOTTOM:
            od.rect.y = otherRect.y - rect.h + offset.y;
            break;

        case H2DE_FACE_LEFT:
            od.rect.x = otherRect.x + otherRect.w + offset.x;
            break;

        default: return;
    }

    resetSurfaceBuffers();
}

// GETTER
const std::vector<H2DE_Surface*> H2DE_Object::getSortedSurfaces(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) {
    std::vector<H2DE_Surface*> res;
    res.reserve(surfaces.size());

    for (const auto& [name, surface] : surfaces) {
        res.push_back(surface);
    }

    std::sort(res.begin(), res.end(), [](H2DE_Surface* a, H2DE_Surface* b) {
        return a->sd.index < b->sd.index;
    });

    return res;
}

H2DE_Surface* H2DE_Object::getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
    const auto it = surfaces.find(name);
    if (it == surfaces.end()) {
        H2DE_Error::logError("Surface named \"" + name + "\" not found");
    }
    return it->second;
}

std::vector<H2DE_SurfaceBuffer> H2DE_Object::getSurfaceBuffers() const {
    return surfaceBuffers;
}

H2DE_Hitbox& H2DE_Object::getHitbox(const std::string& hitboxName) {
    const auto it = od.hitboxes.find(hitboxName);
    if (it == od.hitboxes.end()) {
        H2DE_Error::logError("Hitbox named \"" + hitboxName + "\" not found");
    }
    return it->second;
}

H2DE_LevelPos H2DE_GetObjectPos(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.rect.getPos();
}

H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.rect.getSize();
}

H2DE_LevelRect H2DE_GetObjectRect(const H2DE_Object* object) {
    return object->od.rect;
}

H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.pivot;
}

float H2DE_GetObjectRotation(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.rotation;
}

H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.flip;
}

std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.hitboxes;
}

H2DE_Hitbox H2DE_GetObjectHitbox(const H2DE_Object* object, const std::string& name) {
    H2DE_Error::checkObject(object);

    auto it = object->od.hitboxes.find(name);
    if (it == object->od.hitboxes.end()) {
        H2DE_Error::logError("Hitbox named \"" + name + "\" not found");
    }

    return it->second;
}

H2DE_LevelRect H2DE_GetObjectHitboxWorldRect(const H2DE_Object* object, const std::string& name) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelRect L_hitboxRect = H2DE_GetObjectHitbox(object, name).rect;
    const H2DE_ObjectData& od = object->od;

    return H2DE_Transform::getHitboxWorldDestRect(od.rect, od.pivot, od.rotation, od.flip, L_hitboxRect);
}

int H2DE_GetObjectIndex(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.index;
}

bool H2DE_IsObjectAbsolute(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.absolute;
}

bool H2DE_IsObjectHidden(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->hidden;
}

// SETTER
void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos) {
    H2DE_Error::checkObject(object);
    object->od.rect.x = pos.x;
    object->od.rect.y = pos.y;
}

void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelPos defaultPos = object->od.rect.getPos();
    const H2DE_LevelPos posToAdd = pos - defaultPos;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultPos, posToAdd](float blend) {
        H2DE_SetObjectPos(object, defaultPos + (posToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size) {
    H2DE_Error::checkObject(object);

    object->od.rect.w = size.x;
    object->od.rect.h = size.y;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelSize& defaultSize = object->od.rect.getSize();
    const H2DE_LevelSize sizeToAdd = size - defaultSize;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultSize, sizeToAdd](float blend) {
        H2DE_SetObjectSize(object, defaultSize + (sizeToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectRect(H2DE_Object* object, const H2DE_LevelRect& rect) {
    H2DE_Error::checkObject(object);
    object->od.rect = rect;
}

void H2DE_SetObjectRect(H2DE_Object* object, const H2DE_LevelRect& rect, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelRect& defaultRect = object->od.rect;
    const H2DE_LevelPos posToAdd = rect.getPos() - defaultRect.getPos();
    const H2DE_LevelSize sizeToAdd = rect.getSize() - defaultRect.getSize();

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultRect, posToAdd, sizeToAdd](float blend) {
        H2DE_LevelRect interpolatedRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        interpolatedRect.addPos(defaultRect.getPos() + (posToAdd * blend));
        interpolatedRect.addSize(defaultRect.getSize() + (sizeToAdd * blend));

        H2DE_SetObjectRect(object, interpolatedRect);
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectIndex(H2DE_Object* object, int index) {
    H2DE_Error::checkObject(object);
    object->od.index = index;
}

void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot) {
    H2DE_Error::checkObject(object);

    object->od.pivot = pivot;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation) {
    H2DE_Error::checkObject(object);

    object->od.rotation = rotation;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const float& defaultRotation = object->od.rotation;
    const float rotationToAdd = rotation - defaultRotation;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultRotation, rotationToAdd](float blend) {
        H2DE_SetObjectRotation(object, defaultRotation + (rotationToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip) {
    H2DE_Error::checkObject(object);

    object->od.flip = flip;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectHitboxRect(H2DE_Object* object, const std::string& hitboxName, const H2DE_LevelRect& rect) {
    H2DE_Error::checkObject(object);
    object->getHitbox(hitboxName).rect = rect;
}

void H2DE_SetObjectHitboxColor(H2DE_Object* object, const std::string& hitboxName, const H2DE_ColorRGB& color) {
    H2DE_Error::checkObject(object);
    object->getHitbox(hitboxName).color = color;
}

void H2DE_SetObjectHitboxCollisionIndex(H2DE_Object* object, const std::string& hitboxName, int index) {
    H2DE_Error::checkObject(object);
    object->getHitbox(hitboxName).collisionIndex = index;
}

void H2DE_SetObjectHitboxSnap(H2DE_Object* object, const std::string& hitboxName, bool snap) {
    H2DE_Error::checkObject(object);
    object->getHitbox(hitboxName).snap = snap;
}

void H2DE_SetObjectHitboxOnCollide(H2DE_Object* object, const std::string& hitboxName, const std::function<void(H2DE_Object*, H2DE_Face)>& call) {
    H2DE_Error::checkObject(object);
    object->getHitbox(hitboxName).onCollide = call;
}

void H2DE_ShowObject(H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    object->hidden = false;
}

void H2DE_HideObject(H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    object->hidden = true;
}
