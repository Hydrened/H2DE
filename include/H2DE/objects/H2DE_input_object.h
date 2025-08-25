#pragma once

/**
 * @file H2DE_input_object.h
 * @brief Defines the H2DE_InputObject class used for interactive text input fields.
 * 
 * This header declares the H2DE_InputObject class, which inherits from H2DE_Object and
 * provides a customizable 2D input field with support for text content, styling via H2DE_Text,
 * and input constraints such as maximum length and allowed input types.
 * 
 * Input fields can have callbacks assigned for events such as onInput, onFocus, onBlur,
 * and onSubmit, allowing for animations and logic triggered on user interaction.
 * After submission, the input automatically loses focus, triggering the onBlur callback if set.
 */

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

/**
 * @class H2DE_InputObject
 * @brief A flexible 2D input field supporting text input, styling, and interaction callbacks.
 * 
 * H2DE_InputObject extends H2DE_Object to implement interactive text input fields.
 * It manages text content, styling via H2DE_Text, and input constraints such as max length and allowed types.
 * 
 * Interaction events such as onInput, onFocus, onBlur, and onSubmit can be assigned
 * user callbacks that receive an H2DE_InputEventData struct with information about
 * the input state and typed characters. After submission, the input automatically
 * loses focus, triggering the onBlur callback if set.
 * 
 * Typical use includes UI text fields for menus, forms, chat boxes, or other
 * text input scenarios within a 2D game or application interface.
 */
class H2DE_InputObject : public H2DE_Object {
public:
    /**
     * @brief Add a surface to the object.
     * 
     * Adds a new surface (texture, sprite, or color) to the object.
     * The surface is stored and identified by a unique name.
     * 
     * @tparam H2DE_Surface_T The type of surface to add (e.g. Texture, Sprite, Color).
     * @param name The unique name to associate with the surface.
     * @param surfaceData Common parameters such as position, size, etc.
     * @param specificData Specific data for the surface type.
     * @return A pointer to the created surface, or nullptr if it failed.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* addSurface(const std::string& name, const H2DE_SurfaceData& surfaceData, const typename H2DE_Surface_T::H2DE_DataType& specificData) {
        return H2DE_Object::_addSurface<H2DE_Surface_T>(_surfaces, name, surfaceData, specificData);
    }

    /**
     * @brief Remove a surface from the object.
     * 
     * Removes a surface previously added by its unique name.
     * 
     * @param name The name of the surface to remove.
     * @return true if the surface was found and removed, false otherwise.
     */
    inline bool removeSurface(const std::string& name) {
        return H2DE_Object::_removeSurface(_surfaces, name);
    }

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
     * @brief Get all surfaces of the object.
     * 
     * Returns a map of all currently attached surfaces. Each entry maps the surface name
     * to its base `H2DE_Surface` pointer.
     * 
     * @return A map of surface names to their respective surface pointers.
     */
    inline std::unordered_map<std::string, H2DE_Surface*> getSurfaces() const noexcept {
        return _surfaces; 
    }
    /**
     * @brief Get a surface by name and cast it to a specific type.
     * 
     * Retrieves a surface of a specific type from the object. If the name is not found or
     * the type is incorrect, returns nullptr.
     * 
     * @tparam H2DE_Surface_T The expected surface type (Texture, Sprite, Color, etc.).
     * @param name The name of the surface to retrieve.
     * @return A pointer to the surface cast to the specified type, or nullptr.
     */
    template<typename H2DE_Surface_T>
    inline H2DE_Surface_T* getSurface(const std::string& name) const {
        return H2DE_Object::_getSurface<H2DE_Surface_T>(_surfaces, name); 
    }
    /**
     * @brief Checks whether a surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the surface exists, false otherwise.
     */
    inline bool hasSurface(const std::string& name) const {
        return H2DE_Object::_hasSurface(_surfaces, name);
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

    H2DE_TextObject* _textObject = H2DE_NULL_OBJECT;
    int _cursorPosition = -1;
    H2DE_Color* _cursor = H2DE_NULL_SURFACE;
    std::unordered_map<std::string, H2DE_Surface*> _surfaces = {};

    bool _disabled = false;

    H2DE_InputObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_InputObjectData& inputObjectData);
    ~H2DE_InputObject() override;

    void _initCursor();

    void _refreshTextObject();
    void _refreshCursor();
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;

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
