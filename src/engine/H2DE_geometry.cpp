#include "H2DE/engine/H2DE_geometry.h"

// RECT
H2DE_LevelRect H2DE_Geometry::getRect(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle) {
    const H2DE_Transform parentTransform = object->getTransform();
    const H2DE_Transform childTransform = transform;
    
    const H2DE_LevelRect world_parentRect = parentTransform.translate.makeRect(parentTransform.scale);
    const H2DE_LevelRect local_childRect = childTransform.translate.makeRect(childTransform.scale);

    H2DE_Flip parentFlip = G::getFlipFromScale(parentTransform.scale);
    H2DE_Flip childFlip = G::getFlipFromScale(childTransform.scale);

    // 1 => FLIP PARENT
    // -- 1 => flip parent pivot in parent rect with parent flip
    const H2DE_Pivot local_parentFliped_parentPivot = G::flipPivot(world_parentRect, parentTransform.pivot, parentFlip);
    const H2DE_Pivot world_parentFliped_parentPivot = local_parentFliped_parentPivot + parentTransform.translate;
    // -- 2 => flip parent rotation with parent flip
    const float local_parentFliped_parentRotation = G::snapRotation(G::flipRotation(parentTransform.rotation, parentFlip), snapAngle);
    // -- 3 => flip child rect in parent rect woth parent flip
    const H2DE_LevelRect local_parentFliped_childRect = G::flipRect(local_childRect, parentFlip);
    const H2DE_LevelRect world_parentFliped_childRect = local_parentFliped_childRect.addTranslate(parentTransform.translate);
    // -- 4 => flip child pivot in child rect with parent flip
    const H2DE_Pivot local_parentFliped_childPivot = G::flipPivot(world_parentRect, childTransform.pivot, parentFlip);
    // -- 5 => flip child rotation with parent flip
    const float local_parentFliped_childRotation = G::snapRotation(G::flipRotation(childTransform.rotation, parentFlip), snapAngle);

    // 2 => FLIP CHILD
    // -- 1 => flip child pivot in child rect with child flip
    const H2DE_Pivot local_childFliped_parentFliped_childPivot = G::flipPivot(local_parentFliped_childRect, local_parentFliped_childPivot, childFlip);
    const H2DE_Pivot world_childFliped_parentFliped_childPivot = local_childFliped_parentFliped_childPivot + world_parentFliped_childRect.getTranslate();
    // -- 2 => flip child rotation with child flip
    const float local_childFliped_parentFliped_childRotation = G::snapRotation(G::flipRotation(local_parentFliped_childRotation, childFlip), snapAngle);



    if (childTransform.scale.x == 0.8f) {
        std::cout << world_parentFliped_parentPivot << std::endl;
    }



    // 3 => ROTATE PARENT
    // -- 1 => rotate parent rect with parent pivot and parent rotation
    const H2DE_LevelRect world_parentRotated_parentRect = G::applyRotationOnRect(world_parentRect, world_parentFliped_parentPivot, local_parentFliped_parentRotation);
    // -- 2 => rotate child rect with parent pivot and parent rotation
    const H2DE_LevelRect world_parentRotated_parentFliped_childRect = G::applyRotationOnRect(world_parentFliped_childRect, world_parentFliped_parentPivot, local_parentFliped_parentRotation);
    // -- 3 => rotate child pivot with parent pivot and parent rotation
    const H2DE_Pivot world_parentRotated_childFliped_parentFliped_childPivot = G::applyRotationOnPivot(world_childFliped_parentFliped_childPivot, world_parentFliped_parentPivot, local_parentFliped_parentRotation);

    // 4 => ROTATE CHILD
    // -- 1 => rotate child rect with child pivot and child rotation
    const H2DE_LevelRect world_childRotated_parentRotated_parentFliped_childRect = G::applyRotationOnRect(world_parentRotated_parentFliped_childRect, world_parentRotated_childFliped_parentFliped_childPivot, local_childFliped_parentFliped_childRotation);

    return world_childRotated_parentRotated_parentFliped_childRect;
}

