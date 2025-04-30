#ifndef H2DE_BUTTON_OBJECT_H
#define H2DE_BUTTON_OBJECT_H

/**
 * @file H2DE_button_object.h
 * @brief Header file for the `H2DE_ButtonObject` class.
 * 
 * This file contains the declaration of the `H2DE_ButtonObject` class, which is
 * used to represent button objects in the H2DE engine. These objects allow for
 * interactions such as mouse hovering, clicking, and event handling, making them
 * ideal for user interface elements.
 * 
 * The class inherits from `H2DE_Object`, providing basic object functionality
 * while adding specific behavior for buttons, including surface management and
 * callback handling for mouse events.
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @brief A button object class within the H2DE engine.
 * 
 * This class extends `H2DE_Object` and represents a button element in the H2DE engine.
 * It manages surfaces, mouse interactions, and provides functionality for handling
 * hover, click, and blur states. Callback functions can be assigned for each mouse
 * event (mouse down, mouse up, hover, and blur).
 * 
 * Instances of this class should be created through the `H2DE_CreateButtonObject`
 * function and managed according to the engine's object management practices.
 */
class H2DE_ButtonObject : public H2DE_Object {
private:
    H2DE_ButtonObjectData bod;

    bool hover = false;

    H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData bod);
    ~H2DE_ButtonObject() override;

    void resetSurfaceBuffers() override;

public:
    /**
     * @brief Creates a button object.
     * 
     * This function creates and initializes a new `H2DE_ButtonObject` with the provided
     * engine, object data, and button-specific data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param objectData The general object data for initialization.
     * @param buttonObjectData The specific data for the button object.
     * @return A pointer to the created `H2DE_ButtonObject`.
     */
    friend H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_ButtonObjectData& buttonObjectData);
    /**
     * @brief Retrieves a surface from the button object.
     * 
     * This function retrieves a surface associated with the `H2DE_ButtonObject`,
     * identified by the given name.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param name The name of the surface to retrieve.
     * @return A pointer to the requested surface.
     */
    friend H2DE_Surface* H2DE_GetButtonSurface(const H2DE_ButtonObject* button, const std::string& name);

    /**
     * @brief Adds a surface to the button object.
     * 
     * This function adds a surface to the `H2DE_ButtonObject`, identified by the
     * provided name.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param surface A pointer to the surface to be added.
     * @param name The name to associate with the surface.
     */
    friend void H2DE_AddSurfaceToButtonObject(H2DE_ButtonObject* button, H2DE_Surface* surface, const std::string& name);
    /**
     * @brief Removes a surface from the button object.
     * 
     * This function removes a surface from the `H2DE_ButtonObject`, identified by
     * the given name.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param name The name of the surface to remove.
     */
    friend void H2DE_RemoveSurfaceFromButtonObject(H2DE_ButtonObject* button, const std::string& name);
    
    /**
     * @brief Triggers a mouse down event on the button.
     * 
     * This function triggers the mouse down event on the button, typically called
     * when the mouse button is pressed while hovering over the button.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     */
    friend void H2DE_ButtonMouseDown(H2DE_ButtonObject* button);
    /**
     * @brief Triggers a mouse up event on the button.
     * 
     * This function triggers the mouse up event on the button, typically called
     * when the mouse button is released after being pressed on the button.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     */
    friend void H2DE_ButtonMouseUp(H2DE_ButtonObject* button);
    /**
     * @brief Triggers a hover event on the button.
     * 
     * This function triggers the hover event on the button, typically called when
     * the mouse pointer hovers over the button.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     */
    friend void H2DE_ButtonHover(H2DE_ButtonObject* button);
    /**
     * @brief Triggers a blur event on the button.
     * 
     * This function triggers the blur event on the button, typically called when
     * the mouse pointer leaves the button area after hovering.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     */
    friend void H2DE_ButtonBlur(H2DE_ButtonObject* button);

    /**
     * @brief Sets a callback for the mouse down event.
     * 
     * This function sets the callback to be executed when the mouse down event occurs
     * on the button (when the mouse button is pressed while hovering over the button).
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param onMouseDown A function to be executed on mouse down.
     */
    friend void H2DE_SetButtonOnMouseDown(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseDown);
    /**
     * @brief Sets a callback for the mouse up event.
     * 
     * This function sets the callback to be executed when the mouse up event occurs
     * on the button (when the mouse button is released after being pressed on the button).
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param onMouseUp A function to be executed on mouse up.
     */
    friend void H2DE_SetButtonOnMouseUp(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onMouseUp);
    /**
     * @brief Sets a callback for the hover event.
     * 
     * This function sets the callback to be executed when the mouse hovers over the button.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param onHover A function to be executed on hover.
     */
    friend void H2DE_SetButtonOnHover(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onHover);
    /**
     * @brief Sets a callback for the blur event.
     * 
     * This function sets the callback to be executed when the mouse leaves the button
     * area after hovering.
     * 
     * @param button A pointer to the `H2DE_ButtonObject`.
     * @param onBlur A function to be executed on blur.
     */
    friend void H2DE_SetButtonOnBlur(H2DE_ButtonObject* button, const std::function<void(H2DE_Object*)>& onBlur);

    friend class H2DE_Engine;
};

#endif
