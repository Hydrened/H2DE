#ifndef H2DE_BAR_OBJECT_H
#define H2DE_BAR_OBJECT_H

#include <H2DE/H2DE_engine.h>

class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData bod;

    H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData bod);
    ~H2DE_BarObject();

    void update() override;

public:
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_BarObjectData& bod);
};

#endif
