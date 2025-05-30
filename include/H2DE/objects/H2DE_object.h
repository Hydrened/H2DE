#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;
class H2DE_Texture;
class H2DE_Sprite;
class H2DE_Color;

class H2DE_Object {
private:
    bool hidden = false;
    bool isGrid = false;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData objectData;

    std::vector<H2DE_Surface*> surfaceBuffers = {};
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    float maxRadius;

    H2DE_Object(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    virtual ~H2DE_Object();

    static void destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    void clearSurfaceBuffer();

    void update();
    void updateCollisions();
    void snap(const H2DE_LevelRect& world_hitboxRect, const H2DE_LevelRect& world_otherHitboxRect, H2DE_Face face);

    virtual void refreshSurfaceBuffers() = 0;
    virtual void refreshMaxRadius() = 0;

    template<typename H2DE_Surface_T, typename H2DE_SurfaceData_T>
    H2DE_Surface_T* addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_SurfaceData_T& specificData) {
        H2DE_Surface_T* surface = new H2DE_Surface_T(engine, this, surfaceData, specificData);
        surfaces[name] = surface;
        refreshMaxRadius();
        refreshSurfaceBuffers();
        return surface;
    }
    inline H2DE_Texture* addTexture(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData) {
        return addSurface<H2DE_Texture, H2DE_TextureData>(surfaces, name, surfaceData, textureData);
    }
    inline H2DE_Sprite* addSprite(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData) {
        return addSurface<H2DE_Sprite, H2DE_SpriteData>(surfaces, name, surfaceData, spriteData);
    }
    inline H2DE_Color* addColor(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData) {
        return addSurface<H2DE_Color, H2DE_ColorData>(surfaces, name, surfaceData, colorData);
    }
    void removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

    static H2DE_Surface* getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

    static const std::vector<H2DE_Surface*> getSortedSurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static const std::array<H2DE_Translate, 4> getCorners(const H2DE_Transform& transform);
    inline bool hasHitbox(const std::string& name) const { return (hitboxes.find(name) != hitboxes.end()); }
    float getMaxHitboxRadius() const;
    float getMaxSurfaceRadius(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) const;

public:
    void addHitbox(const std::string& name, const H2DE_Hitbox& hitbox);
    void removeHitbox(const std::string& name);

    inline void show() { hidden = false; }
    inline void hide() { hidden = true; }

    inline H2DE_ObjectData getObjectData() const { return objectData; }
    inline H2DE_Transform getTransform() const { return objectData.transform; }
    inline H2DE_Translate getTranslate() const { return objectData.transform.translate; }
    inline H2DE_Scale getScale() const { return objectData.transform.scale; }
    inline float getRotation() const { return objectData.transform.rotation; }
    inline H2DE_Pivot getPivot() const { return objectData.transform.pivot; }
    inline Uint8 getOpacity() const { return objectData.opacity; }
    inline bool isAbsolute() const { return objectData.absolute; }
    inline int getIndex() const { return objectData.index; }
    inline bool isHidden() const { return hidden; }
    
    inline std::unordered_map<std::string, H2DE_Hitbox> getHitboxes() const { return hitboxes; }
    H2DE_Hitbox getHitbox(const std::string& name) const;

    void setTranslate(const H2DE_Translate& translate);
    void setScale(const H2DE_Scale& scale);
    void setRotation(float rotation);
    void setPivot(const H2DE_Pivot& pivot);
    void setOpacity(Uint8 opacity);
    void setAbsolute(bool absolute);
    void setIndex(int index);

    unsigned int setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setScale(const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setRotation(float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setOpacity(Uint8 opacity, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    void setHitboxTranslate(const std::string& name, const H2DE_Translate& translate);
    void setHitboxScale(const std::string& name, const H2DE_Scale& scale);
    void setHitboxRotation(const std::string& name, float rotation);
    void setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot);
    void setHitboxColor(const std::string& name, const H2DE_ColorRGB& color);
    void setHitboxCollisionIndex(const std::string& name, int collisionIndex);
    void setHitboxSnap(const std::string& name, bool snap);
    void setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide);

    unsigned int setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setHitboxScale(const std::string& name, const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setHitboxRotation(const std::string& name, float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Engine;
    friend class H2DE_Renderer;
    friend class H2DE_Camera;
    friend class H2DE_Surface;
    friend class H2DE_Texture;
    friend class H2DE_Sprite;
    friend class H2DE_Color;
    friend class H2DE_ButtonManager;
};

#include <H2DE/objects/H2DE_bar_object.h>
#include <H2DE/objects/H2DE_basic_object.h>
#include <H2DE/objects/H2DE_button_object.h>
#include <H2DE/objects/H2DE_text_object.h>

#endif
