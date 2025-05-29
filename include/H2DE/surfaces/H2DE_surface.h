#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Surface {
private:
    H2DE_Object* object;
    H2DE_SurfaceData surfaceData;

    bool hidden = false;

    virtual std::optional<H2DE_PixelRect> getSrcRect() const = 0;

protected:
    H2DE_Engine* engine;

    H2DE_Surface(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData);
    virtual ~H2DE_Surface();

public:
    inline void show() { hidden = false; }
    inline void hide() { hidden = true; }

    inline H2DE_SurfaceData getSurfaceData() const { return surfaceData; }
    inline std::string getTextureName() const { return surfaceData.textureName; }
    inline H2DE_Transform getTransform() const { return surfaceData.transform; }
    inline H2DE_Translate getTranslate() const { return surfaceData.transform.translate; }
    inline H2DE_Scale getScale() const { return surfaceData.transform.scale; }
    inline float getRotation() const { return surfaceData.transform.rotation; }
    inline H2DE_Pivot getPivot() const { return surfaceData.transform.pivot; }
    inline H2DE_ColorRGB getColor() const { return surfaceData.color; }
    inline H2DE_ScaleMode getScaleMode() const { return surfaceData.scaleMode; }
    inline H2DE_BlendMode getBlendMode() const { return surfaceData.blendMode; }
    inline int getIndex() const { return surfaceData.index; }
    inline bool isHidden() const { return hidden; }

    inline void setTextureName(const std::string& textureName) { surfaceData.textureName = textureName; }
    void setTranslate(const H2DE_Translate& translate);
    void setScale(const H2DE_Scale& scale);
    void setRotation(float rotation);
    void setPivot(const H2DE_Pivot& pivot);
    inline void setColor(const H2DE_ColorRGB& color) { surfaceData.color = color; }
    inline void setScaleMode(H2DE_ScaleMode scaleMode) { surfaceData.scaleMode = scaleMode; }
    inline void setBlendMode(H2DE_BlendMode blendMode) { surfaceData.blendMode = blendMode; }
    inline void setIndex(int index) { surfaceData.index = index; }

    unsigned int setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setScale(const H2DE_Scale& scale, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setRotation(float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setColor(const H2DE_ColorRGB& color, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Renderer;
    friend class H2DE_Object;
};

#include <H2DE/surfaces/H2DE_texture.h>
#include <H2DE/surfaces/H2DE_sprite.h>
#include <H2DE/surfaces/H2DE_color.h>

#endif
