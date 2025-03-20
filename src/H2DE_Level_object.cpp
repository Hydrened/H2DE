#include "H2DE/H2DE_level_object.h"

// INIT
H2DE_LevelObject::H2DE_LevelObject(H2DE_Engine* e, H2DE_LevelObjectData d) : engine(e), data(d) {
    
}

H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, H2DE_LevelObjectData data) {
    H2DE_LevelObject* object = new H2DE_LevelObject(engine, data);
    engine->objects.push_back(object);
    std::cout << engine->objects.size() << std::endl;
    return object;
}

// CLEANUP
H2DE_LevelObject::~H2DE_LevelObject() {
    
}

void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* object) {
    if (object) {
        auto it = std::find(engine->objects.begin(), engine->objects.end(), object);

        if (it != engine->objects.end()) {
            engine->objects.erase(it);
            delete object;
            object = nullptr;
            std::cout << "removed" << std::endl;
        }
    }
}
