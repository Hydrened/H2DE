#ifndef H2DE_CAMERA_H
#define H2DE_CAMERA_H

#include <H2DE_engine.h>
class H2DE_Engine;

/**
 * Type used to identify a camera
 * \since H2DE-2.0.2
 */
class H2DE_Camera {
private:
    H2DE_Engine* engine;
    H2DE_CameraData data;
    H2DE_LevelPos pos = { 0.0f, 0.0f };

    H2DE_LevelObject* reference = nullptr;
    bool lockedToReference = false;
    H2DE_LevelSize lockedPadding = { 0.0f, 0.0f };
    H2DE_LevelSize padding = { 0.0f, 0.0f };

    void setPaddingFromReference();

public:
    H2DE_Camera(H2DE_Engine* engine, H2DE_CameraData data);
    ~H2DE_Camera();

    void update();

    /**
     * Gets the camera's position
     * \param camera pointer to the camera
     * \returns camera's position
     * \since H2DE-2.0.2
     */
    friend H2DE_LevelPos H2DE_GetCameraPos(H2DE_Camera* camera); 
    /**
     * Gets the camera's size
     * \param camera pointer to the camera
     * \returns camera's size
     * \since H2DE-2.0.2
     */
    friend H2DE_LevelSize H2DE_GetCameraSize(H2DE_Camera* camera);
    /**
     * Determines whether a rect is visible on the camera
     * \param camera pointer to the camera
     * \param hitbox level rect of an element
     * \returns boolean
     * \since H2DE-2.0.2
     */
    friend bool H2DE_CameraContains(H2DE_Camera* camera, H2DE_LevelRect hitbox);

    /**
     * Sets the camera's position
     * \param camera pointer to the camera
     * \param pos new camera's position
     * \since H2DE-2.0.2
     */
    friend void H2DE_SetCameraPos(H2DE_Camera* camera, H2DE_LevelPos pos);
    /**
     * Sets the camera's size
     * 
     * Height will be automatically set by the window ratio
     * \param camera pointer to the camera
     * \param width width of the camera
     * \since H2DE-2.0.2
     */
    friend void H2DE_SetCameraSize(H2DE_Camera* camera, float width);
    /**
     * Sets the smoothing value of a camera
     * \param camera pointer to the camera
     * \param smoothing smoothing of the camera
     * \since H2DE-2.0.6
     */
    friend void H2DE_SetCameraSmoothing(H2DE_Camera* camera, float smoothing);
    /**
     * Sets the camera's reference level object
     * \param camera pointer to the camera
     * \param reference pointer to a level object
     * \since H2DE-2.0.6
     */
    friend void H2DE_SetCameraReference(H2DE_Camera* camera, H2DE_LevelObject* reference);
    /**
     * Sets whether the camera should be locked to it's reference level object
     * \param camera pointer to the camera
     * \param state whether it should be locked
     * \since H2DE-2.0.6
     */
    friend void H2DE_SetCameraLockedToReference(H2DE_Camera* camera, bool state);
    /**
     * Sets the padding on camera edges
     * \param camera pointer to the camera
     * \param padding level size of the padding
     * \since H2DE-2.0.6
     */
    friend void H2DE_SetCameraPadding(H2DE_Camera* camera, H2DE_LevelSize padding);
};

#endif
