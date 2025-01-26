#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include "H2DE_engine.h"
class H2DE_Engine;

class H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;
    H2DE_LevelPos pos = { 0.0f, 0.0f };
    float zoom = 1.0f;

public:
    H2DE_Camera(H2DE_Engine* engine, H2DE_CameraData data);
    ~H2DE_Camera();

    friend H2DE_LevelPos H2DE_GetCameraPos(H2DE_Camera* camera); 
    friend H2DE_LevelSize H2DE_GetCameraSize(H2DE_Camera* camera);
    friend float H2DE_GetCameraZoom(H2DE_Camera* camera);
    friend bool H2DE_CameraContains(H2DE_Camera* camera, H2DE_LevelHitbox hitbox);

    friend void H2DE_SetCameraPos(H2DE_Camera* camera, H2DE_LevelPos pos);
    friend void H2DE_SetCameraSize(H2DE_Camera* camera, float blockOnWidth);
    friend void H2DE_SetCameraZoom(H2DE_Camera* camera, float zoom);
};

#endif
