#include "H2DE/H2DE_object.h"
#undef min

// INIT
H2DE_LevelObject::H2DE_LevelObject(H2DE_Engine* e, std::vector<H2DE_LevelObject*>* o, H2DE_LevelObjectData d) : engine(e), objects(o), data(d) {

}

// CLEANUP
H2DE_LevelObject::~H2DE_LevelObject() {
    delete data.texture;
}

// EVENTS
void H2DE_LevelObject::snap(H2DE_LevelRect objRect, H2DE_LevelRect otherRect, H2DE_Face face) {
    switch (face) {
        case H2DE_NO_FACE: return;

        case H2DE_TOP_FACE:
            this->data.pos.y = otherRect.y + otherRect.h;
            this->data.velocity.y = 0.0f;
            break;

        case H2DE_BOTTOM_FACE:
            this->data.pos.y = otherRect.y - objRect.h;
            this->data.velocity.y = 0.0f;
            break;

        case H2DE_LEFT_FACE:
            this->data.pos.x = otherRect.x + otherRect.w;
            this->data.velocity.x = 0.0f;
            break;

        case H2DE_RIGHT_FACE:
            this->data.pos.x = otherRect.x - objRect.w;
            this->data.velocity.x = 0.0f;
            break;

        default: return;
    }
}

void H2DE_ShowLevelObject(H2DE_LevelObject* object) {
    object->hidden = false;
}

void H2DE_HideLevelObject(H2DE_LevelObject* object) {
    object->hidden = true;
}

// UPDATE
void H2DE_LevelObject::update() {
    static H2DE_GameData* gameData = H2DE_GetGameData(engine);

    if (data.gravity) {
        float gravity = gameData->gravity;
        float maxGravity = gameData->maxGravity;

        data.velocity.y = data.velocity.y + gravity;
        data.velocity.y = std::min(data.velocity.y, maxGravity);
        data.pos = data.pos + data.velocity;
    }

    for (const auto& [key1, objHitbox] : data.hitboxes) {
        H2DE_LevelRect objRect = objHitbox.rect + data.pos;
        if (!objHitbox.onCollide.has_value() && !objHitbox.snap) continue;

        for (H2DE_LevelObject* otherObj : *objects) {
            if (otherObj == this) continue;

            H2DE_LevelObjectData otherData = otherObj->data;
            for (const auto& [key2, otherHitbox] : otherData.hitboxes) {
                if (objHitbox.collisionIndex != otherHitbox.collisionIndex) continue;
                
                H2DE_LevelRect otherRect = otherHitbox.rect + otherData.pos;
                H2DE_Face face = objRect.collides(otherRect);
                if (face != H2DE_NO_FACE) {
                    if (objHitbox.snap) snap(objRect, otherRect, face);
                    if (objHitbox.onCollide.has_value()) objHitbox.onCollide.value()(otherObj);
                }
            }
        }
    }
}

// GETTER
H2DE_LevelObjectData* H2DE_GetLevelObjectData(H2DE_LevelObject* object) {
    return &(object->data);
}

bool H2DE_IsLevelObjectHidden(H2DE_LevelObject* object) {
    return object->hidden;
}
