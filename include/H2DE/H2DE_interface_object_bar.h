#ifndef H2DE_INTERFACE_OBJECT_BAR_H
#define H2DE_INTERFACE_OBJECT_BAR_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_interface_object.h>

class H2DE_InterfaceObjectBar : public H2DE_InterfaceObject {
private:
    H2DE_InterfaceObjectBarData data;

    H2DE_InterfaceObjectBar(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectBarData& iod);
    ~H2DE_InterfaceObjectBar();
    
public:
    friend H2DE_InterfaceObjectBar* H2DE_CreateInterfaceObjectBar(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectBarData& iod);
};

#endif
