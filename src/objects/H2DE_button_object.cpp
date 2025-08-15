#include "H2DE/objects/H2DE_button_object.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) : H2DE_Object(e, od), _buttonObjectData(bod), _eventData({ this, nullptr }) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    if (_eventData.timeline != nullptr) {
        _eventData.timeline->stop(false);
    }

    if (_textObject != nullptr) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = nullptr;
        }
    }

    H2DE_Object::_destroySurfaces(_surfaces);
}

// ACTIONS
void H2DE_ButtonObject::_refreshTextObject() {
    if (_textObject != nullptr) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = nullptr;
        }
    }

    if (_buttonObjectData.text.text == "") {
        return;
    }

    H2DE_ObjectData od = _objectData;
    od.index++;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    tod.text = _buttonObjectData.text;

    _textObject = _engine->createObject<H2DE_TextObject>(od, tod);
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
        _buttonObjectData.onMouseDown(_eventData);
    }
}

void H2DE_ButtonObject::mouseUp() {
    if (_buttonObjectData.onMouseUp && !_disabled) {
        _buttonObjectData.onMouseUp(_eventData);
    }
}

void H2DE_ButtonObject::mouseHover() {
    if (_buttonObjectData.onHover && !_disabled) {
        _buttonObjectData.onHover(_eventData);
    }
}

void H2DE_ButtonObject::mouseBlur() {
    if (_buttonObjectData.onBlur && !_disabled) {
        _buttonObjectData.onBlur(_eventData);
    }
}
