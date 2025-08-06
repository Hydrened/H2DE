#pragma once

/**
 * @file H2DE_bar_object.h
 * @brief Defines the H2DE_BarObject class used for creating customizable bar elements (e.g. HP bars, progress bars).
 * 
 * This header declares the H2DE_BarObject class, which inherits from H2DE_Object and provides a flexible 2D bar component 
 * capable of displaying dynamic values using surfaces for the background and foreground. It supports both direct and animated updates.
 * 
 * Surfaces can be added either to the fill or to the background (e.g. static panel) with fine-grained control.
 * 
 * The bar’s value, minimum, and maximum can be modified directly or animated with timeline support and easing functions.
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @class H2DE_BarObject
 * @brief Represents a 2D UI bar component with background and foreground visuals.
 * 
 * A `H2DE_BarObject` is a flexible element used to visually represent a value between a minimum and maximum.
 * It can be used for health bars, loading bars, mana bars, experience bars, etc.
 * 
 * This object supports:
 * - Custom fill and background surfaces (textures, sprites, or colors).
 * - Real-time or animated value updates (with easing support).
 * - Access to the underlying bar data and surface management.
 * 
 * It inherits from `H2DE_Object` and uses the standard H2DE surface system.
 */
class H2DE_BarObject : public H2DE_Object {
public:
    /**
     * @brief Add a surface to the fill of the bar.
     * 
     * This function adds a surface (texture, sprite, or color) in fill of the bar,
     * which visually represents the actual filling part of the bar.
     * 
     * @tparam H2DE_Surface_T The type of the surface (e.g. Texture, Sprite, Color).
     * @param name The unique identifier for the surface.
     * @param surfaceData Common surface parameters (position, size, etc.).
     * @param specificData Additional data specific to the surface type.
     * @return A pointer to the created surface, or nullptr on failure.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToFill(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(fillSurfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a fill surface from the bar.
     * 
     * This removes a surface that was previously added in fill of the bar
     * using its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was removed successfully, false otherwise.
     */
    inline bool removeSurfaceFromFill(const std::string& name) {
        return H2DE_Object::removeSurface(fillSurfaces, name);
    }
    
    /**
     * @brief Add a surface to the background of the bar.
     * 
     * This adds a decorative or background surface behind the bar's fill.
     * 
     * @tparam H2DE_Surface_T The type of the surface (e.g. Texture, Sprite, Color).
     * @param name The unique identifier for the surface.
     * @param surfaceData Common surface parameters (position, size, etc.).
     * @param specificData Additional data specific to the surface type.
     * @return A pointer to the created surface, or nullptr on failure.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurfaceToBackground(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::addSurface<H2DE_Surface_T>(backgroundSurfaces, name, surfaceData, specificData);
    }
    /**
     * @brief Remove a background surface from the bar.
     * 
     * This removes a surface that was added behind the bar using its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was removed successfully, false otherwise.
     */
    inline bool removeSurfaceFromBackground(const std::string& name) {
        return H2DE_Object::removeSurface(backgroundSurfaces, name);
    }

    /**
     * @brief Get the internal data of the bar.
     * 
     * Returns a copy of the entire data structure that defines the bar's
     * configuration, including its min, max, and current value.
     * 
     * @return The current H2DE_BarObjectData structure.
     */
    constexpr H2DE_BarObjectData getBarData() const noexcept {
        return barObjectData; 
}
    /**
     * @brief Get the minimum value of the bar.
     * 
     * This is the lowest possible value the bar can represent.
     * 
     * @return The minimum float value of the bar.
     */
    constexpr float getMin() const noexcept {
        return barObjectData.min;
    }
    /**
     * @brief Get the maximum value of the bar.
     * 
     * This is the highest possible value the bar can represent.
     * 
     * @return The maximum float value of the bar.
     */
    constexpr float getMax() const noexcept {
        return barObjectData.max;
    }
    /**
     * @brief Get the current value of the bar.
     * 
     * Represents the current fill or progress level of the bar
     * between its minimum and maximum values.
     * 
     * @return The current float value of the bar.
     */
    constexpr float getValue() const noexcept {
        return barObjectData.value;
    }

