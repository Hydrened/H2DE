#ifndef H2DE_INTERFACE_OBJECT_IMAGE_H
#define H2DE_INTERFACE_OBJECT_IMAGE_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_interface_object.h>

class H2DE_InterfaceObjectImage : public H2DE_InterfaceObject {
private:
    H2DE_InterfaceObjectImageData data;

    H2DE_InterfaceObjectImage(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectImageData& iod);
    ~H2DE_InterfaceObjectImage();
    
public:
    friend H2DE_InterfaceObjectImage* H2DE_CreateInterfaceObjectImage(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_InterfaceObjectImageData& iod);
};

#endif
