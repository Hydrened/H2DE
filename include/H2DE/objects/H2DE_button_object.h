#pragma once

/**
 * @file H2DE_button_object.h
 * @brief Defines the H2DE_ButtonObject class used for interactive button elements.
 * 
 * This header declares the H2DE_ButtonObject class, which inherits from H2DE_Object and
 * provides a customizable 2D button with support for multiple surfaces, text labels,
 * and user interaction events such as mouse down, up, hover, and blur.
 * 
 * Buttons can have callbacks assigned for these events, allowing for animations and
 * logic triggered on user input. They can be enabled or disabled, and optionally respond
 * to the engine's pause state.
 */

#include <H2DE/objects/H2DE_object.h>
class H2DE_TextObject;

/**
 * @class H2DE_ButtonObject
 * @brief A flexible 2D button object supporting surfaces, text, and interaction callbacks.
 * 
 * H2DE_ButtonObject extends H2DE_Object to implement clickable and interactive buttons.
 * It manages a collection of surfaces (textures, sprites, colors) identified by unique names,
 * and supports text rendering inside the button via an optional H2DE_TextObject.
 * 
 * Interaction events such as mouseDown, mouseUp, mouseHover, and mouseBlur can be assigned
 * user callbacks that receive the button instance and a timeline ID for animation control.
 * The button can be enabled or disabled, and configured to be sensitive or insensitive to
 * the engine's pause state.
 * 
 * Typical use includes UI buttons for menus, HUD, or gameplay controls with visual feedback.
 */
class H2DE_ButtonObject : public H2DE_Object {
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
     * @brief Enable the button (makes it active).
     */
    inline void enable() { 
        _disabled = false;
    }
    /**
     * @brief Disable the button (makes it inactive).
     */
    inline void disable() {
        _disabled = true;
    }

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
     * @brief Check if the button is sensitive to the game's pause state.
     * 
     * Returns true if the button reacts differently (or is disabled) when the game is paused,
     * false otherwise.
     * 
     * @return true if pause sensitive, false if not.
     */
    constexpr bool isPauseSensitive() const noexcept {
        return _buttonObjectData.pauseSensitive; 
    }
    /**
     * @brief Check if the button is disabled.
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
     * @brief Get the text object associated with the button.
     * 
     * Returns a pointer to the `H2DE_TextObject` used for rendering
     * any text label or content inside the button.
     * 
     * @return Pointer to the button's text object, or nullptr if none.
     */
    inline H2DE_TextObject* getTextObject() const noexcept {
        return _textObject; 
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
    /**
     * @brief Set whether the button's events are sensitive to the engine's pause state.
     * 
     * When set to true, the button callbacks will not trigger if the engine is paused.
     * 
     * @param pauseSensitive True to make the button sensitive to pause, false otherwise.
     */
    inline void setPauseSensitive(bool pauseSensitive) noexcept { 
        _buttonObjectData.pauseSensitive = pauseSensitive; 
    }

    using H2DE_DataType = H2DE_ButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_ButtonObjectData _buttonObjectData;

    H2DE_TextObject* _textObject = H2DE_NULL_OBJECT;
    std::unordered_map<std::string, H2DE_Surface*> _surfaces = {};
    H2DE_ButtonEventData _eventData;

    bool _disabled = false;

    H2DE_ButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    ~H2DE_ButtonObject() override;

    void _refreshTextObject();
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;
};
