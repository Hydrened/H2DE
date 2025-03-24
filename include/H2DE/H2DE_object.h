#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Object {
private:
    bool hidden = false;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData od;
    
    H2DE_Object(H2DE_Engine* engine, H2DE_ObjectData od);
    virtual ~H2DE_Object();

    void update();
    void updateCollision();
    virtual void updateImpl() = 0;

    void snap(const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face);
    
    virtual std::vector<H2DE_Surface*> getSurfaces() const = 0;

public:
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);

    friend void H2DE_AddHitboxToObject(H2DE_Object* object, const std::string& name, const H2DE_Hitbox& hitbox);
    friend void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& name);

    friend H2DE_LevelPos H2DE_GetObjectPos(H2DE_Object* object);
    friend H2DE_LevelSize H2DE_GetObjectSize(H2DE_Object* object);
    friend std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(H2DE_Object* object);
    friend int H2DE_GetObjectIndex(H2DE_Object* object);
    friend bool H2DE_IsObjectAbsolute(H2DE_Object* object);
    friend bool H2DE_IsObjectHidden(H2DE_Object* object);

    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    friend void H2DE_SetObjectIndex(H2DE_Object* object, int index);
    friend void H2DE_ShowObject(H2DE_Object* object);
    friend void H2DE_HideObject(H2DE_Object* object);

    friend class H2DE_Engine;
};

#endif
