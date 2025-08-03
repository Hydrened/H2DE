#pragma once

/**
 * @file H2DE_camera.h
 * @brief Camera system for the H2DE 2D engine.
 * 
 * This file defines the H2DE_Camera class, which manages the viewport, 
 * scaling, positioning, and smooth animated transitions of the camera 
 * within the 2D game world. It handles collision detection with objects, 
 * rectangle areas, and points to determine visibility inside the camera view.
 * 
 * The camera supports game scale and interface scale independently, 
 * padding for viewport adjustments, origin alignment, and smoothing for 
 * smooth camera movement and zoom effects.
 * 
 * Animation of camera properties (position, zoom) is done through lerp-based 
 * timeline animations, supporting easing functions and pause sensitivity.
 */

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;
class H2DE_BasicObject;

/**
 * @class H2DE_Camera
 * @brief Represents the 2D camera for rendering the game world.
 * 
 * The H2DE_Camera class manages the viewport in world coordinates,
 * offering utilities to check if objects or areas are visible inside the camera view.
 * 
 * It holds internal data for position, scale, padding, origin faces, smoothing,
 * and supports animated transitions of position and zoom with timeline-based interpolation.
 * 
 * It also handles an optional grid overlay that can be drawn on top of the scene.
 */
class H2DE_Camera {
public:
    /**
     * @brief Check if the camera's viewport contains a given game object.
     * 
     * Tests if the object's bounding circle (defined by its position and maxRadius)
     * intersects with the camera's world rectangle.
     * 
     * @param object Pointer to the H2DE_Object to test.
     * @return true if the object is at least partially inside the camera's viewport, false otherwise.
     */
    bool containsObject(const H2DE_Object* object) const;
    /**
     * @brief Check if a rectangular area is inside the camera's viewport.
     * 
     * Uses collision detection between the camera's world rectangle and the given rectangle.
     * 
     * @param rect Rectangle in world coordinates.
     * @return true if the rectangle intersects the camera viewport, false otherwise.
     */
    inline bool containsRect(const H2DE_LevelRect& rect) const {
        return getWorldRect().collides(rect);
    }
    /**
     * @brief Check if a point is inside the camera's viewport.
     * 
     * Tests if the point lies within the camera's world rectangle.
     * 
     * @param translate Point coordinates in world space.
     * @return true if the point is inside the camera's viewport, false otherwise.
     */
    inline bool containsPoint(const H2DE_Translate& translate) const {
        return getWorldRect().collides(translate);
    }

