#include "H2DE/objects/H2DE_basic_object.h"

// ACTIONS
void H2DE_BasicObject::_refreshSurfaceBuffers() {
    H2DE_Object::_refreshSurfaceBuffers();
    H2DE_SingleSurfaceObject::_refreshSurfaceBuffers();
}
