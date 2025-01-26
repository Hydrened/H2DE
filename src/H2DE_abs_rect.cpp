#include "H2DE_utils.h"

H2DE_AbsRect::operator SDL_Rect() const {
    return { x, y, w, h };
}
