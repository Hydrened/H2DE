#ifndef H2DE_INTERFACE_OBJECT_H
#define H2DE_INTERFACE_OBJECT_H

#include <H2DE/H2DE_engine.h>
#include <H2DE/H2DE_object.h>

class H2DE_InterfaceObject : public H2DE_Object {
private:

protected:
    H2DE_InterfaceObject(H2DE_Engine* engine, const H2DE_ObjectData& data);
    ~H2DE_InterfaceObject();
    
public:
    friend void H2DE_DestroyInterfaceObject(H2DE_Engine* engine, H2DE_InterfaceObject* obj);
};

#endif
