#include "H2DE/H2DE_utils.h"

// METHODS
H2DE_AbsRect H2DE_AbsSize::makeRect(H2DE_AbsPos pos) const {
    return { pos.x, pos.y, w, h };
}
