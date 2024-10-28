#include <H2DE/H2DE_calculator.h>

bool H2DE_Calculator::isIndexGreater(const H2DE_GraphicObject* s1, const H2DE_GraphicObject* s2) {
    return s1->index < s2->index;
}

float H2DE_Calculator::convertToRadians(float rotation) {
    return rotation * M_PI / 180.0f;
}

H2DE_Size H2DE_Calculator::getPolygonSize(std::vector<SDL_Point> points) {
    SDL_Point min = { 0, 0 };
    SDL_Point max = { 0, 0 };
    size_t nbPoints = points.size();

    for (int i = 0; i < nbPoints; i++) {
        SDL_Point point = points[i];
        if (point.x < min.x) min.x = point.x;
        else if (point.x > max.x) max.x = point.x;
        if (point.y < min.y) min.y = point.y;
        else if (point.y > max.y) max.y = point.y;
    }
    return { min.x + max.x, min.y + max.y };
}

H2DE_Pos H2DE_Calculator::getPosFromParents(H2DE_GraphicObject* g) {
    H2DE_Pos pos = g->pos;
    H2DE_GraphicObject* parent = g->parent;
    while (parent) {
        pos.x += parent->pos.x;
        pos.y += parent->pos.y;
        parent = parent->parent;
    }
    return pos;
}

H2DE_Pos H2DE_Calculator::getCenter(H2DE_Pos pos, H2DE_Size size, H2DE_Scale scale) {
    return { static_cast<int>(pos.x + size.w * scale.x / 2), static_cast<int>(pos.y + size.h * scale.y / 2) };
}

H2DE_Pos H2DE_Calculator::getFlipedPos(H2DE_Pos pos, H2DE_Pos center, SDL_RendererFlip flip) {
    switch (flip) {
        case SDL_FLIP_VERTICAL: return { center.x * 2 - pos.x, pos.y };
        case SDL_FLIP_HORIZONTAL: return { pos.x, center.y * 2 - pos.y };
        default: return pos;
    }
}

H2DE_Pos H2DE_Calculator::getRotationOrigin(H2DE_Pos pos, H2DE_Pos origin) {
    return { pos.x + origin.x, pos.y + origin.y };
}

H2DE_Pos H2DE_Calculator::getScaleOrigin(H2DE_Pos pos, H2DE_Pos origin) {
    return { pos.x + origin.x, pos.y + origin.y };
}

H2DE_Pos H2DE_Calculator::getRotatedPos(H2DE_Pos pos, H2DE_Pos rotationOrigin, float rotation) {
    if (rotation != 0.0f) {
        float x_translated = pos.x - rotationOrigin.x;
        float y_translated = pos.y - rotationOrigin.y;

        float radians = H2DE_Calculator::convertToRadians(rotation);

        float x_rotated = x_translated * std::cos(radians) - y_translated * std::sin(radians);
        float y_rotated = x_translated * std::sin(radians) + y_translated * std::cos(radians);

        float x_final = x_rotated + rotationOrigin.x;
        float y_final = y_rotated + rotationOrigin.y;

        return { static_cast<int>(x_final), static_cast<int>(y_final) };
    } else return pos;
}

H2DE_Pos H2DE_Calculator::getRescaledPos(H2DE_Pos pos, H2DE_Size size, H2DE_Pos origin, H2DE_Scale scale) {
    if (scale.x != 1.0f || scale.y != 1.0f) {
        int xOffset = ((size.w * scale.x - size.w) * (static_cast<float>(origin.x - pos.x) / static_cast<float>(size.w)));
        int yOffset = ((size.h * scale.y - size.h) * (static_cast<float>(origin.y - pos.y) / static_cast<float>(size.h)));
        return { pos.x - xOffset, pos.y - yOffset };
    } return pos;
}






// H2DE_Size H2DE_Calculator::getRescaledSize(H2DE_Size size, H2DE_Scale scale) {
//     if (scale.x != 1.0f || scale.y != 1.0f) return { static_cast<int>(size.w * scale.x), static_cast<int>(size.h * scale.y) };
//     else return size;
// }

// H2DE_Pos H2DE_Calculator::getRescaledRotationOrigin(H2DE_Pos rotationOrigin, H2DE_Scale scale) {
//     if (scale.x != 1.0f || scale.y != 1.0f) return { static_cast<int>(rotationOrigin.x * scale.x), static_cast<int>(rotationOrigin.y * scale.y) };
//     else return rotationOrigin;
// }








// H2DE_Pos H2DE_Calculator::getRescaledPos(H2DE_Pos pos, H2DE_Size size, H2DE_Pos scaleOrigin, H2DE_Scale scale) {
//     if (scale.x != 1.0f || scale.y != 1.0f) {
//         int xOffset = ((size.w * scale.x - size.w) * (static_cast<float>(scaleOrigin.x) / static_cast<float>(size.w))) * -1;
//         int yOffset = ((size.h * scale.y - size.h) * (static_cast<float>(scaleOrigin.y) / static_cast<float>(size.h))) * -1;
//         return { pos.x + xOffset, pos.y + yOffset };
//     } return pos;
// }



// H2DE_Pos H2DE_Calculator::getRotatedPos(H2DE_Pos pos, H2DE_Pos rotationOrigin, float rotation) {
//     if (rotation != 0.0f) {
//         float x_translated = pos.x - rotationOrigin.x;
//         float y_translated = pos.y - rotationOrigin.y;

//         float radians = H2DE_Calculator::convertToRadians(rotation);

//         float x_rotated = x_translated * std::cos(radians) - y_translated * std::sin(radians);
//         float y_rotated = x_translated * std::sin(radians) + y_translated * std::cos(radians);

//         float x_final = x_rotated + rotationOrigin.x;
//         float y_final = y_rotated + rotationOrigin.y;

//         return { static_cast<int>(x_final), static_cast<int>(y_final) };
//     } else return pos;
// }


// H2DE_Pos H2DE_Calculator::getFlipedPos(H2DE_Pos pos, H2DE_Pos objPos, H2DE_Size size, SDL_RendererFlip flip) {
//     H2DE_Pos center = { objPos.x + static_cast<int>(size.w / 2), objPos.y + static_cast<int>(size.h / 2) };

//     switch (flip) {
//         case SDL_FLIP_VERTICAL: return { center.x * 2 - pos.x, pos.y };
//         case SDL_FLIP_HORIZONTAL: return { pos.x, center.y * 2 - pos.y };
//         default: return pos;
//     }
// }
