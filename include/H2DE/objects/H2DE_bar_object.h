#ifndef H2DE_BAR_OBJECT_H
#define H2DE_BAR_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData barObjectData;

    std::unordered_map<std::string, H2DE_Surface*> frontSurfaces = {};
    std::unordered_map<std::string, H2DE_Surface*> backgroundSurfaces = {};

    H2DE_BarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    ~H2DE_BarObject() override;

    void updateSurfaceBuffers() override;
    void updateMaxRadius() override;

public:
    inline H2DE_Texture* addTextureToFront(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData) {
        return H2DE_Object::addTexture(frontSurfaces, name, surfaceData, textureData);
    }
    inline H2DE_Sprite* addSpriteToFront(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData) {
        return H2DE_Object::addSprite(frontSurfaces, name, surfaceData, spriteData);
    }
    inline H2DE_Color* addColorToFront(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData) {
        return H2DE_Object::addColor(frontSurfaces, name, surfaceData, colorData);
    }
    inline void removeSurfaceFromFront(const std::string& name) {
        H2DE_Object::removeSurface(frontSurfaces, name);
    }

    inline H2DE_Texture* addTextureToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData) {
        return H2DE_Object::addTexture(backgroundSurfaces, name, surfaceData, textureData);
    }
    inline H2DE_Sprite* addSpriteToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData) {
        return H2DE_Object::addSprite(backgroundSurfaces, name, surfaceData, spriteData);
    }
    inline H2DE_Color* addColorToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData) {
        return H2DE_Object::addColor(backgroundSurfaces, name, surfaceData, colorData);
    }
    inline void removeSurfaceFromBackground(const std::string& name) {
        H2DE_Object::removeSurface(backgroundSurfaces, name);
    }

    inline H2DE_BarObjectData getBarData() const { return barObjectData; }
    inline float getMin() const { return barObjectData.min; }
    inline float getMax() const { return barObjectData.max; }
    inline float getValue() const { return barObjectData.value; }

    inline std::unordered_map<std::string, H2DE_Surface*> getFrontSurfaces() const { return frontSurfaces; }
    inline std::unordered_map<std::string, H2DE_Surface*> getBackgroundSurfaces() const { return backgroundSurfaces; }
    inline H2DE_Surface* getFrontSurface(const std::string& name) const { return H2DE_Object::getSurface(frontSurfaces, name); }
    inline H2DE_Surface* getBackgroundSurface(const std::string& name) const { return H2DE_Object::getSurface(backgroundSurfaces, name); }

    void setMin(float min);
    void setMax(float max);
    void setValue(float value);

    unsigned int setMin(float min, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setMax(float max, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setValue(float value, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Engine;
};

#endif
