#include "H2DE/H2DE_level_object.h"

// INIT
H2DE_LevelObject::H2DE_LevelObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_LevelObjectData& lod) : H2DE_Object(engine, od), data(lod) {

}

H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_LevelObjectData& lod) {
    H2DE_LevelObject* obj = new H2DE_LevelObject(engine, od, lod);
    engine->addLevelObject(obj);
    return obj;
}

// CLEANUP
H2DE_LevelObject::~H2DE_LevelObject() {

}

void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* obj) {
    if (obj) {
        engine->destroyLevelObject(obj);
        delete obj;
        obj = nullptr;
    }
}
