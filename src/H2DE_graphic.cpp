#include <H2DE/H2DE_graphic.h>

void H2DE_DestroyGraphicObject(H2DE_GraphicObject* g) {
    delete g;
}

H2DE_GraphicObject* H2DE_CreateGraphicObject() {
    return new H2DE_GraphicObject();
}

H2DE_GraphicObject* H2DE_CreateGraphicObject(H2DE_GraphicObject g) {
    return new H2DE_GraphicObject(g);
}
