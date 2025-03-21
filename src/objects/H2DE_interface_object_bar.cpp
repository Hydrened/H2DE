#include "H2DE/H2DE_interface_object_bar.h"

// INIT
H2DE_InterfaceObjectBar::H2DE_InterfaceObjectBar(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectBarData& iod) : H2DE_InterfaceObject(engine, od), data(iod) {

}

H2DE_InterfaceObjectBar* H2DE_CreateInterfaceObjectBar(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectBarData& iod) {
    H2DE_InterfaceObjectBar* obj = new H2DE_InterfaceObjectBar(engine, od, iod);
    engine->addInterfaceObject(obj);
    return obj;
}

// CLEANUP
H2DE_InterfaceObjectBar::~H2DE_InterfaceObjectBar() {

}