    /**
     * @brief Get all fill surfaces of the bar.
     * 
     * Returns a map containing all surfaces rendered in fill of the bar background.
     * These typically include the foreground bar fill or decorative elements.
     * 
     * @return A map of surface names to H2DE_Surface pointers.
     */
    inline std::unordered_map<std::string, H2DE_Surface*> getFillSurfaces() const noexcept {
        return fillSurfaces;
    }
    /**
     * @brief Get all background surfaces of the bar.
     * 
     * Returns a map containing all surfaces rendered behind the bar foreground.
     * These typically include background textures or colored panels.
     * 
     * @return A map of surface names to H2DE_Surface pointers.
     */
    inline std::unordered_map<std::string, H2DE_Surface*> getBackgroundSurfaces() const noexcept {
        return backgroundSurfaces;
    }
    /**
     * @brief Get a fill surface by name and type.
     * 
     * Retrieves a specific surface from the fill layer, cast to the requested surface type.
     * If the name does not exist or the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface, or nullptr if not found or mismatched.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getFillSurface(const std::string& name) const {
        return H2DE_Object::getSurface<H2DE_Surface_T>(fillSurfaces, name);
    }
    /**
     * @brief Get a background surface by name and type.
     * 
     * Retrieves a specific surface from the background layer, cast to the requested surface type.
     * If the name does not exist or the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface, or nullptr if not found or mismatched.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getBackgroundSurface(const std::string& name) const {
        return H2DE_Object::getSurface<H2DE_Surface_T>(backgroundSurfaces, name);
    }
    /**
     * @brief Checks whether a fill surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the fill surface exists, false otherwise.
     */
    inline bool hasFillSurface(const std::string& name) const {
        return H2DE_Object::hasSurface(fillSurfaces, name); 
}
    /**
     * @brief Checks whether a background surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the background surface exists, false otherwise.
     */
    inline bool hasBackgroundSurface(const std::string& name) const {
        return H2DE_Object::hasSurface(backgroundSurfaces, name);
    }

    /**
     * @brief Set the minimum value of the bar.
     * 
     * Updates the minimum limit that the bar value can reach.
     * 
     * @param min The new minimum value.
     */
    void setMin(float min);
    /**
     * @brief Set the maximum value of the bar.
     * 
     * Updates the maximum limit that the bar value can reach.
     * 
     * @param max The new maximum value.
     */
    void setMax(float max);
    /**
     * @brief Set the current value of the bar.
     * 
     * Directly updates the current value of the bar. This does not interpolate the change.
     * 
     * @param value The new value to assign.
     */
    void setValue(float value);

    /**
     * @brief Animate the minimum value of the bar.
     * 
     * Starts a timeline animation that changes the minimum value over time.
     * 
     * @param min The target minimum value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setMin(float min, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the maximum value of the bar.
     * 
     * Starts a timeline animation that changes the maximum value over time.
     * 
     * @param max The target maximum value.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setMax(float max, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);
    /**
     * @brief Animate the current value of the bar.
     * 
     * Starts a timeline animation that changes the bar value over time.
     * 
     * @param value The target value to reach.
     * @param duration Duration of the animation.
     * @param easing Easing function to apply for interpolation.
     * @param completed Callback function called once the animation finishes.
     * @param pauseSensitive If true, animation pauses when the game is paused.
     * @return Timeline controlling this animation.
     */
    H2DE_Timeline* setValue(float value, uint32_t duration, H2DE_Easing easing = H2DE_EASING_LINEAR, const std::function<void()>& completed = nullptr, bool pauseSensitive = true);

    using H2DE_DataType = H2DE_BarObjectData;

    friend class H2DE_Engine;
    friend class H2DE_Renderer;

private:
    H2DE_BarObjectData barObjectData;

    std::unordered_map<std::string, H2DE_Surface*> fillSurfaces = {};
    std::unordered_map<std::string, H2DE_Surface*> backgroundSurfaces = {};

    H2DE_BarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);
    ~H2DE_BarObject() override;

    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;

    constexpr float getFillBlend() const {
        return H2DE::clamp((getValue() - getMin()) / (getMax() - getMin()), 0.0f, 1.0f);
    }
    bool isSurfaceFill(H2DE_Surface* surface) const;
};
