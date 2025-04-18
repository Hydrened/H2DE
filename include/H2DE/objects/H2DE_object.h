#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <sstream>
#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Object {
private:
    bool hidden = false;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData od;

    std::vector<H2DE_SurfaceBuffer> surfaceBuffers = {};
    
    H2DE_Object(H2DE_Engine* engine, H2DE_ObjectData od);
    virtual ~H2DE_Object();

    static void destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static H2DE_Surface* getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);
    
    void update();
    void updateCollision();
    void snap(const H2DE_LevelPos& offset, const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face);

    virtual void resetSurfaceBuffers() = 0;
    void clearSurfaceBuffers();

    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const;
    H2DE_Hitbox& getHitbox(const std::string& hitboxName);

    void addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, H2DE_Surface* surface, const std::string& name);
    void removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

public:
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);

    friend void H2DE_AddHitboxToObject(H2DE_Object* object, const H2DE_Hitbox& hitbox, const std::string& hitboxName);
    friend void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& hitboxName);

    friend H2DE_LevelPos H2DE_GetObjectPos(const H2DE_Object* object);
    friend H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object);
    friend float H2DE_GetObjectRotation(const H2DE_Object* object);
    friend H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object);
    friend H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object);
    friend std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object);
    friend int H2DE_GetObjectIndex(const H2DE_Object* object);
    friend bool H2DE_IsObjectAbsolute(const H2DE_Object* object);
    friend bool H2DE_IsObjectHidden(const H2DE_Object* object);

    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectIndex(H2DE_Object* object, int index);
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation);
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot);
    friend void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip);
    friend void H2DE_SetObjectHitboxRect(H2DE_Object* object, const std::string& hitboxName, const H2DE_LevelRect& rect);
    friend void H2DE_SetObjectHitboxColor(H2DE_Object* object, const std::string& hitboxName, const H2DE_ColorRGB& color);
    friend void H2DE_SetObjectHitboxCollisionIndex(H2DE_Object* object, const std::string& hitboxName, int index);
    friend void H2DE_SetObjectHitboxSnap(H2DE_Object* object, const std::string& hitboxName, bool snap);
    friend void H2DE_SetObjectHitboxOnCollide(H2DE_Object* object, const std::string& hitboxName, const std::function<void(H2DE_Object*, H2DE_Face)>& call);
    friend void H2DE_ShowObject(H2DE_Object* object);
    friend void H2DE_HideObject(H2DE_Object* object);

    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object);

    friend class H2DE_Engine;
};

#include <H2DE/objects/H2DE_bar_object.h>
#include <H2DE/objects/H2DE_basic_object.h>
#include <H2DE/objects/H2DE_button_object.h>
#include <H2DE/objects/H2DE_text_object.h>

#endif
