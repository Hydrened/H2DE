#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

/**
 * @file H2DE_bject.h
 * @brief Core class for game objects in the H2DE 2D engine.
 * 
 * This file defines the H2DE_Object class, which represents
 * an interactive game entity with transform, rendering,
 * hitboxes, and animation capabilities.
 * 
 * H2DE_Object handles spatial data (position, scale, rotation, pivot),
 * visibility, hitbox management (add, remove, transform, animate),
 * and interaction callbacks for collisions.
 * 
 * It's designed for flexibility and performance in a 2D game engine context,
 * supporting smooth animations with easing and layered rendering.
 */

#include <H2DE/engine/H2DE_engine.h>
#include <H2DE/engine/H2DE_error.h>
class H2DE_Engine;
class H2DE_Texture;
class H2DE_Sprite;
class H2DE_Color;

/**
 * @class H2DE_Object
 * @brief Represents a drawable and interactive object in the game world.
 * 
 * H2DE_Object manages:
 * - Transform properties: position, scale, rotation, pivot
 * - Visibility control (show/hide)
 * - Opacity and layering (render index)
 * - Hitboxes with customizable transforms, colors, collision groups, and callbacks
 * - Animation over time for all transform properties and hitboxes using easing functions
 * 
 * It acts as the base class for any game entity requiring
 * collision detection, animation, and rendering control.
 * 
 * @note Use addHitbox() and removeHitbox() to manage collision zones.
 * @note Animation methods return a timeline ID for control and chaining.
 * @note Hidden objects are excluded from rendering and updating.
 */
class H2DE_Object {
public:
    /**
     * @brief Add a hitbox to the object.
     * 
     * Inserts a new hitbox into the object's internal hitbox map, 
     * associated with a custom name. If a hitbox with the same name
     * already exists, it will be overwritten.
     * 
     * @param name Name used to identify the hitbox.
     * @param hitbox The hitbox to add.
     */
    void addHitbox(const std::string& name, const H2DE_Hitbox& hitbox);
    /**
     * @brief Remove a hitbox from the object.
     * 
     * Searches for a hitbox by its name and removes it if found.
     * 
     * @param name The name of the hitbox to remove.
     * @return true if the hitbox was found and removed, false otherwise.
     */
    bool removeHitbox(const std::string& name);

    /**
     * @brief Show the object (make it visible when rendering).
     */
    inline void show() noexcept {
        hidden = false; 
    }
    /**
     * @brief Hide the object (exclude it from rendering).
     */
    inline void hide() noexcept {
        hidden = true; 
    }

    /**
     * @brief Get the full object data.
     * 
     * Returns a copy of the internal H2DE_ObjectData struct, 
     * which contains transform, opacity, index, and other rendering data.
     * 
     * @return The current object data.
     */
    constexpr H2DE_ObjectData getObjectData() const noexcept {
        return objectData;
    }
    /**
     * @brief Get the object's full transform.
     * 
     * Includes position (translate), scale, rotation and pivot.
     * 
     * @return The current transform state of the object.
     */
    constexpr H2DE_Transform getTransform() const noexcept {
        return objectData.transform; 
    }
    /**
     * @brief Get the translation (position) of the object.
     * 
     * @return The current position (x, y) in pixels.
     */
    constexpr H2DE_Translate getTranslate() const noexcept {
        return objectData.transform.translate;
    }
    /**
     * @brief Get the scale of the object.
     * 
     * @return The current scale on x and y axes.
     */
    constexpr H2DE_Scale getScale() const noexcept {
        return objectData.transform.scale;
    }
    /**
     * @brief Get the rotation of the object in degrees.
     * 
     * @return The rotation angle in degrees.
     */
    constexpr float getRotation() const noexcept {
        return objectData.transform.rotation;
    }
    /**
     * @brief Get the pivot point of the object.
     * 
     * Used as the origin for rotation and scaling.
     * 
     * @return The pivot coordinates (usually between 0.0 and 1.0).
     */
    constexpr H2DE_Pivot getPivot() const noexcept {
        return objectData.transform.pivot;
    }
    /**
     * @brief Get the object's opacity.
     * 
     * @return An 8-bit value from 0 (fully transparent) to 255 (fully opaque).
     */
    constexpr uint8_t getOpacity() const noexcept {
        return objectData.opacity;
    }
    /**
     * @brief Check if the object's position is absolute.
     * 
     * If true, the object ignores camera movement.
     * 
     * @return true if the object is rendered in absolute (screen-space) coordinates.
     */
    constexpr bool isAbsolute() const noexcept {
        return objectData.absolute;
    }
    /**
     * @brief Get the rendering index of the object.
     * 
     * Higher index means higher priority in rendering (drawn above lower indices).
     * 
     * @return The render order index.
     */
    constexpr int getIndex() const noexcept {
        return objectData.index;
    }
    /**
     * @brief Check if the object is hidden.
     * 
     * @return true if hidden, false if visible.
     */
    constexpr bool isHidden() const noexcept {
        return hidden;
    }
    
