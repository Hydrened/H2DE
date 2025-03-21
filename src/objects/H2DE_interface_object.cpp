#include "H2DE/H2DE_interface_object.h"

// INIT
H2DE_InterfaceObject::H2DE_InterfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& data) : H2DE_Object(engine, data) {

}

// CLEANUP
H2DE_InterfaceObject::~H2DE_InterfaceObject() {

}

void H2DE_DestroyInterfaceObject(H2DE_Engine* engine, H2DE_InterfaceObject* obj) {
    if (obj) {
        engine->destroyInterfaceObject(obj);
        delete obj;
        obj = nullptr;
    }
}
