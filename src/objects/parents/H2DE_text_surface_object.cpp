#include "H2DE/objects/parents/H2DE_text_surface_object.h"
#include "H2DE/engine/H2DE_engine.h"

// INIT
H2DE_TextSurfaceObject::H2DE_TextSurfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_Text& text) : H2DE_Object(engine, objectData) {
    _setText(text);
    _refreshSurfaceBuffers();
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
    _refreshTextObject();
}

void H2DE_TextSurfaceObject::_refreshTextObject() {
    _textObject = H2DE_Object::_refreshTextObject(_textObject, _text);
}

// GETTER
const std::string H2DE_TextSurfaceObject::_getFormatedText(const std::string& text) {
    if (text.length() < 2) {
        return text;
    }

    std::string res = "";

    for (int i = 1; i < text.length() - 1; i++) {
        const char& pc = text.at(i - 1);
        const char& cc = text.at(i);
        const char& nc = text.at(i + 1);

        bool isFriendReturn = (pc == '\n' || nc == '\n');
        if (cc == ' ' && isFriendReturn) {
            continue;
        }

        res += cc;
    }

    return text.substr(0, 1) + res + text.substr(text.length() - 1, 1);
}

// SETTER
void H2DE_TextSurfaceObject::_setText(const H2DE_Text& text) {
    _text = text;
    _text.text = H2DE_TextSurfaceObject::_getFormatedText(text.text);
    
    _refreshTextObject();
}
