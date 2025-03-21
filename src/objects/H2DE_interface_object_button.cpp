#include "H2DE/H2DE_interface_object_button.h"

// INIT
H2DE_InterfaceObjectButton::H2DE_InterfaceObjectButton(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectButtonData& iod) : H2DE_InterfaceObject(engine, od), data(iod) {

}

H2DE_InterfaceObjectButton* H2DE_CreateInterfaceObjectButton(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectButtonData& iod) {
    H2DE_InterfaceObjectButton* obj = new H2DE_InterfaceObjectButton(engine, od, iod);
    engine->addInterfaceObject(obj);
    return obj;
}

// CLEANUP
H2DE_InterfaceObjectButton::~H2DE_InterfaceObjectButton() {

}
