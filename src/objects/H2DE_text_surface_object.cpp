#include "H2DE/objects/H2DE_text_surface_object.h"

// INIT
H2DE_TextSurfaceObject::H2DE_TextSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, std::string& text) : H2DE_Object(engine, objectData), _text(text) {
    const std::string textCopy = text;
    text = H2DE_TextObject::_getFormatedText(textCopy);
    
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_TextSurfaceObject::~H2DE_TextSurfaceObject() {
    if (_textObject != H2DE_NULL_OBJECT) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = H2DE_NULL_OBJECT;
        }
    }
}

// ACTIONS
void H2DE_TextSurfaceObject::_refreshSurfaceBuffers() {
    _textObject = H2DE_Object::_refreshTextObject(_textObject, *_text);

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::_getSortedSurfaces(_surfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    _rescaleSurfaceBuffers();
}

void H2DE_TextSurfaceObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius(_surfaces);
    
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}
