#include "H2DE/H2DE_text_object.h"

// INIT
H2DE_TextObject::H2DE_TextObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_TextObjectData t) : H2DE_Object(engine, od), tod(t) {

}

H2DE_TextObject* H2DE_CreateTextObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_TextObjectData& tod) {
    H2DE_TextObject* object = new H2DE_TextObject(engine, od, tod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_TextObject::~H2DE_TextObject() {

}

// GETTER
std::vector<H2DE_Surface*> H2DE_TextObject::getSurfaces() const {
    return {  };
}
