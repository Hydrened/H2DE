#include "H2DE/H2DE_bar_object.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData b) : H2DE_Object(engine, od), bod(b) {

}

H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) {
    H2DE_BarObject* object = new H2DE_BarObject(engine, od, bod);
    engine->addObject(object);
    return object;
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {

}

// UPDATE
void H2DE_BarObject::update() {
    
}
