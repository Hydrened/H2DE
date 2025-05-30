#ifndef H2DE_BASIC_OBJECT_H
#define H2DE_BASIC_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BasicObject : public H2DE_Object {
private:
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    H2DE_BasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData);
    ~H2DE_BasicObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

public:
    inline H2DE_Texture* addTexture(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_TextureData& textureData) {
        return H2DE_Object::addTexture(surfaces, name, surfaceData, textureData);
    }
    inline H2DE_Sprite* addSprite(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_SpriteData& spriteData) {
        return H2DE_Object::addSprite(surfaces, name, surfaceData, spriteData);
    }
    inline H2DE_Color* addColor(const std::string& name, const H2DE_SurfaceData& surfaceData, const H2DE_ColorData& colorData) {
        return H2DE_Object::addColor(surfaces, name, surfaceData, colorData);
    }
    inline void removeSurface(const std::string& name) {
        H2DE_Object::removeSurface(surfaces, name);
    }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const { return surfaces; }
    inline H2DE_Surface* getSurface(const std::string& name) const { return H2DE_Object::getSurface(surfaces, name); }

    friend class H2DE_Engine;
};

#endif
