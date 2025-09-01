#include "H2DE/objects/H2DE_button_object.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) : H2DE_TextSurfaceObject(e, od, bod.text), _buttonObjectData(bod) {
    _buttonObjectData.text.text = H2DE_TextSurfaceObject::_getFormatedText(_buttonObjectData.text.text);
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    
}

// ACTIONS
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
    const std::string formatedText = H2DE_TextSurfaceObject::_getFormatedText(text);

    if (formatedText == _buttonObjectData.text.text) {
        return;
    }

    _buttonObjectData.text.text = formatedText;
    _setText(_buttonObjectData.text);
}