    /**
     * @brief Get all hitboxes attached to the object.
     * 
     * Returns a copy of the unordered map containing all hitboxes, indexed by name.
     * 
     * @return A map of hitbox names to their respective H2DE_Hitbox data.
     */
    inline std::unordered_map<std::string, H2DE_Hitbox> getHitboxes() const noexcept {
        return hitboxes; 
    }
    /**
     * @brief Get a specific hitbox by name.
     * 
     * If the hitbox doesn't exist, an error is logged and the behavior is undefined (likely crash).
     * Make sure the hitbox exists before calling this method.
     * 
     * @param name The name of the hitbox to retrieve.
     * @return Reference to the corresponding H2DE_Hitbox.
     */
    const H2DE_Hitbox& getHitbox(const std::string& name) const;
    /**
     * @brief Get the world-space rectangle of a specific hitbox.
     * 
     * This applies the object's global translation to the local hitbox rect,
     * and returns the hitbox's absolute position and size in the level.
     * 
     * @param name The name of the hitbox.
     * @return A H2DE_LevelRect representing the world position and size of the hitbox.
     */
    const H2DE_LevelRect getHitboxWorldRect(const std::string& name) const;
    /**
     * @brief Checks whether a hitbox with the given name exists.
     * @param name Name of the hitbox to check.
     * @return true if the hitbox exists, false otherwise.
     */
    inline bool hasHitbox(const std::string& name) const {
        return (hitboxes.find(name) != hitboxes.end()); 
    }

    /**
     * @brief Set the object's position in world space.
     * 
     * @param translate The new position to apply.
     */
    void setTranslate(const H2DE_Translate& translate);
    /**
     * @brief Set the object's scale in both X and Y directions.
     * 
     * Scaling affects the visual size of the object and its hitboxes.
     * 
     * @param scale The new scale values.
     */
    void setScale(const H2DE_Scale& scale);
    /**
     * @brief Set the object's rotation in degrees.
     * 
     * Rotation is applied around the pivot point.
     * 
     * @param rotation New rotation angle, in degrees.
     */
    void setRotation(float rotation);
    /**
     * @brief Set the object's pivot point.
     * 
     * The pivot defines the origin for scaling and rotation.
     * 
     * @param pivot New pivot position, typically normalized between 0 and 1.
     */
    void setPivot(const H2DE_Pivot& pivot);
    /**
     * @brief Set the object's opacity.
     * 
     * @param opacity A value between 0 (fully transparent) and 255 (fully opaque).
     */
    void setOpacity(uint8_t opacity);
    /**
     * @brief Define whether the object uses absolute positioning.
     * 
     * If true, the object ignores camera transformations and remains fixed on screen.
     * 
     * @param absolute True to enable absolute mode, false to follow camera.
     */
    void setAbsolute(bool absolute);
    /**
     * @brief Set the rendering index of the object.
     * 
     * @param index Rendering index of the object.
     */
    void setIndex(int index);

