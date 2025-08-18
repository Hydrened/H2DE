#include "H2DE/objects/H2DE_input_object.h"

// INIT
H2DE_InputObject::H2DE_InputObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_InputObjectData& iod) : H2DE_Object(e, od), _inputObjectData(iod), _eventData({ this, nullptr, "", std::nullopt }) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();
}

// CLEANUP
H2DE_InputObject::~H2DE_InputObject() {
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
void H2DE_InputObject::_refreshTextObject() {
    _textObject = H2DE_Object::_refreshTextObject(_textObject, _inputObjectData.text);
}

void H2DE_InputObject::_refreshSurfaceBuffers() {
    _refreshTextObject();

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::_getSortedSurfaces(_surfaces);

    _surfaceBuffers.clear();
    _surfaceBuffers.reserve(sortedSurfaces.size());
    _surfaceBuffers.insert(_surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    _rescaleSurfaceBuffers();
}

void H2DE_InputObject::_refreshMaxRadius() {
    float maxHitboxesRadius = _getMaxHitboxRadius();
    float maxSurfaceRadius = _getMaxSurfaceRadius(_surfaces);
    
    _maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_InputObject::input(char c) {
    _inputObjectData.text.text = _inputObjectData.text.text + c;
    _refreshTextObject();

    if (_inputObjectData.onInput && !_disabled) {
        _eventData.text = _inputObjectData.text.text;
        _eventData.character = c;

        _inputObjectData.onInput(_eventData);
    }
}

void H2DE_InputObject::focus() {
    if (_inputObjectData.onFocus && !_disabled) {
        _eventData.text = _inputObjectData.text.text;
        _inputObjectData.onFocus(_eventData);
    }
}

void H2DE_InputObject::blur() {
    if (_inputObjectData.onBlur && !_disabled) {
        _eventData.text = _inputObjectData.text.text;
        _inputObjectData.onBlur(_eventData);
    }
}

// SETTER
void H2DE_InputObject::setText(const std::string& text) {
    _inputObjectData.text.text = text;
    _refreshTextObject();
}
