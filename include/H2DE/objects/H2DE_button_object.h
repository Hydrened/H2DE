#pragma once

/**
 * @file H2DE_button_object.h
 * @brief UI object representing a clickable button with text and state.
 * 
 * This file defines the H2DE_ButtonObject class, which combines
 * multiple intermediate object types to provide a functional UI button.
 * 
 * A button is composed of:
 * - A single surface for its visual background
 * - An optional text label rendered on top
 * - A toggleable state (enabled/disabled) for interaction control
 * 
 * H2DE_ButtonObject is a fundamental interactive component in UI systems,
 * designed for menus, dialogs, and in-game HUDs.
 */

#include <H2DE/objects/parents/H2DE_single_surface_object.h>
#include <H2DE/objects/parents/H2DE_text_surface_object.h>
#include <H2DE/objects/parents/H2DE_toggleable_object.h>

/**
 * @class H2DE_ButtonObject
 * @brief Interactive UI button with text and enable/disable state.
 * 
 * H2DE_ButtonObject inherits from:
 * - H2DE_SingleSurfaceObject for rendering the button surface
 * - H2DE_TextSurfaceObject for managing an optional text label
 * - H2DE_ToggleableObject for enabling/disabling interaction
 * 
 * Responsibilities:
 * - Display a visual surface for the button background
 * - Optionally render a text label centered or aligned on the button
 * - Provide enable/disable state control for interaction availability
 * 
 * Typical use cases:
 * - Menu buttons
 * - Confirm/Cancel UI elements
 * - Custom interactive controls
 * 
 * This class is a final object and is meant to be directly instantiated
 * for building user interfaces.
 */
class H2DE_ButtonObject : public H2DE_SingleSurfaceObject, public H2DE_TextSurfaceObject, public H2DE_ToggleableObject {
public:
    /**
     * @brief Calls the mouse down event handler assigned to the button.
     * 
     * This function triggers the callback or event linked to the mouse button press on the button.
     */
    void mouseDown();
    /**
     * @brief Calls the mouse up event handler assigned to the button.
     * 
     * This function triggers the callback or event linked to the mouse button release on the button.
     */
    void mouseUp();
    /**
     * @brief Calls the mouse hover event handler assigned to the button.
     * 
     * This function triggers the callback or event linked to the mouse cursor hovering over the button.
     */
    void mouseHover();
    /**
     * @brief Calls the mouse blur event handler assigned to the button.
     * 
     * This function triggers the callback or event linked to the mouse cursor leaving the button area.
     */
    void mouseBlur();

    /**
     * @brief Get the data specific to the button object.
     * 
     * Returns a copy of the internal `H2DE_ButtonObjectData` structure
     * which contains all configuration and state info related to the button.
     * 
     * @return A copy of the button's data.
     */
    inline H2DE_ButtonObjectData getButtonData() const noexcept {
        return _buttonObjectData;
    }
    /**
     * @brief Get the text content of the button.
     * 
     * @return The current text of the button.
     */
    inline std::string getText() const noexcept {
        return _buttonObjectData.text.text;
    }
    /**
     * @brief Get the mouse button(s) configured to trigger this button.
     * 
     * Returns the `H2DE_MouseButton` value that defines which mouse button(s)
     * can interact with this button (e.g., LEFT, RIGHT, MIDDLE, or combinations).
     * 
     * @return The configured mouse button(s) for this button.
     */
    constexpr H2DE_MouseButton getMouseButton() const noexcept {
        return _buttonObjectData.mouseButton;
    }
    /**
     * @brief Get the cursor displayed when hovering over this button.
     * 
     * Returns the `H2DE_Cursor` value that defines the cursor shape
     * shown while the mouse is hovering over the button 
     * (e.g., ARROW, HAND, or other custom cursors).
     * 
     * @return The configured cursor for this button.
     */
    constexpr H2DE_Cursor getCursor() const noexcept {
        return _buttonObjectData.cursor;
    }

    /**
     * @brief Set the text content of the button.
     * 
     * Updates the internal text object with the provided string.
     * 
     * @param text The new text to display on the button.
     */
    void setText(const std::string& text);
    /**
     * @brief Set the mouse button(s) that can trigger this button.
     * 
     * Allows you to define which mouse button(s) should be detected
     * for interaction with this button (e.g., LEFT, RIGHT, MIDDLE, or combined).
     * 
     * @param mouseButton The mouse button(s) to use for triggering the button.
     */
    inline void setMouseButton(H2DE_MouseButton mouseButton) noexcept {
        _buttonObjectData.mouseButton = mouseButton;
    }
    /**
     * @brief Set the callback function triggered when the button is pressed down.
     * 
     * This callback is called when the mouse button goes down on the button.
     * 
     * @param onMouseDown The function to call on mouse down event.
     */
    inline void setMouseDown(const std::function<void(H2DE_ButtonEventData)>& onMouseDown) noexcept {
        _buttonObjectData.onMouseDown = onMouseDown;
    }
    /**
     * @brief Set the callback function triggered when the mouse button is released.
     * 
     * This callback is called when the mouse button is released on the button.
     * 
     * @param onMouseUp The function to call on mouse up event.
     */
    inline void setMouseUp(const std::function<void(H2DE_ButtonEventData)>& onMouseUp) noexcept {
        _buttonObjectData.onMouseUp = onMouseUp;
    }
    /**
     * @brief Set the callback function triggered when the mouse starts hovering over the button.
     * 
     * This callback is called when the mouse cursor moves over the button.
     * 
     * @param onHover The function to call on mouse hover event.
     */
    inline void setMouseHover(const std::function<void(H2DE_ButtonEventData)>& onHover) noexcept {
        _buttonObjectData.onHover = onHover;
    }
    /**
     * @brief Set the callback function triggered when the mouse stops hovering over the button.
     * 
     * This callback is called when the mouse cursor leaves the button area.
     * 
     * @param onBlur The function to call on mouse blur event.
     */
    inline void setMouseBlur(const std::function<void(H2DE_ButtonEventData)>& onBlur) noexcept {
        _buttonObjectData.onBlur = onBlur;
    }
    /**
     * @brief Set the cursor displayed when hovering over the button.
     * 
     * Defines the `H2DE_Cursor` value to be shown when the mouse is
     * hovering over the button (e.g., ARROW, HAND, or custom cursor).
     * 
     * @param cursor The cursor to display while hovering over the button.
     */
    inline void setCursor(H2DE_Cursor cursor) noexcept {
        _buttonObjectData.cursor = cursor;
    }

    using H2DE_DataType = H2DE_ButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_ButtonObjectData _buttonObjectData;

    H2DE_ButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    ~H2DE_ButtonObject() override = default;

    void _refreshSurfaceBuffers() override;
};
