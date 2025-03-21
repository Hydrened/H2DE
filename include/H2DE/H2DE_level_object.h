#ifndef H2DE_LEVEL_OBJECT_H
#define H2DE_LEVEL_OBJECT_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_object.h>

class H2DE_LevelObject : public H2DE_Object {
private:
    H2DE_LevelObjectData data;

    H2DE_LevelObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_LevelObjectData& lod);
    ~H2DE_LevelObject();
    
public:
    friend H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, const H2DE_ObjectData& lo, const H2DE_LevelObjectData& lod);
    friend void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* obj);
};

#endif
