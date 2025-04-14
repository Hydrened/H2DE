#ifndef H2DE_BASIC_OBJECT_H
#define H2DE_BASIC_OBJECT_H

#include <H2DE/objects/H2DE_object.h>

class H2DE_BasicObject : public H2DE_Object {
    private:
        H2DE_BasicObjectData bod;
    
        H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData bod);
        ~H2DE_BasicObject() override;
    
        void resetSurfaceBuffers() override;
    
    public:
        friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
        friend H2DE_Surface* H2DE_GetBasicObjectSurface(const H2DE_BasicObject* basicObject, const std::string& name);

        friend void H2DE_AddSurfaceToBasicObject(H2DE_BasicObject* basicObject, H2DE_Surface* surface, const std::string& name);
        friend void H2DE_RemoveSurfaceFromBasicObject(H2DE_BasicObject* basicObject, const std::string& name);
    };

#endif
