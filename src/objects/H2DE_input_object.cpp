#include "H2DE/objects/H2DE_input_object.h"

// INIT
H2DE_InputObject::H2DE_InputObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_InputObjectData& iod) : H2DE_Object(e, od), _inputObjectData(iod) {
    _inputObjectData.text.text = H2DE_TextObject::_getFormatedText(_inputObjectData.text.text);
    
    _refreshSurfaceBuffers();
    _refreshMaxRadius();

    _initCursor();
    _refreshCursor();
}

void H2DE_InputObject::_initCursor() {
    H2DE_SurfaceData sd = H2DE_SurfaceData();
    sd.transform.scale = { _inputObjectData.text.spacing.x, _textObject->_getFixedFontSize() };

    _cursor = addSurface<H2DE_Color>("cursor", sd, H2DE_ColorData());
    _cursor->_fromText = true;
}

// CLEANUP
H2DE_InputObject::~H2DE_InputObject() {
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
    bool isCursorPositionInvalid = (_cursorPosition == -1);
    bool hasNoText = (_textObject == H2DE_NULL_OBJECT);

    if (isCursorPositionInvalid || hasNoText) {
        _cursor->hide();
        return;
    }

    int nbLetters = _textObject->_surfaceBuffers.size();

    bool cursorIsAtLastPosition = (_cursorPosition >= nbLetters);
    int surfaceIndex = ((cursorIsAtLastPosition) ? nbLetters - 1 : H2DE::clamp(_cursorPosition, 0, nbLetters + 1));

    H2DE_Surface* letter = _textObject->_surfaceBuffers.at(surfaceIndex);

    H2DE_LevelRect letterRect = letter->getRect();
    letterRect.x *= 1.0f / _objectData.transform.scale.x;
    letterRect.y *= 1.0f / _objectData.transform.scale.y;

    const float x = ((cursorIsAtLastPosition) ? letterRect.getMaxX() : letterRect.getMinX() + _inputObjectData.text.spacing.x * 0.5f);
    _cursor->setTranslate({ x, letterRect.y });

    _cursor->show();
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
    if (_disabled) {
        return;
    }

    if (!_isInputValid(c)) {
        return;
    }

    char character = static_cast<char>(c);

    _inputObjectData.text.text = _inputObjectData.text.text + character;
    _refreshTextObject();

    if (_inputObjectData.onInput && !_disabled) {
        _inputObjectData.onInput({ this, _inputObjectData.text.text, character });
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
    if (H2DE_TextObject::_getFormatedText(text) == _inputObjectData.text.text) {
        return;
    }

    _inputObjectData.text.text = H2DE_TextObject::_getFormatedText(text);
    _refreshTextObject();
}

void H2DE_InputObject::_setCursorPosition(int position) {
    if (position == _cursorPosition) {
        return;
    }

    _cursorPosition = position;
    _refreshCursor();
}
