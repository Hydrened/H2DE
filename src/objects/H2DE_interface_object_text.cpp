#include "H2DE/H2DE_interface_object_text.h"

// INIT
H2DE_InterfaceObjectText::H2DE_InterfaceObjectText(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectTextData& iod) : H2DE_InterfaceObject(engine, od), data(iod) {

}

H2DE_InterfaceObjectText* H2DE_CreateInterfaceObjectText(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectTextData& iod) {
    H2DE_InterfaceObjectText* obj = new H2DE_InterfaceObjectText(engine, od, iod);
    engine->addInterfaceObject(obj);
    return obj;
}

// CLEANUP
H2DE_InterfaceObjectText::~H2DE_InterfaceObjectText() {

}
