#include "H2DE_camera.h"

// INIT
H2DE_Camera::H2DE_Camera(H2DE_Engine* e, H2DE_CameraData d) : engine(e), data(d) {

}

// CLEANUP
H2DE_Camera::~H2DE_Camera() {

}

// GETTER
H2DE_LevelPos H2DE_GetCameraPos(H2DE_Camera* camera) {
    return camera->pos;
} 

H2DE_LevelSize H2DE_GetCameraSize(H2DE_Camera* camera) {
    static H2DE_Window* window = H2DE_GetWindow(camera->engine);
    H2DE_AbsSize windowSize = H2DE_GetWindowSize(window);

    H2DE_LevelSize size = H2DE_LevelSize();
    size.w = camera->data.blockOnWidth;
    size.h = windowSize.w / windowSize.h * camera->data.blockOnWidth;
    return size;
}

float H2DE_GetCameraZoom(H2DE_Camera* camera) {
    return camera->zoom;
}

bool H2DE_CameraContains(H2DE_Camera* camera, H2DE_LevelHitbox hitbox) {
    H2DE_LevelHitbox camHitbox = camera->pos.makeHitbox(H2DE_GetCameraSize(camera));
    return (camHitbox.collides(hitbox));
}

// SETTER
void H2DE_SetCameraPos(H2DE_Camera* camera, H2DE_LevelPos pos) {
    camera->pos = pos;
}

void H2DE_SetCameraSize(H2DE_Camera* camera, float blockOnWidth) {
    camera->data.blockOnWidth = blockOnWidth;
}

void H2DE_SetCameraZoom(H2DE_Camera* camera, float zoom) {
    camera->zoom = zoom;
}
