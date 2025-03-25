#include "H2DE/H2DE_object.h"

// INIT
H2DE_ButtonObject::H2DE_ButtonObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_ButtonObjectData b) : H2DE_Object(engine, od), bod(b) {

}

H2DE_ButtonObject* H2DE_CreateButtonObject(H2DE_Engine* engine, const H2DE_ObjectData& od, const H2DE_ButtonObjectData& bod) {
    H2DE_ButtonObject* object = new H2DE_ButtonObject(engine, od, bod);
    engine->objects.push_back(object);
    return object;
}

// CLEANUP
H2DE_ButtonObject::~H2DE_ButtonObject() {
    if (bod.surface) {
        delete bod.surface;
    }
}

// UPDATE
void H2DE_ButtonObject::update() {
    if (!bod.onclick && !bod.onhover && !bod.onout) {
        return;
    }

    H2DE_LevelPos mousePos = H2DE_GetMousePos(engine, false);
    H2DE_LevelPos pos = od.pos;
    bool clicked = false;
    bool hovered = false;

    for (auto [name, hitbox] : od.hitboxes) {
        if (hitbox.rect.addPos(pos).collides(mousePos)) {
            if (bod.onclick && !clicked && engine->click.has_value()) {
                bod.onclick();
                clicked = true;
            }

            if (bod.onhover && !hover && !hovered) {
                hover = true;
                hovered = true;
                bod.onhover();
            }
        }
    }

    if (!hovered && hover && bod.onout) {
        int nbOut = 0;

        for (auto [name, hitbox] : od.hitboxes) {
            if (!hitbox.rect.addPos(pos).collides(mousePos)) {
                nbOut++;
            }
        }

        if (nbOut >= od.hitboxes.size()) {
            bod.onout();
            hover = false;
        }
    }
}

// GETTER
std::vector<H2DE_Surface*> H2DE_ButtonObject::getSurfaces() const {
    return { bod.surface };
}
