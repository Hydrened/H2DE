#pragma once

/**
 * @file H2DE_checkbox_object.h
 * @brief UI object representing a checkbox with toggleable state.
 * 
 * This file defines the H2DE_CheckboxObject class, which extends
 * H2DE_DualSurfaceObject and H2DE_ToggleableObject to implement
 * a functional checkbox UI element.
 * 
 * The checkbox is composed of:
 * - A background surface (the box outline or base)
 * - A foreground surface (the checkmark or fill)
 * - An enabled/disabled state for controlling interaction
 * 
 * H2DE_CheckboxObject is commonly used in menus, settings, and
 * UI forms where binary (on/off) options are needed.
 */

#include <H2DE/objects/parents/H2DE_dual_surface_object.h>
#include <H2DE/objects/parents/H2DE_toggleable_object.h>

/**
 * @class H2DE_CheckboxObject
 * @brief Interactive UI checkbox with dual surfaces and toggle state.
 * 
 * H2DE_CheckboxObject inherits from:
 * - H2DE_DualSurfaceObject for rendering both the box and the checkmark
 * - H2DE_ToggleableObject for managing enabled/disabled interaction
 * 
 * Responsibilities:
 * - Render a checkbox with a background and a foreground (checkmark)
 * - Allow toggling between checked and unchecked states
 * - Respect enable/disable state for interaction availability
 * 
 * Typical use cases:
 * - Settings menus with binary options
 * - Form inputs within game UIs
 * - Interactive toggles in HUDs
 * 
 * This class is a final object and can be directly instantiated
 * to provide checkbox functionality in user interfaces.
 */
class H2DE_CheckboxObject : public H2DE_DualSurfaceObject, public H2DE_ToggleableObject {
public:
    /**
     * @brief Check the checkbox.
     * 
     * Sets the checkbox state to checked and triggers the `onChange` callback if defined.
     */
    void check();
    /**
     * @brief Uncheck the checkbox.
     * 
     * Sets the checkbox state to unchecked and triggers the `onChange` callback if defined.
     */
    void uncheck();
    /**
     * @brief Toggle the checkbox state.
     * 
     * Switches the checkbox state from checked to unchecked or vice versa,
     * and triggers the `onChange` callback if defined.
     */
    void toggleCheck();

    /**
     * @brief Get the data specific to the checkbox object.
     * 
     * Returns a copy of the internal `H2DE_CheckboxObjectData` structure
     * which contains all configuration and state info related to the checkbox.
     * 
     * @return A copy of the checkbox's data.
     */
    inline H2DE_CheckboxObjectData getCheckboxData() const noexcept {
        return _checkboxObjectData;
    }
    /**
     * @brief Check if the checkbox is currently checked.
     * 
     * @return true if the checkbox is checked, false otherwise.
     */
    constexpr bool isChecked() const noexcept {
        return _checkboxObjectData.checked;
    }

    /**
     * @brief Set the callback function triggered when the checkbox state changes.
     * 
     * The provided function will be called whenever the checkbox is checked or unchecked.
     * It receives an `H2DE_CheckboxEventData` containing information about the event.
     * 
     * @param onChange The callback function to handle checkbox state changes.
     */
    inline void setOnChange(const std::function<void(H2DE_CheckboxEventData)>& onChange) noexcept {
        _checkboxObjectData.onChange = onChange;
    }

    using H2DE_DataType = H2DE_CheckboxObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    static constexpr bool _isParentObject = false;
    
    H2DE_CheckboxObjectData _checkboxObjectData;

    H2DE_CheckboxObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_CheckboxObjectData& checkboxObjectData) : H2DE_Object(engine, objectData), H2DE_DualSurfaceObject(engine, objectData), H2DE_ToggleableObject(engine, objectData), _checkboxObjectData(checkboxObjectData) {}
    ~H2DE_CheckboxObject() override = default;

    void _refreshSurfaceBuffers() override;

    void _setCheck(bool checked);
};
