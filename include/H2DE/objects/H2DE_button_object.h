#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

class H2DE_ButtonObject : public H2DE_Object {
private:
    H2DE_ButtonObjectData buttonObjectData;

    H2DE_TextObject* textObject = nullptr;
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    H2DE_ButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    ~H2DE_ButtonObject() override;

    void refreshTextObject();
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

    void mouseDown();
    void mouseUp();
    void mouseHover();
    void mouseBlur();

    inline H2DE_ButtonObjectData getButtonData() const { return buttonObjectData; }
    inline bool isPauseSensitive() const { return buttonObjectData.pauseSensitive; }

    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const { return surfaces; }
    inline H2DE_Surface* getSurface(const std::string& name) const { return H2DE_Object::getSurface(surfaces, name); }
    inline H2DE_TextObject* getTextObject() const { return textObject; }

    inline void setMouseDown(const std::function<void(H2DE_Object*)>& onMouseDown) { buttonObjectData.onMouseDown = onMouseDown; }
    inline void setMouseUp(const std::function<void(H2DE_Object*)>& onMouseUp) { buttonObjectData.onMouseUp = onMouseUp; }
    inline void setMouseHover(const std::function<void(H2DE_Object*)>& onHover) { buttonObjectData.onHover = onHover; }
    inline void setMouseBlur(const std::function<void(H2DE_Object*)>& onBlur) { buttonObjectData.onBlur = onBlur; }
    inline void setPauseSensitive(bool pauseSensitive) { buttonObjectData.pauseSensitive = pauseSensitive; }

    friend class H2DE_Engine;
    friend class H2DE_ButtonManager;
};

#endif
