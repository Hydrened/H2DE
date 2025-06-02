#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include <H2DE/engine/H2DE_engine.h>
#include <string>
#include <limits>
class H2DE_Engine;
class H2DE_BasicObject;

class H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;

    H2DE_BasicObject* grid = nullptr;
    bool onTop = false;

    H2DE_Camera(H2DE_Engine* engine, const H2DE_CameraData& data);
    ~H2DE_Camera();

    void initGrid();

    void destroyGrid();
    void destroyGridHitboxes();

    void update();
    void updateGrid();
    void updateGridObjectTransform();
    void updateGridObjectHitboxes();
    void updateCrosshair();

    H2DE_Scale getScale(float width) const;

public:
    bool containsObject(const H2DE_Object* object) const;
    inline bool containsRect(const H2DE_LevelRect& rect) const { return getWorldRect().collides(rect); }
    inline bool containsPoint(const H2DE_Translate& translate) const { return getWorldRect().collides(translate); }

    inline H2DE_CameraData getData() const { return data; }
    inline H2DE_Translate getTranslate() const { return data.translate; }
    inline H2DE_Scale getGameScale() const { return getScale(data.gameWidth); }
    inline H2DE_Scale getInterfaceScale() const { return getScale(data.interfaceWidth); }
    inline H2DE_LevelRect getWorldRect() const { return data.translate.makeRect(getGameScale()); }
    inline float getGameWidth() const { return data.gameWidth; }
    inline float getInterfaceWidth() const { return data.interfaceWidth; }
    inline float getSmoothing() const { return data.smoothing; }
    inline H2DE_Padding getPadding() const { return data.padding; }
    inline H2DE_Face getXOrigin() const { return data.xOrigin; }
    inline H2DE_Face getYOrigin() const { return data.yOrigin; }

    void setTranslate(const H2DE_Translate& translate);
    void setGameWidth(float width);
    inline void setInterfaceWidth(float width) { data.interfaceWidth = width; }
    inline void setSmoothing(float smoothing) { data.smoothing = smoothing; }
    inline void setPadding(const H2DE_Padding& padding) { data.padding = padding; }
    inline void setXOrigin(H2DE_Face xOrigin) { data.xOrigin = xOrigin; }
    inline void setYOrigin(H2DE_Face yOrigin) { data.yOrigin = yOrigin; }
    inline void setGridOnTop(bool state) { onTop = state; }

    unsigned int setTranslate(const H2DE_Translate& translate, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setGameWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    unsigned int setInterfaceWidth(float width, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);

    friend class H2DE_Engine;
    friend class H2DE_Renderer;
};

#endif
