#include "H2DE/objects/H2DE_object.h"

// INIT
H2DE_Object::H2DE_Object(H2DE_Engine* e, H2DE_ObjectData d) : engine(e), od(d) {

}

// CLEANUP
H2DE_Object::~H2DE_Object() {
    
}

void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object) {
    if (object) {
        auto it = std::find(engine->objects.begin(), engine->objects.end(), object);

        if (it != engine->objects.end()) {
            engine->objects.erase(it); 
            delete object;
        }
    }
}

// EVENTS
void H2DE_AddHitboxToObject(H2DE_Object* object, const std::string& name, const H2DE_Hitbox& hitbox) {
    if (object->od.hitboxes.find(name) != object->od.hitboxes.end()) {
        std::cout << "H2DE => \033[33mWarning\033[0m: Hitbox " << '"' << name << '"' << " has been overridden" << std::endl;
    }

    object->od.hitboxes[name] = hitbox;
}

void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& name) {
    auto it = object->od.hitboxes.find(name);

    if (it != object->od.hitboxes.end()) {
        object->od.hitboxes.erase(it);
    }
}

void H2DE_Object::snap(const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face) {
    switch (face) {
        case H2DE_FACE_TOP:
            od.pos.y = otherRect.y + otherRect.h;
            break;

        case H2DE_FACE_BOTTOM:
            od.pos.y = otherRect.y - rect.h;
            break;

        case H2DE_FACE_LEFT:
            od.pos.x = otherRect.x + otherRect.w;
            break;

        case H2DE_FACE_RIGHT:
            od.pos.x = otherRect.x - rect.w;
            break;

        default: return;
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

    for (auto [name, hitbox] : od.hitboxes) {
        if (!hitbox.onCollide && !hitbox.snap) {
            continue;
        }

        H2DE_LevelRect rect = hitbox.rect.addPos(od.pos);

        for (H2DE_Object* otherObject : engine->objects) {
            if (otherObject == this) {
                continue;
            }

            for (auto [otherName, otherHitbox] : od.hitboxes) {
                if (otherHitbox.collisionIndex != hitbox.collisionIndex) {
                    continue;
                }

                H2DE_LevelRect otherRect = otherHitbox.rect.addPos(H2DE_GetObjectPos(otherObject));

                if (rect.collides(otherRect)) {
                    if (hitbox.onCollide) {
                        hitbox.onCollide(otherObject);
                    }

                    if (hitbox.snap) {
                        std::optional<H2DE_Face> face = rect.getCollidedFace(otherRect);
                        if (face.has_value()) {
                            snap(rect, otherRect, face.value());
                        }
                    }
                }
            }
        }
    }
}

// GETTER
H2DE_LevelPos H2DE_GetObjectPos(const H2DE_Object* object) {
    return object->od.pos;
}

H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object) {
    return object->od.size;
}

float H2DE_GetObjectRotation(const H2DE_Object* object) {
    return object->od.rotation;
}

H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object) {
    return object->od.pivot;
}

H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object) {
    return object->od.flip;
}

std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object) {
    return object->od.hitboxes;
}

int H2DE_GetObjectIndex(const H2DE_Object* object) {
    return object->od.index;
}

bool H2DE_IsObjectAbsolute(const H2DE_Object* object) {
    return object->od.absolute;
}

bool H2DE_IsObjectHidden(const H2DE_Object* object) {
    return object->hidden;
}

// SETTER
void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos) {
    object->od.pos = pos;

    H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);
    if (text) {
        text->resetSurfaces();
    }
}

void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    const H2DE_LevelPos defaultPos = H2DE_GetObjectPos(object);
    const H2DE_LevelPos posToAdd = pos - defaultPos;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultPos, posToAdd](float blend) {
        H2DE_SetObjectPos(object, defaultPos + (posToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size) {
    object->od.size = size;

    H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);
    if (text) {
        text->resetSurfaces();
    }
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    const H2DE_LevelSize defaultSize = H2DE_GetObjectSize(object);
    const H2DE_LevelSize sizeToAdd = size - defaultSize;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultSize, sizeToAdd](float blend) {
        H2DE_SetObjectSize(object, defaultSize + (sizeToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectIndex(H2DE_Object* object, int index) {
    object->od.index = index;
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation) {
    object->od.rotation = rotation;

    H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);
    if (text) {
        text->resetSurfaces();
    }
}

void H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive) {
    const float defaultRotation = H2DE_GetObjectRotation(object);
    const float rotationToAdd = rotation - defaultRotation;

    H2DE_CreateTimeline(object->engine, duration, easing, [object, defaultRotation, rotationToAdd](float blend) {
        H2DE_SetObjectRotation(object, defaultRotation + (rotationToAdd * blend));
    }, nullptr, 0, pauseSensitive);
}

void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot) {
    object->od.pivot = pivot;

    H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);
    if (text) {
        text->resetSurfaces();
    }
}

void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip) {
    object->od.flip = flip;

    H2DE_TextObject* text = dynamic_cast<H2DE_TextObject*>(object);
    if (text) {
        text->resetSurfaces();
    }
}

void H2DE_ShowObject(H2DE_Object* object) {
    object->hidden = false;
}

void H2DE_HideObject(H2DE_Object* object) {
    object->hidden = true;
}
