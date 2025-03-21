#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;
    H2DE_LevelPos pos;

    H2DE_LevelObject* reference = nullptr;
    bool lockedToReference = false;
    H2DE_LevelPadding lockedPadding = { 0.0f, 0.0f, 0.0f, 0.0f };

public:
    H2DE_Camera(H2DE_Engine* engine, const H2DE_CameraData& data);
    ~H2DE_Camera();

    friend H2DE_LevelPos H2DE_GetCameraPos(H2DE_Engine* engine);
    friend H2DE_LevelSize H2DE_GetCameraSize(H2DE_Engine* engine);
    friend bool H2DE_CameraContains(H2DE_Engine* engine, const H2DE_Hitbox& hitbox);
    friend void H2DE_SetCameraPos(H2DE_Engine* engine, const H2DE_LevelPos& pos);
    friend void H2DE_SetCameraWidth(H2DE_Engine* engine, const float& width);
    friend void H2DE_SetCameraSmoothing(H2DE_Engine* engine, const float& smoothing);
    friend void H2DE_SetCameraReference(H2DE_Engine* engine, H2DE_LevelObject* object);
    friend void H2DE_SetCameraLockedToReference(H2DE_Engine* engine, const bool& state);
    friend void H2DE_SetCameraPadding(H2DE_Engine* engine, const H2DE_LevelPadding& padding);
};

#endif
