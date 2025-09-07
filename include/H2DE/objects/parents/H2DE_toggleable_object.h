#pragma once

/**
 * @file H2DE_toggleable_object.h
 * @brief Intermediate base class for objects with enable/disable state.
 * 
 * This file defines the H2DE_ToggleableObject class, which extends
 * H2DE_Object to provide toggleable behavior. It introduces a simple
 * enabled/disabled state that can be controlled at runtime, making it
 * useful for interactive UI components such as buttons, checkboxes,
 * or input fields.
 * 
 * H2DE_ToggleableObject simplifies state management by exposing methods
 * to enable, disable, or toggle the object, while also allowing checks
 * on whether the object is currently active or inactive.
 */

#include <H2DE/objects/parents/H2DE_object.h>

/**
 * @class H2DE_ToggleableObject
 * @brief Base class for objects that can be enabled or disabled.
 * 
 * H2DE_ToggleableObject provides a lightweight state system
 * for interactive UI elements. It allows derived objects to
 * respond differently when active or inactive (e.g., a button
 * that cannot be clicked when disabled).
 * 
 * Responsibilities:
 * - Store and manage a disabled/enabled flag
 * - Provide inline methods for enabling, disabling, or toggling state
 * - Expose constexpr checks (`isEnabled`, `isDisabled`) for fast queries
 * 
 * This class is intended as an intermediate layer for higher-level
 * interactive UI components, ensuring consistent state management
 * across the engine.
 */
class H2DE_ToggleableObject : virtual public H2DE_Object {
public:
    /**
     * @brief Enable the object.
     * 
     * Sets the internal disabled flag to false, marking the
     * object as active and interactive.
     */
    inline void enable() noexcept {
        _disabled = false;
    }
    /**
     * @brief Disable the object.
     * 
     * Sets the internal disabled flag to true, preventing
     * interaction or use of the object.
     */
    inline void disable() noexcept {
        _disabled = true;
    }
    /**
     * @brief Toggle the enable state of the object.
     * 
     * Switches the object state between enabled and disabled.
     * If the object is enabled, it becomes disabled, and vice versa.
     */
    inline void toggleEnable() noexcept {
        _disabled = !_disabled;
    }

    /**
     * @brief Check if the object is enabled.
     * @return true if the object is enabled, false otherwise.
     */
    constexpr bool isEnabled() noexcept {
        return !_disabled;
    }
    /**
     * @brief Check if the object is disabled.
     * @return true if the object is disabled, false otherwise.
     */
    constexpr bool isDisabled() noexcept {
        return _disabled;
    }

    friend class H2DE_Engine;

protected:
    H2DE_ToggleableObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData) : H2DE_Object(engine, objectData) {}
    ~H2DE_ToggleableObject() override = default;
};
