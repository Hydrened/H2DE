#pragma once

#include <H2DE/objects/H2DE_object.h>

class H2DE_BasicObject : public H2DE_SingleSurfaceObject {
private:
    H2DE_BasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData) noexcept : H2DE_SingleSurfaceObject(engine, objectData) {}
    ~H2DE_BasicObject() override;

    friend class H2DE_Engine;
};
