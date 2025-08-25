#include "H2DE/objects/H2DE_button_object.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) : H2DE_Object(e, od), _buttonObjectData(bod) {
    _buttonObjectData.text.text = H2DE_TextObject::_getFormatedText(_buttonObjectData.text.text);

    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    if (_textObject != H2DE_NULL_OBJECT) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = H2DE_NULL_OBJECT;
        }
    }

    H2DE_Object::_destroySurfaces(_surfaces);
}

// ACTIONS
void H2DE_ButtonObject::_refreshTextObject() {
    _textObject = H2DE_Object::_refreshTextObject(_textObject, _buttonObjectData.text);
}

void H2DE_ButtonObject::_refreshSurfaceBuffers() {
    _refreshTextObject();

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::_getSortedSurfaces(_surfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    _rescaleSurfaceBuffers();
}

void H2DE_ButtonObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius(_surfaces);
    
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_ButtonObject::mouseDown() {
    if (_buttonObjectData.onMouseDown && !_disabled) {
        _buttonObjectData.onMouseDown({ this });
    }
}

void H2DE_ButtonObject::mouseUp() {
    if (_buttonObjectData.onMouseUp && !_disabled) {
        _buttonObjectData.onMouseUp({ this });
    }
}

void H2DE_ButtonObject::mouseHover() {
    if (_buttonObjectData.onHover && !_disabled) {
        _buttonObjectData.onHover({ this });
    }
}

void H2DE_ButtonObject::mouseBlur() {
    if (_buttonObjectData.onBlur && !_disabled) {
        _buttonObjectData.onBlur({ this });
    }
}

// SETTER
void H2DE_ButtonObject::setText(const std::string& text) {
    if (H2DE_TextObject::_getFormatedText(text) == _buttonObjectData.text.text) {
        return;
    }

    _buttonObjectData.text.text = H2DE_TextObject::_getFormatedText(text);
    _refreshTextObject();
}
