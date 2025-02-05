#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE_engine.h>
#include <functional>
class H2DE_Engine;
class H2DE_LevelObject;

/**
 * Type used to identify a level object
 * \since H2DE-2.0.5
 */
class H2DE_LevelObject {
private:
    H2DE_Engine* engine;
    std::vector<H2DE_LevelObject*>* objects;
    H2DE_LevelObjectData data;

    void snap(H2DE_LevelRect thisRect, H2DE_LevelRect otherRect, H2DE_Face face);

public:
    H2DE_LevelObject(H2DE_Engine* engine, std::vector<H2DE_LevelObject*>* objects, H2DE_LevelObjectData data);
    ~H2DE_LevelObject();

    void update();

    /**
     * Gets a level object's data
     * \param object pointer to a level object
     * \returns level object's data
     * \since H2DE-2.0.5
     */
    friend H2DE_LevelObjectData* H2DE_GetObjectData(H2DE_LevelObject* object);
};

#endif
