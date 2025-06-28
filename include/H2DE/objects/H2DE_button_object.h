#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

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
        return H2DE_Object::addSurface<H2DE_Surface_T>(surfaces, name, surfaceData, specificData);
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
        return H2DE_Object::removeSurface(surfaces, name);
    }

    /**
     * @brief Enable the button (makes it active).
     */
    inline void enable() { 
        disabled = false;
    }
    /**
     * @brief Disable the button (makes it inactive).
     */
    inline void disable() {
        disabled = true;
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
     * @brief Stop the button's timeline animation.
     * 
     * This function stops the currently running timeline associated with the button.
     * The timeline ID is provided in the button events to allow animation control
     * when interacting with the button (e.g. press, hover).
     * 
     * @return true if the timeline was successfully stopped, false otherwise.
     */
    bool stopTimeline();

    /**
     * @brief Get the data specific to the button object.
     * 
     * Returns a copy of the internal `H2DE_ButtonObjectData` structure
     * which contains all configuration and state info related to the button.
     * 
     * @return A copy of the button's data.
     */
    inline H2DE_ButtonObjectData getButtonData() const {
        return buttonObjectData;
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
        return buttonObjectData.pauseSensitive; 
    }
    /**
     * @brief Check if the button is disabled.
     * @return true if disabled, false otherwise.
     */
    constexpr bool isDisabled() const noexcept {
        return disabled; 
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
        return surfaces; 
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
        return H2DE_Object::getSurface<H2DE_Surface_T>(surfaces, name); 
    }
    /**
     * @brief Checks whether a surface with the given name exists.
     * @param name Name of the surface to check.
     * @return true if the surface exists, false otherwise.
     */
    inline bool hasSurface(const std::string& name) const {
        return H2DE_Object::hasSurface(surfaces, name);
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
        return textObject; 
    }

    /**
     * @brief Set the callback function triggered when the button is pressed down.
     * 
     * The function signature should be: void(H2DE_ButtonObject*, H2DE_TimelineID&).
     * This callback is called when the mouse button goes down on the button.
     * 
     * @param onMouseDown The function to call on mouse down event.
     */
    inline void setMouseDown(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onMouseDown) noexcept {
        buttonObjectData.onMouseDown = onMouseDown;
    }
    /**
     * @brief Set the callback function triggered when the mouse button is released.
     * 
     * This callback is called when the mouse button is released on the button.
     * 
     * @param onMouseUp The function to call on mouse up event.
     */
    inline void setMouseUp(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onMouseUp) noexcept {
        buttonObjectData.onMouseUp = onMouseUp;
    }
    /**
     * @brief Set the callback function triggered when the mouse starts hovering over the button.
     * 
     * This callback is called when the mouse cursor moves over the button.
     * 
     * @param onHover The function to call on mouse hover event.
     */
    inline void setMouseHover(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onHover) noexcept {
        buttonObjectData.onHover = onHover;
    }
    /**
     * @brief Set the callback function triggered when the mouse stops hovering over the button.
     * 
     * This callback is called when the mouse cursor leaves the button area.
     * 
     * @param onBlur The function to call on mouse blur event.
     */
    inline void setMouseBlur(const std::function<void(H2DE_ButtonObject*, H2DE_TimelineID&)>& onBlur) noexcept {
        buttonObjectData.onBlur = onBlur;
    }
    /**
     * @brief Set whether the button's events are sensitive to the engine's pause state.
     * 
     * When set to true, the button callbacks will not trigger if the engine is paused.
     * 
     * @param pauseSensitive True to make the button sensitive to pause, false otherwise.
     */
    inline void setPauseSensitive(bool pauseSensitive) noexcept { 
        buttonObjectData.pauseSensitive = pauseSensitive; 
    }

    using H2DE_DataType = H2DE_ButtonObjectData;

    friend class H2DE_Engine;
    friend class H2DE_ObjectManager;

private:
    H2DE_ButtonObjectData buttonObjectData;

    H2DE_TextObject* textObject = nullptr;
    std::unordered_map<std::string, H2DE_Surface*> surfaces = {};

    bool disabled = false;
    H2DE_TimelineID currentTimelineID = H2DE_INVALID_TIMELINE_ID;

    H2DE_ButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    ~H2DE_ButtonObject() override;

    void refreshTextObject();
    void refreshSurfaceBuffers() override;
    void refreshMaxRadius() override;
};

#endif
