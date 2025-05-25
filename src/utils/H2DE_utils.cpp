#include "H2DE/H2DE_utils.h"

std::vector<H2DE_Translate> H2DE_Transform::getCorners() const {
    float offsetX = pivot.x - translate.x;
    float offsetY = pivot.y - translate.y;
    
    float scaledX = translate.x - offsetX;
    float scaledY = translate.y - offsetY;
    
    float halfW = scale.x * 0.5f;
    float halfH = scale.y * 0.5f;
    
    return {
        H2DE_Translate{ scaledX - halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY - halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX + halfW, scaledY + halfH }.rotate(pivot, rotation),
        H2DE_Translate{ scaledX - halfW, scaledY + halfH }.rotate(pivot, rotation),
    };
}
