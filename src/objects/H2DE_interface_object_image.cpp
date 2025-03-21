#include "H2DE/H2DE_interface_object_image.h"

// INIT
H2DE_InterfaceObjectImage::H2DE_InterfaceObjectImage(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectImageData& iod) : H2DE_InterfaceObject(engine, od), data(iod) {

}

H2DE_InterfaceObjectImage* H2DE_CreateInterfaceObjectImage(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectImageData& iod) {
    H2DE_InterfaceObjectImage* obj = new H2DE_InterfaceObjectImage(engine, od, iod);
    engine->addInterfaceObject(obj);
    return obj;
}

// CLEANUP
H2DE_InterfaceObjectImage::~H2DE_InterfaceObjectImage() {

}