// ROTATION
float H2DE_Geometry::getRotation(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle) {
    const H2DE_Transform parentTransform = object->getTransform();
    const H2DE_Transform childTransform = transform;
    
    H2DE_Flip parentFlip = G::getFlipFromScale(parentTransform.scale);
    H2DE_Flip childFlip = G::getFlipFromScale(childTransform.scale);

    const float local_parentFliped_parentRotation = G::snapRotation(G::flipRotation(parentTransform.rotation, parentFlip), snapAngle);
    const float local_parentFliped_childRotation = G::snapRotation(G::flipRotation(childTransform.rotation, parentFlip), snapAngle);
    const float local_childFliped_parentFliped_childRotation = G::snapRotation(G::flipRotation(local_parentFliped_childRotation, childFlip), snapAngle);

    return G::normalizeRotation(G::snapRotation(local_parentFliped_parentRotation + local_childFliped_parentFliped_childRotation, snapAngle));
}













// FLIP
H2DE_LevelRect H2DE_Geometry::flipRect(const H2DE_LevelRect& local_childRect, H2DE_Flip flip) {
    H2DE_LevelRect res = local_childRect;

    if (flip & H2DE_FLIP_X) {
        res.x *= -1;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y *= -1;
    }

    return res;
}

float H2DE_Geometry::flipRotation(float rotation, H2DE_Flip flip) {
    float res = rotation;

    switch (flip) {
        case H2DE_FLIP_X: res = -res; break;
        case H2DE_FLIP_Y: res = 180.0f - res; break;
        case H2DE_FLIP_XY: res = 180.0f + res; break;
        default: break;
    }

    return G::normalizeRotation(res + G::getRotationCausedByFlip(flip));
}

H2DE_Pivot H2DE_Geometry::flipPivot(const H2DE_LevelRect& world_parentRect, const H2DE_Pivot& local_pivot, H2DE_Flip flip) {
    H2DE_Pivot res = local_pivot;

    if (flip & H2DE_FLIP_X) {
        res.x *= -1;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y *= -1;
    }

    return res;
}

// -- getter
H2DE_Flip H2DE_Geometry::getFlipFromScale(const H2DE_Scale& scale) {
    H2DE_Flip res = H2DE_FLIP_NONE;

    if (scale.x < 0.0f) {
        res = static_cast<H2DE_Flip>(res | H2DE_FLIP_X);
    }

    if (scale.y < 0.0f) {
        res = static_cast<H2DE_Flip>(res | H2DE_FLIP_Y);
    }

    return res;
}

// -- operations
H2DE_Flip H2DE_Geometry::addFlip(H2DE_Flip flip1, H2DE_Flip flip2) {
    if (flip1 == flip2) {
        return H2DE_FLIP_NONE;
    }

    return static_cast<H2DE_Flip>(flip1 | flip2);
}

// ROTATE
H2DE_LevelRect H2DE_Geometry::applyRotationOnRect(const H2DE_LevelRect& world_rect, const H2DE_Pivot& world_pivot, float rotation) {
    if (rotation == 0.0f) {
        return world_rect;
    }

    const H2DE_Translate world_rectCenter = world_rect.getTranslate();
    const H2DE_Translate world_rotatedRectCenter = world_rectCenter.rotate(world_pivot, rotation);

    H2DE_Scale rectScale = world_rect.getScale();

    if (std::fmod(rotation, 180.0f) != 0.0f) {
        std::swap(rectScale.x, rectScale.y);
    }

    return world_rotatedRectCenter.makeRect(rectScale);
}

float H2DE_Geometry::normalizeRotation(float rotation) {
    while (rotation >= 360.0f) {
        rotation -= 360.0f;
    }

    while (rotation < 0.0f) {
        rotation += 360.0f;
    }

    return rotation;
}
