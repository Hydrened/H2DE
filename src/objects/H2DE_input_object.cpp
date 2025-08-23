#include "H2DE/objects/H2DE_input_object.h"
#include "H2DE/engine/H2DE_geometry.h"

// INIT
H2DE_InputObject::H2DE_InputObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_InputObjectData& iod) : H2DE_Object(e, od), _inputObjectData(iod), _eventData({ this, nullptr, "", std::nullopt }) {
    _refreshSurfaceBuffers();
    _refreshMaxRadius();

    _initCursor();
    _refreshCursor();
}

void H2DE_InputObject::_initCursor() {
    H2DE_SurfaceData sd = H2DE_SurfaceData();
    sd.transform.scale = { 0.1f, _textObject->_getFixedFontSize() };

    _cursor = addSurface<H2DE_Color>("cursor", sd, H2DE_ColorData());
}

// CLEANUP
H2DE_InputObject::~H2DE_InputObject() {
    if (_eventData.timeline != nullptr) {
        _eventData.timeline->stop(false);
    }

    if (_textObject != H2DE_NULL_OBJECT) {
        if (_engine->destroyObject(_textObject)) {
            _textObject = H2DE_NULL_OBJECT;
        }
    }

    H2DE_Object::_destroySurfaces(_surfaces);
}

// ACTIONS
void H2DE_InputObject::_refreshTextObject() {
    _textObject = H2DE_Object::_refreshTextObject(_textObject, _inputObjectData.text);
}

void H2DE_InputObject::_refreshCursor() {
//     bool isCursorPositionInvalid = (_cursorPosition == -1);
//     bool hasNoText = (_textObject == H2DE_NULL_OBJECT);

//     if (isCursorPositionInvalid || hasNoText) {
//         _cursor->hide();
//         return;
//     }

//     int nbLetters = _textObject->_surfaceBuffers.size();

//     bool cursorIsAtLastPosition = (_cursorPosition >= nbLetters);
//     int surfaceIndex = ((cursorIsAtLastPosition) ? nbLetters - 1 : _cursorPosition);

//     H2DE_Surface* letter = _textObject->_surfaceBuffers.at(surfaceIndex);
//     const H2DE_LevelRect letterRect = G::getSurfaceRect(this, letter);

//     const float x = ((cursorIsAtLastPosition) ? letterRect.getMaxX() : letterRect.getMinX());
//     _cursor->setTranslate({ x, letterRect.y });

//     _cursor->show();
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

void H2DE_InputObject::input(unsigned char c) {
    if (!_isInputValid(c)) {
        return;
    }

    _inputObjectData.text.text = _inputObjectData.text.text + static_cast<char>(c);
    _refreshTextObject();

    if (_inputObjectData.onInput && !_disabled) {
        _eventData.text = _inputObjectData.text.text;
        _eventData.character = c;

        _inputObjectData.onInput(_eventData);
    }
}

void H2DE_InputObject::focus() {
    _engine->_objectManager->focusInput(this);
}

void H2DE_InputObject::blur() {
    _engine->_objectManager->blurInput(this);
}

void H2DE_InputObject::submit() {
    _engine->_objectManager->submitInput(this);
}

// SETTER
void H2DE_InputObject::setText(const std::string& text) {
    if (text == _inputObjectData.text.text) {
        return;
    }

    _inputObjectData.text.text = text;
    _refreshTextObject();
}

void H2DE_InputObject::_setCursorPosition(int position) {
    if (position == _cursorPosition) {
        return;
    }

    _cursorPosition = position;
    _refreshCursor();
}
