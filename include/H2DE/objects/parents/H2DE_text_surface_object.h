#pragma once

/**
 * @file H2DE_text_surface_object.h
 * @brief Intermediate base class for objects with text rendering support.
 * 
 * This file defines the H2DE_TextSurfaceObject class, which extends
 * H2DE_Object to add text rendering capabilities. It provides a bridge
 * between core objects and UI elements (such as buttons, inputs)
 * that require an associated text label or content.
 * 
 * H2DE_TextSurfaceObject manages a dedicated H2DE_TextObject, as well as
 * internal text data and surface updates. It handles synchronization
 * between the object and its text representation, ensuring proper rendering
 * of labels, values, or descriptions.
 */

#include <H2DE/objects/parents/H2DE_object.h>

/**
 * @class H2DE_TextSurfaceObject
 * @brief Base class for objects that include a text element.
 * 
 * H2DE_TextSurfaceObject is designed as an intermediate class
 * for UI components (like buttons or inputs) that
 * need to display text alongside their graphical surface.
 * 
 * Responsibilities:
 * - Hold and manage a pointer to an internal H2DE_TextObject
 * - Store text data (`H2DE_Text`) and provide formatting utilities
 * - Refresh surface buffers to ensure text is properly rendered
 * - Provide safe access to the text object for customization or updates
 * 
 * This class is not meant to be used directly but to serve as a
 * foundation for higher-level text-based UI objects.
 */
class H2DE_TextSurfaceObject : virtual public H2DE_Object {
public:
    /**
     * @brief Get the text object associated with the button.
     * 
     * Returns a pointer to the `H2DE_TextObject` used for renderings
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

    H2DE_TextSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_Text& text);
    ~H2DE_TextSurfaceObject() override;

    static const std::string _getFormatedText(const std::string& text);

    void _setText(const H2DE_Text& text);

    void _refreshSurfaceBuffers() override;

private:
    static constexpr bool _isParentObject = true;
    
    H2DE_Text _text = H2DE_Text();

    void _refreshTextObject();
};
