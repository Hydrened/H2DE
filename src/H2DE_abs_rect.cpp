#include "H2DE/H2DE_utils.h"

H2DE_AbsRect::operator SDL_Rect() const {
    return { x, y, w, h };
}

bool H2DE_AbsRect::contains(const H2DE_AbsPos& pos) {
    return (pos.x >= x && pos.x <= x + w && pos.y >= y && pos.y <= y + h);
}
