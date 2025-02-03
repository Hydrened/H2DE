#include "H2DE_camera.h"

// INIT
H2DE_Camera::H2DE_Camera(H2DE_Engine* e, H2DE_CameraData d) : engine(e), data(d) {

}

// CLEANUP
H2DE_Camera::~H2DE_Camera() {

}

// UPDATE
void H2DE_Camera::update() {
    if (!reference) return;

    H2DE_LevelObjectData refData = *H2DE_GetObjectData(reference);
    H2DE_LevelRect refRect = refData.pos.makeHitbox(refData.texture.size);

    H2DE_LevelSize camSize = H2DE_GetCameraSize(this);
    float smoothing = data.smoothing;

    if (!lockedToReference) {
        if (refRect.x < pos.x + padding.w) {
            pos.x =  H2DE_Lerp(pos.x, refRect.x - padding.w, smoothing);
        }
        else if (refRect.x + refRect.w > pos.x + camSize.w - padding.w) {
            pos.x = H2DE_Lerp(pos.x, refRect.x + refRect.w + padding.w - camSize.w, smoothing);
        }
        if (refRect.y < pos.y + padding.h) {
            pos.y = H2DE_Lerp(pos.y, refRect.y - padding.h, smoothing);
        }
        else if (refRect.y + refRect.h > pos.y + camSize.h - padding.h) {
            pos.y = H2DE_Lerp(pos.y, refRect.y + refRect.h + padding.h - camSize.h, smoothing);
        }

    } else pos = refRect.getPos() - lockedPadding.toPos();
}

// GETTER
H2DE_LevelPos H2DE_GetCameraPos(H2DE_Camera* camera) {
    return camera->pos;
} 

H2DE_LevelSize H2DE_GetCameraSize(H2DE_Camera* camera) {
    static H2DE_Window* window = H2DE_GetWindow(camera->engine);
    H2DE_AbsSize windowSize = H2DE_GetWindowSize(window);

    H2DE_LevelSize size = H2DE_LevelSize();
    size.w = camera->data.width;
    size.h = static_cast<float>(windowSize.h) / static_cast<float>(windowSize.w) * camera->data.width;
    return size;
}

bool H2DE_CameraContains(H2DE_Camera* camera, H2DE_LevelRect hitbox) {
    H2DE_LevelRect camHitbox = camera->pos.makeHitbox(H2DE_GetCameraSize(camera));
    return (camHitbox.collides(hitbox));
}

// SETTER
void H2DE_SetCameraPos(H2DE_Camera* camera, H2DE_LevelPos pos) {
    camera->pos = pos;
}

void H2DE_SetCameraSize(H2DE_Camera* camera, float width) {
    camera->data.width = width;
}

void H2DE_SetCameraSmoothing(H2DE_Camera* camera, float smoothing) {
    camera->data.smoothing = smoothing;
}

void H2DE_SetCameraReference(H2DE_Camera* camera, H2DE_LevelObject* reference) {
    camera->reference = reference;
    if (camera->lockedToReference) camera->setPaddingFromReference();
}

void H2DE_SetCameraLockedToReference(H2DE_Camera* camera, bool state) {
    camera->lockedToReference = state;
    if (camera->reference) camera->setPaddingFromReference();
}

void H2DE_SetCameraPadding(H2DE_Camera* camera, H2DE_LevelSize padding) {
    camera->padding = padding;
}

void H2DE_Camera::setPaddingFromReference() {
    H2DE_LevelPos diff = pos - (*H2DE_GetObjectData(reference)).pos;
    lockedPadding = { std::abs(diff.x), std::abs(diff.y) };
}
