#pragma once

/**
 * @file H2DE_text_surface_object.h
 * @brief Defines H2DE_TextSurfaceObject, a base class for single-surface objects with an associated text object.
 * 
 * This file contains the H2DE_TextSurfaceObject class, which inherits from H2DE_SingleSurfaceObject and
 * provides functionalities to manage a single drawable surface along with a text object.
 * 
 * The text object allows rendering labels, dynamic content, or formatted text on top of the surface.
 * The class also provides methods for text formatting, updating, and automatic refresh of surface buffers.
 * 
 * @note Internal methods handle text formatting and buffer refresh automatically.
 * @note Inherits all surface management functions from H2DE_SingleSurfaceObject.
 */

#include <H2DE/objects/parents/H2DE_single_surface_object.h>

/**
 * @class H2DE_TextSurfaceObject
 * @brief Base class for objects that include a single surface with an associated text object.
 * 
 * H2DE_TextSurfaceObject extends H2DE_SingleSurfaceObject by adding:
 * - A dedicated text object for rendering labels or dynamic text content
 * - Text management and formatting utilities
 * - Automatic refresh of the text object when the surface or text changes
 * 
 * This class is ideal for UI elements, buttons, or any object that requires a single surface with text.
 * 
 * @note Internal methods handle text formatting and buffer refresh automatically.
 * @note Inherits all surface management functions from H2DE_SingleSurfaceObject.
 */
class H2DE_TextSurfaceObject : public H2DE_SingleSurfaceObject {
public:
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

    friend class H2DE_Engine;
    friend class H2DE_TextObject;

protected:
    H2DE_TextObject* _textObject = H2DE_NULL_OBJECT;

    H2DE_Text _text = H2DE_Text();

    H2DE_TextSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_Text& text);
    ~H2DE_TextSurfaceObject() override;

    static const std::string _getFormatedText(const std::string& text);

    void _setText(const H2DE_Text& text);

    void _refreshSurfaceBuffers() override;
    void _refreshTextObject();
};
