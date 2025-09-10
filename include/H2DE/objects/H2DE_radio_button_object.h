#pragma once

/**
 * @file H2DE_radio_button_object.h
 * @brief UI object representing a radio button with group-based selection.
 * 
 * This file defines the H2DE_RadioButtonObject class, which extends
 * H2DE_DualSurfaceObject and H2DE_ToggleableObject to implement
 * a functional radio button UI element.
 * 
 * The radio button is composed of:
 * - A background surface (the button outline or base)
 * - A foreground surface (the selection indicator or fill)
 * - An enabled/disabled state for controlling interaction
 * - A unique identifier (H2DE_RadioButtonID) for group logic
 * 
 * H2DE_RadioButtonObject is commonly used in menus, settings, and
 * UI forms where exclusive (one-of-many) options are needed.
 */

#include <H2DE/objects/parents/H2DE_dual_surface_object.h>
#include <H2DE/objects/parents/H2DE_toggleable_object.h>

/**
 * @class H2DE_RadioButtonObject
 * @brief Interactive UI radio button with group-based selection logic.
 * 
 * H2DE_RadioButtonObject inherits from:
 * - H2DE_DualSurfaceObject for rendering both the button background and the indicator
 * - H2DE_ToggleableObject for managing enabled/disabled interaction
 * 
 * Responsibilities:
 * - Render a radio button with a background and a foreground (selection indicator)
 * - Manage checked state based on group logic (only one active per group)
 * - Provide access to its unique identifier within the group
 * - Respect enable/disable state for interaction availability
 * 
 * Typical use cases:
 * - Option groups in settings menus (e.g., difficulty selection)
 * - Form inputs where only one option may be chosen
 * - Exclusive toggle systems in HUDs or menus
 * 
 * This class is a final object and can be directly instantiated
 * to provide radio button functionality in user interfaces.
 */
class H2DE_RadioButtonObject : public H2DE_DualSurfaceObject, public H2DE_ToggleableObject {
public:
    /**
     * @brief Check the radio button.
     * 
     * Sets the radio button state to checked and triggers the `onChange` callback if defined.
     */
    void check();
    /**
     * @brief Uncheck the radio button.
     * 
     * Sets the radio button state to unchecked and triggers the `onChange` callback if defined.
     */
    void uncheck();
    /**
     * @brief Toggle the radio button state.
     * 
     * Switches the radio button state from checked to unchecked or vice versa,
     * and triggers the `onChange` callback if defined.
     */
    void toggleCheck();

    /**
     * @brief Get the data specific to the radio button object.
     * 
     * Returns a copy of the internal `H2DE_RadioButtonObjectData` structure
     * which contains all configuration and state info related to the radio button.
     * 
     * @return A copy of the radio button's data.
     */
    inline H2DE_RadioButtonObjectData getRadioButtonData() const noexcept {
        return _radioButtonObjectData;
    }
    /**
     * @brief Get the unique identifier of the radio button.
     * 
     * Returns the `H2DE_RadioButtonID` associated with this radio button,
     * which distinguishes it from others within the same group.
     * 
     * @return The unique identifier of the radio button.
     */
    constexpr H2DE_RadioButtonID getID() const noexcept {
        return _radioButtonObjectData.id;
    }
    /**
     * @brief Check if the radio button is currently checked.
     * 
     * @return true if the radio button is checked, false otherwise.
     */
    constexpr bool isChecked() const noexcept {
        return _radioButtonObjectData.checked;
    }

    /**
     * @brief Set the callback function triggered when the radio button state changes.
     * 
     * The provided function will be called whenever the radio button is checked or unchecked.
     * It receives an `H2DE_RadioButtonEventData` containing information about the event.
     * 
     * @param onChange The callback function to handle radio button state changes.
     */
    inline void setOnChange(const std::function<void(H2DE_RadioButtonEventData)>& onChange) noexcept {
        _radioButtonObjectData.onChange = onChange;
    }

    using H2DE_DataType = H2DE_RadioButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    static constexpr bool _isParentObject = false;

    H2DE_RadioButtonObjectData _radioButtonObjectData;

    H2DE_RadioButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_RadioButtonObjectData& radioButtonObjectData) : H2DE_Object(engine, objectData), H2DE_DualSurfaceObject(engine, objectData), H2DE_ToggleableObject(engine, objectData), _radioButtonObjectData(radioButtonObjectData) {}
    ~H2DE_RadioButtonObject() override = default;

    void _refreshSurfaceBuffers() override;
    void _check();

    void _setCheck(bool checked);
};
