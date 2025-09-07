#pragma once

/**
 * @file H2DE_input_object.h
 * @brief UI object representing a text input field.
 * 
 * This file defines the H2DE_InputObject class, which combines
 * multiple intermediate classes to implement a functional input
 * box for user interaction.
 * 
 * An input field is composed of:
 * - A single surface for its visual background or frame
 * - A text surface for user-entered text or placeholder
 * - A toggleable state (enabled/disabled) to control availability
 * 
 * H2DE_InputObject is designed for menus, dialogs, or forms
 * where players need to enter names, codes, or other text data.
 */

#include <H2DE/objects/parents/H2DE_single_surface_object.h>
#include <H2DE/objects/parents/H2DE_text_surface_object.h>
#include <H2DE/objects/parents/H2DE_toggleable_object.h>

/**
 * @class H2DE_InputObject
 * @brief Interactive UI input field with text handling and toggle state.
 * 
 * H2DE_InputObject inherits from:
 * - H2DE_SingleSurfaceObject for rendering the input box surface
 * - H2DE_TextSurfaceObject for displaying input text or placeholder
 * - H2DE_ToggleableObject for enabling/disabling text entry
 * 
 * Responsibilities:
 * - Render an input box with background visuals
 * - Display and update text as the user types
 * - Support placeholder text when empty
 * - Respect enabled/disabled state for interaction control
 * 
 * Typical use cases:
 * - Name or nickname entry fields
 * - Password or code input
 * - Custom in-game forms or settings menus
 * 
 * This class is a final object and is intended to be directly
 * instantiated as part of UI systems requiring text entry.
 */
class H2DE_InputObject : public H2DE_SingleSurfaceObject, public H2DE_TextSurfaceObject, public H2DE_ToggleableObject {
public:
    /**
     * @brief Enable the input (makes it active).
     */
    inline void enable() { 
        _disabled = false;
    }
    /**
     * @brief Disable the input (makes it inactive).
     */
    inline void disable() {
        _disabled = true;
    }

    /**
     * @brief Handle a character input for the input object.
     * 
     * This function processes a single character typed by the user
     * and updates the input's text accordingly. It also triggers
     * the `onInput` callback if one is set.
     * 
     * @param c The character input by the user.
     */
    void input(unsigned char c);
    /**
     * @brief Set focus to the input object.
     * 
     * This function marks the input as focused, allowing it to receive
     * user input. It also triggers the `onFocus` callback if one is set.
     */
    void focus();
    /**
     * @brief Remove focus from the input object.
     * 
     * This function marks the input as no longer focused, preventing it
     * from receiving further user input. It triggers the `onBlur` callback
     * if one is set.
     */
    void blur();
    /**
     * @brief Submit the input's current text.
     * 
     * This function is called when the input is submitted (e.g., pressing Enter).
     * It triggers the `onSubmit` callback if one is set, passing the current
     * text and state. After submission, the input automatically loses focus,
     * which will also trigger the `onBlur` callback if one is set.
     */
    void submit();

    /**
     * @brief Get the data specific to the input object.
     * 
     * Returns a copy of the internal `H2DE_InputObjectData` structure
     * which contains all configuration and state info related to the input.
     * 
     * @return A copy of the input's data.
     */
    inline H2DE_InputObjectData getInputData() const noexcept {
        return _inputObjectData;
    }
    /**
     * @brief Get the type of input accepted by this input object.
     * 
     * Returns the H2DE_InputType specifying whether the input field accepts
     * text, numbers, or any type of input.
     * 
     * @return The input type of the input object.
     */
    constexpr H2DE_InputType getType() const noexcept {
        return _inputObjectData.type;
    }
    /**
     * @brief Get the text content of the input.
     * 
     * @return The current text of the input.
     */
    inline std::string getText() const noexcept {
        return _inputObjectData.text.text;
    }
    /**
     * @brief Get the maximum number of characters allowed in the input.
     * 
     * Returns the configured limit on the input's text length.
     * 
     * @return Maximum number of characters allowed.
     */
    constexpr uint16_t getMaxLength() const noexcept {
        return _inputObjectData.maxLength;
    }
    /**
     * @brief Check if the input is disabled.
     * @return true if disabled, false otherwise.
     */
    constexpr bool isDisabled() const noexcept {
        return _disabled; 
    }
    /**
     * @brief Get the text object associated with the input.
     * 
     * Returns a pointer to the `H2DE_TextObject` used for rendering
     * any text label or content inside the input.
     * 
     * @return Pointer to the input's text object, or nullptr if none.
     */
    inline H2DE_TextObject* getTextObject() const noexcept {
        return _textObject; 
    }

    /**
     * @brief Set the text content of the input.
     * 
     * Updates the internal text object with the provided string.
     * 
     * @param text The new text to display on the input.
     */
    void setText(const std::string& text);
    /**
     * @brief Set the callback to be called when a character is input.
     * 
     * The provided function will be triggered each time the user types
     * a character in the input field. The callback receives an
     * `H2DE_InputEventData` containing information about the input state.
     * 
     * @param onInput The callback function to handle character input events.
     */
    inline void setOnInput(const std::function<void(H2DE_InputEventData)>& onInput) noexcept {
        _inputObjectData.onInput = onInput;
    }
    /**
     * @brief Set the callback to be called when the input gains focus.
     * 
     * The provided function will be triggered when the input field becomes
     * focused. The callback receives an `H2DE_InputEventData` containing
     * information about the input state.
     * 
     * @param onFocus The callback function to handle focus events.
     */
    inline void setOnFocus(const std::function<void(H2DE_InputEventData)>& onFocus) noexcept {
        _inputObjectData.onFocus = onFocus;
    }
    /**
     * @brief Set the callback to be called when the input loses focus.
     * 
     * The provided function will be triggered when the input field loses focus,
     * either manually or automatically (e.g., after `submit`). The callback receives
     * an `H2DE_InputEventData` with the current input state.
     * 
     * @param onBlur The callback function to handle blur events.
     */
    inline void setOnBlur(const std::function<void(H2DE_InputEventData)>& onBlur) noexcept {
        _inputObjectData.onBlur = onBlur;
    }
    /**
     * @brief Set the callback to be called when the input is submitted.
     * 
     * The provided function will be triggered when the input is submitted (e.g., pressing Enter).
     * After submission, the input automatically loses focus, which also triggers
     * the `onBlur` callback if set. The callback receives an `H2DE_InputEventData`
     * containing the current input state.
     * 
     * @param onSubmit The callback function to handle submission events.
     */
    inline void setOnSubmit(const std::function<void(H2DE_InputEventData)>& onSubmit) noexcept {
        _inputObjectData.onSubmit = onSubmit;
    }

    using H2DE_DataType = H2DE_InputObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_InputObjectData _inputObjectData;

    int _cursorPosition = -1;
    H2DE_Color* _cursor = H2DE_NULL_SURFACE;

    H2DE_InputObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_InputObjectData& inputObjectData);
    ~H2DE_InputObject() override = default;

    void _initCursor();

    void _refreshSurfaceBuffers() override;
    void _refreshCursor();

    inline bool _isInputValid(unsigned char c) const {
        return (std::isdigit(c))
            ? ((_inputObjectData.type & H2DE_INPUT_TYPE_NUMBER) != 0)
            : ((_inputObjectData.type & H2DE_INPUT_TYPE_TEXT) != 0);
    }

    constexpr bool _isFull() const {
        return (_inputObjectData.text.text.length() >= _inputObjectData.maxLength);
    }

    void _setCursorPosition(int position);
};