    /**
     * @brief Animate the translation (position) of the object over time.
     * 
     * Moves the object smoothly from its current position to the target `translate` 
     * using the specified easing function over `duration` frames or time units.
     * 
     * @param translate Target position to reach.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setTranslate(const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the scale of the object over time.
     * 
     * Smoothly interpolates the object's scale from current to `scale` using easing.
     * 
     * @param scale Target scale factors for X and Y axes.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setScale(const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the rotation angle of the object over time.
     * 
     * Rotates smoothly from current angle to `rotation` degrees applying easing.
     * 
     * @param rotation Target rotation angle in degrees.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setRotation(float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the opacity (alpha) of the object over time.
     * 
     * Fades the object smoothly to the target `opacity` value.
     * 
     * @param opacity Target opacity value (0-255).
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setOpacity(uint8_t opacity, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    /**
     * @brief Set the translation (position) of a named hitbox instantly.
     * 
     * @param name The identifier of the hitbox.
     * @param translate New position relative to the object.
     */
    void setHitboxTranslate(const std::string& name, const H2DE_Translate& translate);
    /**
     * @brief Set the scale of a named hitbox instantly.
     * 
     * @param name The hitbox's name.
     * @param scale New scale factors for the hitbox.
     */
    void setHitboxScale(const std::string& name, const H2DE_Scale& scale);
    /**
     * @brief Set the rotation angle of a named hitbox instantly.
     * 
     * @param name The hitbox to rotate.
     * @param rotation Rotation angle in degrees.
     */
    void setHitboxRotation(const std::string& name, float rotation);
    /**
     * @brief Set the pivot point of a named hitbox.
     * 
     * @param name The hitbox to adjust.
     * @param pivot New pivot position.
     */
    void setHitboxPivot(const std::string& name, const H2DE_Pivot& pivot);
    /**
     * @brief Change the color used to render the hitbox (debug/visual).
     * 
     * @param name Hitbox identifier.
     * @param color New RGB color.
     */
    void setHitboxColor(const std::string& name, const H2DE_ColorRGB& color);
    /**
     * @brief Set the collision index (category) of a hitbox.
     * 
     * This helps identify collision groups or layers.
     * 
     * @param name Hitbox name.
     * @param collisionIndex Collision group/index integer.
     */
    void setHitboxCollisionIndex(const std::string& name, int collisionIndex);
    /**
     * @brief Assign a callback triggered when the hitbox collides.
     * 
     * @param name Hitbox name.
     * @param onCollide Function called on collision, with pointer to other object and collision face.
     */
    void setHitboxOnCollide(const std::string& name, const std::function<void(H2DE_Object*, H2DE_Face)>& onCollide);

