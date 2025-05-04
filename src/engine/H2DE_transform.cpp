#include "H2DE/H2DE_transform.h"

// FLIP
H2DE_LevelRect H2DE_Transform::flipRect(const H2DE_LevelRect& W_parentRect, const H2DE_LevelRect& L_childRect, H2DE_Flip flip) {
    H2DE_LevelRect res = L_childRect;

    if (flip & H2DE_FLIP_X) {
        res.x = W_parentRect.getSize().x - (L_childRect.getPos().x + L_childRect.getSize().x);
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = W_parentRect.getSize().y - (L_childRect.getPos().y + L_childRect.getSize().y);
    }

    return res + W_parentRect.getPos().makeNullRect();
}

float H2DE_Transform::flipRotation(float rotation, H2DE_Flip flip) {
    float res = rotation;

    switch (flip) {
        case H2DE_FLIP_X: res = -res; break;
        case H2DE_FLIP_Y: res = 180.0f - res; break;
        case H2DE_FLIP_XY: res = 180.0f + res; break;
        default: break;
    }

    while (res < 0.0f) {
        res += 360.0f;
    }
    while (res >= 360.0f) {
        res -= 360.0f;
    }

    return res;
}

H2DE_LevelPos H2DE_Transform::flipPivot(const H2DE_LevelRect& W_parentRect, const H2DE_LevelPos& L_pivot, H2DE_Flip flip) {
    H2DE_LevelPos res = L_pivot;

    if (flip & H2DE_FLIP_X) {
        res.x = W_parentRect.getSize().x - L_pivot.x;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = W_parentRect.getSize().y - L_pivot.y;
    }

    return res + W_parentRect.getPos();
}

// ROTATION
H2DE_LevelPos H2DE_Transform::applyRotationOnPos(const H2DE_LevelPos& W_pos, const H2DE_LevelPos& W_pivot, float rotation) {
    const H2DE_LevelPos W_vecToPivot = W_pos - W_pivot;

    const float rad = rotation * static_cast<float>(M_PI / 180.0f);
    H2DE_LevelPos W_rotatedVec = {
        W_vecToPivot.x * cosf(rad) - W_vecToPivot.y * sinf(rad),
        W_vecToPivot.x * sinf(rad) + W_vecToPivot.y * cosf(rad),
    };

    return W_rotatedVec + W_pivot;
}

H2DE_LevelRect H2DE_Transform::applyRotationOnRect(const H2DE_LevelRect& W_rect, const H2DE_LevelPos& W_pivot, float rotation) {
    const H2DE_LevelSize rectSize = W_rect.getSize();
    const H2DE_LevelPos W_rectCenter = W_rect.getCenter();

    const H2DE_LevelPos W_vecPivot = W_pivot - W_rectCenter;

    const float rad = rotation * static_cast<float>(M_PI / 180.0f);
    const H2DE_LevelPos W_rotatedVec = {
        W_vecPivot.x * cosf(rad) - W_vecPivot.y * sinf(rad),
        W_vecPivot.x * sinf(rad) + W_vecPivot.y * cosf(rad),
    };

    const H2DE_LevelPos W_finalRectCenter = W_pivot - W_rotatedVec;

    return H2DE_LevelPos{ W_finalRectCenter - rectSize.getCenter() }.makeRect(rectSize);
}

// BOTH
H2DE_LevelRect H2DE_Transform::getHitboxWorldDestRect(const H2DE_LevelRect& W_objectRect, const H2DE_LevelPos& L_objectPivot, float objectRotation, H2DE_Flip objectFlip, const H2DE_LevelRect& L_hitboxRect) {
    using T = H2DE_Transform;

    const H2DE_LevelRect W_fliped_hitboxRect = T::flipRect(W_objectRect, L_hitboxRect, objectFlip);

    const float rotationCausedByFlip = (objectFlip == H2DE_FLIP_XY) ? 180.0f : 0.0f;

    const int snapedRotation = std::round((objectRotation + rotationCausedByFlip) / 90.0f) * 90;
    const int objectFliped_snapedRotation = T::flipRotation(snapedRotation, objectFlip);

    const H2DE_LevelPos W_objectFliped_objectPivot = T::flipPivot(W_objectRect, L_objectPivot, objectFlip);
    return T::applyRotationOnRect(W_fliped_hitboxRect, W_objectFliped_objectPivot, objectFliped_snapedRotation);
}
