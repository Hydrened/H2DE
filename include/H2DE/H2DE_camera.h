#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;
    H2DE_Translate t;

    H2DE_Camera(H2DE_Engine* engine, const H2DE_CameraData& data);
    ~H2DE_Camera();

    void update();

public:
    bool containsObject(const H2DE_Object* object) const;
    inline bool containsRect(const H2DE_LevelRect& rect) const { return getWorldRect().collides(rect); }
    inline bool containsPoint(const H2DE_Translate& translate) const { return getWorldRect().collides(translate); }

    inline H2DE_CameraData getData() const { return data; }
    inline H2DE_Translate getTranslate() const { return t; }
    H2DE_Scale getScale() const;
    inline H2DE_LevelRect getWorldRect() const { return t.makeRect(getScale()); }
    inline float getWidth() const { return data.width; }
    inline float getSmoothing() const { return data.smoothing; }
    inline H2DE_Padding getPadding() const { return data.padding; }
    inline H2DE_Face getXOrigin() const { return data.xOrigin; }
    inline H2DE_Face getYOrigin() const { return data.yOrigin; }

    inline void setTranslate(const H2DE_Translate& translate) { t = translate; }
    inline void setWidth(float width) { data.width = width; }
    inline void setSmoothing(float smoothing) { data.smoothing = smoothing; }
    inline void setPadding(const H2DE_Padding& padding) { data.padding = padding; }
    inline void setXOrigin(H2DE_Face xOrigin) { data.xOrigin = xOrigin; }
    inline void setYOrigin(H2DE_Face yOrigin) { data.yOrigin = yOrigin; }

    unsigned int setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Engine;
};

#endif
