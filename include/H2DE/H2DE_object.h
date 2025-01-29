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
    std::function<void()> updateCall = NULL;

    void snap(H2DE_LevelRect thisRect, H2DE_LevelRect otherRect, H2DE_Face face);

public:
    H2DE_LevelObject(H2DE_Engine* engine, std::vector<H2DE_LevelObject*>* objects, H2DE_LevelObjectData data);
    ~H2DE_LevelObject();

    void update();

    /**
     * Gets a level object's data
     * \returns level object's data
     * \since H2DE-2.0.5
     */
    friend H2DE_LevelObjectData* H2DE_GetObjectData(H2DE_LevelObject* object);

    /**
     * Sets the function called every time the level object is updated
     * \param object pointer to a level object
     * \param updateCall function called when updating
     * \since H2DE-2.0.5
     */
    friend void H2DE_SetObjectUpdateCall(H2DE_LevelObject* object, std::function<void()> updateCall);
    /**
     * Sets the position of a level object
     * \param object pointer to a level object
     * \param pos the new position
     * \since H2DE-2.0.6
     */
    friend void H2DE_SetObjectPos(H2DE_LevelObject* object, H2DE_LevelPos pos);
};

#endif
