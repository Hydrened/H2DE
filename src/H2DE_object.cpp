#include "H2DE_object.h"

H2DE_LevelObject* H2DE_CreateLevelObject() {
    return new H2DE_LevelObject();
}

H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_LevelObject g) {
    return new H2DE_LevelObject(g);
}

void H2DE_DestroyLevelObject(H2DE_LevelObject* g) {
    delete g;
}
