#ifndef H2DE_BASIC_OBJECT_H
#define H2DE_BASIC_OBJECT_H

#include <H2DE/H2DE_engine.h>

class H2DE_BasicObject : public H2DE_Object {
private:
    H2DE_BasicObjectData bod;

    H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData bod);
    ~H2DE_BasicObject() override;

    void updateImpl() override;

    std::vector<H2DE_Surface*> getSurfaces() const override;

public:
    friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
    friend class H2DE_Engine;
};

#endif