    /**
     * @brief Animate the translation of a named hitbox.
     * 
     * Moves the hitbox smoothly to the target translate over duration using easing.
     * 
     * @param name Hitbox identifier.
     * @param translate Target position.
     * @param duration Duration of the animation.
     * @param easing Easing function.
     * @param completed Callback when done.
     * @param pauseSensitive If true, pauses animation on game pause.
     * @return Timeline ID for animation control.
     */
    H2DE_Timeline* setHitboxTranslate(const std::string& name, const H2DE_Translate& translate, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the scale of a named hitbox.
     * 
     * Smooth scaling from current to target scale.
     * 
     * @param name Hitbox ID.
     * @param scale Target scale.
     * @param duration Animation length.
     * @param easing Easing type.
     * @param completed Callback at animation end.
     * @param pauseSensitive Pause-aware flag.
     * @return Timeline ID.
     */
    H2DE_Timeline* setHitboxScale(const std::string& name, const H2DE_Scale& scale, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the rotation of a named hitbox.
     * 
     * Rotates the hitbox over time.
     * 
     * @param name Hitbox name.
     * @param rotation Target rotation in degrees.
     * @param duration Duration of animation.
     * @param easing Easing function.
     * @param completed Called when animation finishes.
     * @param pauseSensitive Pause-sensitive toggle.
     * @return Timeline ID.
     */
    H2DE_Timeline* setHitboxRotation(const std::string& name, float rotation, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the color change of a named hitbox.
     * 
     * Interpolates hitbox color smoothly.
     * 
     * @param name Hitbox ID.
     * @param color Target RGB color.
     * @param duration Duration.
     * @param easing Easing method.
     * @param completed Callback on completion.
     * @param pauseSensitive If true, pauses with game.
     * @return Timeline ID.
     */
    H2DE_Timeline* setHitboxColor(const std::string& name, const H2DE_ColorRGB& color, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    friend class H2DE_Engine;
    friend class H2DE_Renderer;
    friend class H2DE_Camera;
    friend class H2DE_Surface;
    friend class H2DE_Texture;
    friend class H2DE_Sprite;
    friend class H2DE_Color;
    friend class H2DE_ObjectManager;

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData objectData;

    std::vector<H2DE_Surface*> surfaceBuffers = {};
    std::vector<H2DE_Timeline*> timelinesBuffer = {};
    std::unordered_map<std::string, H2DE_Hitbox> hitboxes = {};
    float maxRadius;

    H2DE_Object(H2DE_Engine* engine, const H2DE_ObjectData& objectData) noexcept;
    virtual ~H2DE_Object();

    static void destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static void destroySurfaces(std::vector<H2DE_Surface*>& surfaces);
    void removeTimeline(H2DE_Timeline* timeline);

    virtual void update();
    void updateCollisions();
    void updateTimelineBuffer();

    virtual void refreshSurfaceBuffers() = 0;
    virtual void refreshMaxRadius() = 0;

    inline void addTimelineToTimelines(H2DE_Timeline* timeline) {
        timelinesBuffer.push_back(timeline);
    }

    template<typename H2DE_Surface_T>
    H2DE_Surface_T* addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        H2DE_Surface_T* surface = new H2DE_Surface_T(engine, this, surfaceData, specificData);
        surfaces[name] = surface;
        refreshMaxRadius();
        refreshSurfaceBuffers();
        return surface;
    }
    bool removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

    template<typename H2DE_Surface_T>
    static H2DE_Surface_T* getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
        auto it = surfaces.find(name);
        if (it == surfaces.end()) {
            H2DE_Error::throwError("Surface named \"" + name + "\" not found");
        }

        H2DE_Surface_T* castedSurface = dynamic_cast<H2DE_Surface_T*>(it->second);
        if (castedSurface == nullptr) {
            H2DE_Error::throwError("Can't cast surface \"" + name +  "\" to template type");
        }

        return castedSurface;
    }

    static const std::vector<H2DE_Surface*> getSortedSurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static const std::array<H2DE_Translate, 4> getCorners(const H2DE_Transform& transform);

    static inline bool hasSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name) {
        return (surfaces.find(name) != surfaces.end()); 
    }

    void rescaleSurfaceBuffers() noexcept;
    void rescaleHitboxes() noexcept;
    static void rescaleTransform(H2DE_Transform& transform, const H2DE_Scale& scale) noexcept;

    float getMaxHitboxRadius() const;
    float getMaxSurfaceRadius(const std::unordered_map<std::string, H2DE_Surface*>& surfaces) const;

private:
    bool hidden = false;
    bool isGrid = false;

    void stopTimelines();
};

#include <H2DE/objects/H2DE_bar_object.h>
#include <H2DE/objects/H2DE_basic_object.h>
#include <H2DE/objects/H2DE_button_object.h>
#include <H2DE/objects/H2DE_text_object.h>
#include <H2DE/objects/H2DE_timer_object.h>

#endif
