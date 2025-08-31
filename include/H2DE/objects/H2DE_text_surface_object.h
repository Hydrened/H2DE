#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_TextSurfaceObject : public H2DE_Object {
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

protected:
    H2DE_TextObject* _textObject = H2DE_NULL_OBJECT;
    H2DE_SurfaceMap _surfaces = {};

    H2DE_Text& _text;

    H2DE_TextSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, H2DE_Text& text);
    ~H2DE_TextSurfaceObject() override;

private:
    void _refreshSurfaceBuffers() override;
    void _refreshMaxRadius() override;
};
