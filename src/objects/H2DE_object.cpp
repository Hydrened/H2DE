#include "H2DE/H2DE_object.h"

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

// GETTER
H2DE_LevelPos H2DE_GetObjectPos(H2DE_Object* object) {
    return object->od.pos;
}

H2DE_LevelSize H2DE_GetObjectSize(H2DE_Object* object) {
    return object->od.size;
}

std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(H2DE_Object* object) {
    return object->od.hitboxes;
}

int H2DE_GetObjectIndex(H2DE_Object* object) {
    return object->od.index;
}

bool H2DE_IsObjectAbsolute(H2DE_Object* object) {
    return object->od.absolute;
}

bool H2DE_IsObjectHidden(H2DE_Object* object) {
    return object->hidden;
}

// SETTER
void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos) {
    object->od.pos = pos;
}

void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size) {
    object->od.size = size;
}

void H2DE_SetObjectIndex(H2DE_Object* object, int index) {
    object->od.index = index;
}

void H2DE_ShowObject(H2DE_Object* object) {
    object->hidden = false;
}

void H2DE_HideObject(H2DE_Object* object) {
    object->hidden = true;
}
