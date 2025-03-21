#ifndef H2DE_INTERFACE_OBJECT_BUTTON_H
#define H2DE_INTERFACE_OBJECT_BUTTON_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_interface_object.h>

class H2DE_InterfaceObjectButton : public H2DE_InterfaceObject {
private:
    H2DE_InterfaceObjectButtonData data;

    H2DE_InterfaceObjectButton(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectButtonData& iod);
    ~H2DE_InterfaceObjectButton();
    
public:
    friend H2DE_InterfaceObjectButton* H2DE_CreateInterfaceObjectButton(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectButtonData& iod);
};

#endif
