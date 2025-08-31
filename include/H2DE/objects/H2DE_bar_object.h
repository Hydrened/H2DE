#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_BarObject : public H2DE_DualSurfaceObject {
public:
    /**
     * @brief Get the internal data of the bar.
     * 
     * Returns a copy of the entire data structure that defines the bar's
     * configuration, including its min, max, and current value.
     * 
     * @return The current H2DE_BarObjectData structure.
     */
    constexpr H2DE_BarObjectData getBarData() const noexcept {
        return _barObjectData; 
    }
    /**
     * @brief Get the minimum value of the bar.
     * 
     * This is the lowest possible value the bar can represent.
     * 
     * @return The minimum float value of the bar.
     */
    constexpr float getMin() const noexcept {
        return _barObjectData.min;
    }
    /**
     * @brief Get the maximum value of the bar.
     * 
     * This is the highest possible value the bar can represent.
     * 
     * @return The maximum float value of the bar.
     */
    constexpr float getMax() const noexcept {
        return _barObjectData.max;
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
        return _barObjectData.value;
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
    H2DE_BarObjectData _barObjectData;

    H2DE_BarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData) : H2DE_DualSurfaceObject(engine, objectData), _barObjectData(barObjectData) {}
    ~H2DE_BarObject() override = default;

    constexpr float _getFillBlend() const {
        return H2DE::clamp((getValue() - getMin()) / (getMax() - getMin()), 0.0f, 1.0f);
    }
    bool _isSurfaceFill(H2DE_Surface* surface) const;
};
