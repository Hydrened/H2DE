#include "H2DE_renderer.h"

H2DE_LevelPos H2DE_Renderer::getTranslatedPos(H2DE_GraphicObject* object) const {
    H2DE_LevelPos res = { 0.0f, 0.0f };

    whileParent(object, [this, &res](H2DE_GraphicObject* parent) {
        res = res + getPos(parent);
    });

    return res;
}

H2DE_LevelPos H2DE_Renderer::getScaledPos(H2DE_GraphicObject* object) const {
    return getPos(object);
}