    /**
     * @brief Get the camera's internal data struct.
     * 
     * @return A copy of the current H2DE_CameraData.
     */
    inline H2DE_CameraData getData() const noexcept {
        return data;
    }
    /**
     * @brief Get the camera's current position in world coordinates.
     * 
     * @return The translation vector of the camera.
     */
    constexpr H2DE_Translate getTranslate() const noexcept {
        return data.translate;
    }
    /**
     * @brief Calculate and get the game scale based on the game width.
     * 
     * @return The scale factor used for game rendering.
     */
    inline H2DE_Scale getGameScale() const {
        return getScale(data.gameWidth);
    }
    /**
     * @brief Calculate and get the interface scale based on the interface width.
     * 
     * @return The scale factor used for interface/UI rendering.
     */
    inline H2DE_Scale getInterfaceScale() const {
        return getScale(data.interfaceWidth);
    }
    /**
     * @brief Get the camera's world rectangle, representing its viewport.
     * 
     * Uses the camera's translate position and game scale to build the rectangle.
     * 
     * @return The rectangular area covered by the camera in world coordinates.
     */
    inline H2DE_LevelRect getWorldRect() const {
        return getTranslate().makeRect(getGameScale());
    }
    /**
     * @brief Get the camera's game width.
     * 
     * @return The width used for game scale calculations.
     */
    constexpr float getGameWidth() const noexcept {
        return data.gameWidth;
    }
    /**
     * @brief Get the camera's interface width.
     * 
     * @return The width used for interface scale calculations.
     */
    constexpr float getInterfaceWidth() const noexcept {
        return data.interfaceWidth;
    }
    /**
     * @brief Get the smoothing factor for camera movement.
     * 
     * @return The smoothing coefficient.
     */
    constexpr float getSmoothing() const noexcept {
        return data.smoothing;
    }
    /**
     * @brief Get the camera's padding settings.
     * 
     * @return Padding around the camera viewport.
     */
    constexpr H2DE_Padding getPadding() const noexcept {
        return data.padding;
    }
    /**
     * @brief Get the horizontal origin face of the camera.
     * 
     * @return The X axis origin enum value.
     */
    constexpr H2DE_Face getXOrigin() const noexcept {
        return data.xOrigin;
    }
    /**
     * @brief Get the vertical origin face of the camera.
     * 
     * @return The Y axis origin enum value.
     */
    constexpr H2DE_Face getYOrigin() const noexcept {
        return data.yOrigin;
    }
    /**
     * @brief Check if the horizontal origin is inverted (right side).
     * 
     * @return true if origin is on the right, false otherwise.
     */
    constexpr bool isXOriginInverted() const noexcept{
        return (data.xOrigin == H2DE_FACE_RIGHT);
    }
    /**
     * @brief Check if the vertical origin is inverted (bottom side).
     * 
     * @return true if origin is on the bottom, false otherwise.
     */
    constexpr bool isYOriginInverted() const noexcept {
        return (data.yOrigin == H2DE_FACE_BOTTOM);
    }

    /**
     * @brief Set the camera's world position.
     * 
     * @param translate New translation vector to move the camera.
     */
    void setTranslate(const H2DE_Translate& translate);
    /**
     * @brief Set the width used for game scaling.
     * 
     * @param width New game width value.
     */
    void setGameWidth(float width);
    /**
     * @brief Set the width used for interface scaling.
     * 
     * @param width New interface width value.
     */
    inline void setInterfaceWidth(float width) noexcept {
        data.interfaceWidth = width;
    }
    /**
     * @brief Set the smoothing factor for camera movements.
     * 
     * @param smoothing New smoothing coefficient.
     */
    inline void setSmoothing(float smoothing) noexcept {
        data.smoothing = smoothing;
    }
    /**
     * @brief Set the padding around the camera viewport.
     * 
     * @param padding New padding settings.
     */
    inline void setPadding(const H2DE_Padding& padding) noexcept {
        data.padding = padding;
    }
    /**
     * @brief Set the horizontal origin face of the camera.
     * 
     * @param xOrigin New horizontal origin enum.
     */
    inline void setXOrigin(H2DE_Face xOrigin) noexcept {
        data.xOrigin = xOrigin;
    }
    /**
     * @brief Set the vertical origin face of the camera.
     * 
     * @param yOrigin New vertical origin enum.
     */
    inline void setYOrigin(H2DE_Face yOrigin) noexcept {
        data.yOrigin = yOrigin;
    }
    /**
     * @brief Enable or disable rendering the grid on top.
     * 
     * @param state true to draw grid on top, false otherwise.
     */
    inline void setGridOnTop(bool state) noexcept {
        onTop = state;
    }

    /**
     * @brief Animate the camera translation to a new position over time.
     * 
     * This function smoothly interpolates the camera's translate value to the given target,
     * using the specified duration and easing function.
     * 
     * @param translate Target translation vector.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the camera's game width to a new value over time.
     * 
     * Smoothly interpolates the gameWidth parameter, allowing dynamic zoom effects.
     * 
     * @param width Target width.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setGameWidth(float width, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the interface width over time.
     * 
     * Allows smooth transition of the interface scaling width.
     * 
     * @param width Target interface width.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setInterfaceWidth(float width, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    friend class H2DE_Engine;
    friend class H2DE_Renderer;

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
};
