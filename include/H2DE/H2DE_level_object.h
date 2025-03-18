#ifndef H2DE_LEVEL_OBJECT_H
#define H2DE_LEVEL_OBJECT_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_LevelObject {
private:
    H2DE_Engine* engine;

    H2DE_LevelObject(H2DE_Engine* engine);
    ~H2DE_LevelObject();

public:
    friend H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine);
    friend void H2DE_DestroyLevelObject(H2DE_LevelObject* object);
};

#endif
