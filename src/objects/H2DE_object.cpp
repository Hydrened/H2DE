#include "H2DE/objects/H2DE_object.h"
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

void H2DE_Object::clearSurfaceBuffers() {
    surfaceBuffers.clear();
}

// UPDATE
void H2DE_Object::update() {
    updateCollision();
}

void H2DE_Object::updateCollision() {
    if (hidden) {
        return;
    }

    const H2DE_LevelRect offset = od.pos.makeRect({ 0.0f, 0.0f });

    for (const auto& [name, hitbox] : od.hitboxes) {
        if (!hitbox.onCollide && !hitbox.snap) {
            continue;
        }

        const H2DE_LevelRect rect = hitbox.rect + offset;

        for (H2DE_Object* otherObject : engine->objects) {
            H2DE_Error::checkObject(otherObject);

            if (otherObject == this) {
                continue;
            }

            const H2DE_LevelRect otherOffset = otherObject->od.pos.makeRect({ 0.0f, 0.0f });

            for (const auto& [otherName, otherHitbox] : otherObject->od.hitboxes) {
                if (otherHitbox.collisionIndex != hitbox.collisionIndex) {
                    continue;
                }

                const H2DE_LevelRect otherRect = otherHitbox.rect + otherOffset;

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
                    snap(offset.getPos(), rect, otherRect, face.value());
                }
            }
        }
    }
}

void H2DE_Object::snap(const H2DE_LevelPos& offset, const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face) {
    const H2DE_LevelPos o = offset - rect.getPos();

    switch (face) {
        case H2DE_FACE_TOP:
            od.pos.y = otherRect.y + otherRect.h + o.y;
            break;

        case H2DE_FACE_RIGHT:
            od.pos.x = otherRect.x - rect.w + o.x;
            break;

        case H2DE_FACE_BOTTOM:
            od.pos.y = otherRect.y - rect.h + o.y;
            break;

        case H2DE_FACE_LEFT:
            od.pos.x = otherRect.x + otherRect.w + o.x;
            break;

        default: return;
    }

    resetSurfaceBuffers();
}

// GETTER
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
    return object->od.pos;
}

H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.size;
}

float H2DE_GetObjectRotation(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.rotation;
}

H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.pivot;
}

H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.flip;
}

std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object) {
    H2DE_Error::checkObject(object);
    return object->od.hitboxes;
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

    object->od.pos = pos;
}

void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelPos defaultPos = H2DE_GetObjectPos(object);
    const H2DE_LevelPos posToAdd = pos - defaultPos;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultPos, posToAdd](float blend) {
        H2DE_SetObjectPos(object, defaultPos + (posToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size) {
    H2DE_Error::checkObject(object);

    object->od.size = size;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const H2DE_LevelSize defaultSize = H2DE_GetObjectSize(object);
    const H2DE_LevelSize sizeToAdd = size - defaultSize;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultSize, sizeToAdd](float blend) {
        H2DE_SetObjectSize(object, defaultSize + (sizeToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectIndex(H2DE_Object* object, int index) {
    H2DE_Error::checkObject(object);
    object->od.index = index;
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation) {
    H2DE_Error::checkObject(object);

    object->od.rotation = rotation;
    object->resetSurfaceBuffers();
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    H2DE_Error::checkObject(object);

    const float defaultRotation = H2DE_GetObjectRotation(object);
    const float rotationToAdd = rotation - defaultRotation;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultRotation, rotationToAdd](float blend) {
        H2DE_SetObjectRotation(object, defaultRotation + (rotationToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot) {
    H2DE_Error::checkObject(object);

    object->od.pivot = pivot;
    object->resetSurfaceBuffers();
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
