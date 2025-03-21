#ifndef H2DE_INTERFACE_OBJECT_TEXT_H
#define H2DE_INTERFACE_OBJECT_TEXT_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_interface_object.h>

class H2DE_InterfaceObjectText : public H2DE_InterfaceObject {
private:
    H2DE_InterfaceObjectTextData data;

    H2DE_InterfaceObjectText(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectTextData& iod);
    ~H2DE_InterfaceObjectText();
    
public:
    friend H2DE_InterfaceObjectText* H2DE_CreateInterfaceObjectText(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectTextData& iod);
};

#endif
