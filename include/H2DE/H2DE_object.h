#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE_engine.h>
#include <functional>
class H2DE_Engine;
class H2DE_LevelObject;

class H2DE_LevelObject {
private:
    H2DE_Engine* engine;
    std::vector<H2DE_LevelObject*>* objects;
    H2DE_LevelObjectData data;
    std::function<void()> updateCall = NULL;

    void snap(H2DE_LevelRect thisRect, H2DE_LevelRect otherRect, H2DE_Face face);

public:
    H2DE_LevelObject(H2DE_Engine* engine, std::vector<H2DE_LevelObject*>* objects, H2DE_LevelObjectData data);
    ~H2DE_LevelObject();

    void update();

    H2DE_LevelObjectData getData() const;

    friend void H2DE_SetObjectUpdateCall(H2DE_LevelObject* object, std::function<void()> updateCall);
};

#endif
