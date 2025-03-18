#include "H2DE/H2DE_level_object.h"

// INIT
H2DE_LevelObject::H2DE_LevelObject(H2DE_Engine* e) : engine(e) {

}

H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine) {
    return new H2DE_LevelObject(engine);
}

// CLEANUP
H2DE_LevelObject::~H2DE_LevelObject() {
    
}

void H2DE_DestroyLevelObject(H2DE_LevelObject* object) {
    if (object) delete object;
}
