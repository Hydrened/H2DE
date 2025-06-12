#ifndef H2DE_SURFACE_H
#define H2DE_SURFACE_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Surface {
public:
    inline void show() { hidden = false; }
    inline void hide() { hidden = true; }

    constexpr H2DE_SurfaceData getSurfaceData() const { return surfaceData; }
    constexpr H2DE_Transform getTransform() const { return surfaceData.transform; }
    constexpr H2DE_Translate getTranslate() const { return surfaceData.transform.translate; }
    constexpr H2DE_Scale getScale() const { return surfaceData.transform.scale; }
    constexpr float getRotation() const { return surfaceData.transform.rotation; }
    constexpr H2DE_Pivot getPivot() const { return surfaceData.transform.pivot; }
    constexpr H2DE_ScaleMode getScaleMode() const { return surfaceData.scaleMode; }
    constexpr H2DE_BlendMode getBlendMode() const { return surfaceData.blendMode; }
    constexpr int getIndex() const { return surfaceData.index; }
    constexpr bool isHidden() const { return hidden; }

    void setTranslate(const H2DE_Translate& translate);
    void setScale(const H2DE_Scale& scale);
    void setRotation(float rotation);
    void setPivot(const H2DE_Pivot& pivot);
    void setScaleMode(H2DE_ScaleMode scaleMode);
    void setBlendMode(H2DE_BlendMode blendMode);
    void setIndex(int index);

    H2DE_TimelineID setTranslate(const H2DE_Translate& translate, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setScale(const H2DE_Scale& scale, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);
    H2DE_TimelineID setRotation(float rotation, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive = true);

    friend class H2DE_Renderer;
    friend class H2DE_Object;

protected:
    H2DE_Engine* engine;
    H2DE_Object* object;

    H2DE_Surface(H2DE_Engine* engine, H2DE_Object* object, const H2DE_SurfaceData& surfaceData);
    virtual ~H2DE_Surface() = default;

private:
    H2DE_SurfaceData surfaceData;

    bool hidden = false;

    virtual std::string getTextureName() const = 0;
    virtual H2DE_ColorRGB getColor() const = 0;
    virtual std::optional<H2DE_PixelRect> getSrcRect() const = 0;
    virtual bool isVisible() const = 0;
};

#include <H2DE/surfaces/H2DE_texture.h>
#include <H2DE/surfaces/H2DE_sprite.h>
#include <H2DE/surfaces/H2DE_color.h>

#endif
