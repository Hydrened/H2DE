#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

#include <H2DE/engine/H2DE_engine.h>
#include <H2DE/engine/H2DE_error.h>
class H2DE_Engine;
class H2DE_Texture;
class H2DE_Sprite;
class H2DE_Color;

class H2DE_Object {
public:
    void addHitbox(const std::string& name, const H2DE_Hitbox& hitbox);
    bool removeHitbox(const std::string& name);

    inline void show() { hidden = false; }
    inline void hide() { hidden = true; }

    constexpr H2DE_ObjectData getObjectData() const { return objectData; }
    constexpr H2DE_Transform getTransform() const { return objectData.transform; }
    constexpr H2DE_Translate getTranslate() const { return objectData.transform.translate; }
    constexpr H2DE_Scale getScale() const { return objectData.transform.scale; }
    constexpr float getRotation() const { return objectData.transform.rotation; }
    constexpr H2DE_Pivot getPivot() const { return objectData.transform.pivot; }
    constexpr uint8_t getOpacity() const { return objectData.opacity; }
    constexpr bool isAbsolute() const { return objectData.absolute; }
    constexpr int getIndex() const { return objectData.index; }
    constexpr bool isHidden() const { return hidden; }
    
    inline std::unordered_map<std::string, H2DE_Hitbox> getHitboxes() const { return hitboxes; }
    H2DE_Hitbox getHitbox(const std::string& name) const;

    void setTranslate(const H2DE_Translate& translate);
    void setScale(const H2DE_Scale& scale);
    void setRotation(float rotation);
    void setPivot(const H2DE_Pivot& pivot);
    void setOpacity(uint8_t opacity);
    void setAbsolute(bool absolute);
    void setIndex(int index);

    H2DE_TimelineID setTranslate(const H2DE_Translate& translate, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setScale(const H2DE_Scale& scale, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setRotation(float rotation, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setOpacity(uint8_t opacity, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    void setHitboxTranslate(const std::string& name, const H2DE_Translate& translate);
    void setHitboxScale(const std::string& name, const H2DE_Scale& scale);
    void setHitboxRotation(const std::string& name, float rotation);
    void setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot);
    void setHitboxColor(const std::string& name, const H2DE_ColorRGB& color);
    void setHitboxCollisionIndex(const std::string& name, int collisionIndex);
    void setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide);

    H2DE_TimelineID setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setHitboxScale(const std::string& name, const H2DE_Scale& scale, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setHitboxRotation(const std::string& name, float rotation, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    friend class H2DE_Engine;
    friend class H2DE_Renderer;
    friend class H2DE_Camera;
    friend class H2DE_Surface;
    friend class H2DE_Texture;
    friend class H2DE_Sprite;
    friend class H2DE_Color;
    friend class H2DE_ObjectManager;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData objectData;

    std::vector<H2DE_Surface*> surfaceBuffers = {};
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    float maxRadius;

    H2DE_Object(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    virtual ~H2DE_Object() = default;

    static void destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static void destroySurfaces(std::vector<H2DE_Surface*>& surfaces);

    void update();
    void updateCollisions();

    virtual void refreshSurfaceBuffers() = 0;
    virtual void refreshMaxRadius() = 0;

    template<typename H2DE_Surface_T>
    H2DE_Surface_T* addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        H2DE_Surface_T* surface = new H2DE_Surface_T(engine, this, surfaceData, specificData);
        surfaces[name] = surface;
        refreshMaxRadius();
        refreshSurfaceBuffers();
        return surface;
    }
    bool removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

    template<typename H2DE_Surface_T>
    static H2DE_Surface_T* getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
        auto it = surfaces.find(name);
        if (it == surfaces.end()) {
            H2DE_Error::throwError("Surface named \"" + name + "\" not found");
        }

        H2DE_Surface_T* castedSurface = dynamic_cast<H2DE_Surface_T*>(it->second);
        if (castedSurface == nullptr) {
            H2DE_Error::throwError("Can't cast surface \"" + name +  "\" to template type");
        }

        return castedSurface;
    }

    static const std::vector<H2DE_Surface*> getSortedSurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static const std::array<H2DE_Translate, 4> getCorners(const H2DE_Transform& transform);
    inline bool hasHitbox(const std::string& name) const { return (hitboxes.find(name) != hitboxes.end()); }
    float getMaxHitboxRadius() const;
    float getMaxSurfaceRadius(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) const;

private:
    bool hidden = false;
    bool isGrid = false;
};

#include <H2DE/objects/H2DE_bar_object.h>
#include <H2DE/objects/H2DE_basic_object.h>
#include <H2DE/objects/H2DE_button_object.h>
#include <H2DE/objects/H2DE_text_object.h>
#include <H2DE/objects/H2DE_timer_object.h>

#endif
