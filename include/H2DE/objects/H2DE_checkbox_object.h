#pragma once

/**
 * @file H2DE_checkbox_object.h
 * @brief Defines H2DE_CheckboxObject, a dual-surface object representing an interactive checkbox.
 * 
 * This file contains the H2DE_CheckboxObject class, which inherits from H2DE_DualSurfaceObject and
 * provides functionalities for interactive checkboxes in the UI.
 * 
 * The checkbox supports:
 * - Enable/disable state
 * - Checked/unchecked state with toggle functionality
 * - Callback handling for state changes
 * 
 * @note Surface layers (fill and background) are managed by the dual-surface system.
 */

#include <H2DE/objects/parents/H2DE_dual_surface_object.h>

/**
 * @class H2DE_CheckboxObject
 * @brief A dual-surface object representing an interactive UI checkbox.
 * 
 * H2DE_CheckboxObject extends H2DE_DualSurfaceObject by adding:
 * - Checked/unchecked state management
 * - Enable/disable functionality
 * - On-change callback for state updates
 * - Dual-surface support for fill and background visuals
 * 
 * This class is ideal for UI checkboxes, toggles, or any two-state interactive element.
 * 
 * @note Inherits dual-surface management functions from H2DE_DualSurfaceObject.
 */
class H2DE_CheckboxObject : public H2DE_DualSurfaceObject {
public:
    /**
     * @brief Enable the checkbox (makes it active).
     */
    inline void enable() { 
        _disabled = false;
    }
    /**
     * @brief Disable the checkbox (makes it inactive).
     */
    inline void disable() {
        _disabled = true;
    }
    
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
     * @brief Check if the checkbox is disabled.
     * @return true if disabled, false otherwise.
     */
    constexpr bool isDisabled() const noexcept {
        return _disabled; 
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
    H2DE_CheckboxObjectData _checkboxObjectData;

    H2DE_CheckboxObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_CheckboxObjectData& checkboxObjectData) : H2DE_DualSurfaceObject(engine, objectData), _checkboxObjectData(checkboxObjectData) {}
    ~H2DE_CheckboxObject() override = default;

    void _refreshSurfaceBuffers() override;
};
