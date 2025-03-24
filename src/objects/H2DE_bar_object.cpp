#include "H2DE/H2DE_bar_object.h"

// INIT
H2DE_BarObject::H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData b) : H2DE_Object(engine, od), bod(b), value(b.defaultValue) {

}

H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod) {
    H2DE_BarObject* object = new H2DE_BarObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_BarObject::~H2DE_BarObject() {
    if (bod.front) {
        delete bod.front;
    }
    if (bod.background) {
        delete bod.background;
    }
}

// UPDATE
void H2DE_BarObject::updateImpl() {
    std::cout << value << std::endl;

    // bod.front->sd.size // je sais aps trop comment faire pour changer la size de la bar front sans changer la size du background
}

// GETTER
std::vector<H2DE_Surface*> H2DE_BarObject::getSurfaces() const {
    return { bod.background, bod.front };
}

// SETTER
void H2DE_SetBarValue(H2DE_Engine* engine, H2DE_BarObject* bar, float value) {
    bar->value = value;
}
