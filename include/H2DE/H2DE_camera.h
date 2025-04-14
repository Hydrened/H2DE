#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include <H2DE/H2DE_engine.h>

#undef min
#undef max

class H2DE_Engine::H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;
    H2DE_LevelPos pos;

    H2DE_Object* reference = nullptr;
    bool lockedToReference = false;
    H2DE_LevelPadding lockedPadding = { 0.0f, 0.0f, 0.0f, 0.0f };

    bool isObjectRectOnScreen(const H2DE_Object* object) const;
    bool isASurfaceOfObjectOnScreen(const H2DE_Object* object) const;
    bool isAHitboxOfObjectOnScreen(const H2DE_Object* object) const;

public:
    H2DE_Camera(H2DE_Engine* engine, const H2DE_CameraData& data);
    ~H2DE_Camera();

    void update();

    friend H2DE_LevelPos H2DE_GetCameraPos(const H2DE_Engine* engine);
    friend H2DE_LevelSize H2DE_GetCameraSize(const H2DE_Engine* engine);
    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object);
    friend bool H2DE_CameraContainsHitbox(const H2DE_Engine* engine, const H2DE_LevelPos& pos, const H2DE_Hitbox& hitbox, bool absolute);
    friend bool H2DE_CameraContainsRect(const H2DE_Engine* engine, const H2DE_LevelRect& rect, bool absolute);
    friend void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos);
    friend void H2DE_SetCameraPos(const H2DE_Engine* engine, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    friend void H2DE_SetCameraWidth(const H2DE_Engine* engine, float width);
    friend void H2DE_SetCameraSmoothing(const H2DE_Engine* engine, float smoothing);
    friend void H2DE_SetCameraReference(const H2DE_Engine* engine, H2DE_Object* object);
    friend void H2DE_SetCameraLockedToReference(const H2DE_Engine* engine, bool state);
    friend void H2DE_SetCameraPadding(const H2DE_Engine* engine, const H2DE_LevelPadding& padding);
    friend void H2DE_SetCameraPaddingFromReference(const H2DE_Engine* engine, const H2DE_LevelPadding& padding);
};

#endif
