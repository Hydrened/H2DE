#include "H2DE/engine/H2DE_geometry.h"

// GETTERS
H2DE_LevelRect H2DE_Geometry::getRect(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle, bool xIsInverted, bool yIsInverted) {
    H2DE_Transform parentTransform = object->getTransform();
    H2DE_Transform childTransform = transform;

    if (xIsInverted) {
        parentTransform.translate.x *= -1;
        parentTransform.pivot.x *= -1;
        childTransform.translate.x *= -1;
        childTransform.pivot.x *= -1;
    }

    if (yIsInverted) {
        parentTransform.translate.y *= -1;
        parentTransform.pivot.y *= -1;
        childTransform.translate.y *= -1;
        childTransform.pivot.y *= -1;
    }
    
    H2DE_LevelRect world_parentRect = parentTransform.translate.makeRect(parentTransform.scale);
    world_parentRect.w = std::abs(world_parentRect.w);
    world_parentRect.h = std::abs(world_parentRect.h);

    H2DE_LevelRect local_childRect = childTransform.translate.makeRect(childTransform.scale);
    local_childRect.w = std::abs(local_childRect.w);
    local_childRect.h = std::abs(local_childRect.h);

    H2DE_Flip parentFlip = G::getFlipFromScale(parentTransform.scale);
    H2DE_Flip childFlip = G::getFlipFromScale(childTransform.scale);

    const H2DE_Pivot local_parentFliped_parentPivot = G::flipPivot(world_parentRect, parentTransform.pivot, parentFlip);
    const H2DE_Pivot world_parentFliped_parentPivot = local_parentFliped_parentPivot + parentTransform.translate;
    const float local_parentFliped_parentRotation = G::snapRotation(G::flipRotation(parentTransform.rotation, parentFlip), snapAngle);
    const H2DE_LevelRect local_parentFliped_childRect = G::flipRect(local_childRect, parentFlip);
    const H2DE_LevelRect world_parentFliped_childRect = local_parentFliped_childRect.addTranslate(parentTransform.translate);
    const H2DE_Pivot local_parentFliped_childPivot = G::flipPivot(local_childRect, childTransform.pivot, parentFlip);
    const float local_parentFliped_childRotation = G::snapRotation(G::flipRotation(childTransform.rotation, parentFlip), snapAngle);
    const H2DE_LevelRect world_flipRotated_parentRect = G::applyRotationOnRect(world_parentRect, { 0.0f, 0.0f }, G::getRotationCausedByFlip(parentFlip), snapAngle);

    const H2DE_Pivot local_childFliped_parentFliped_childPivot = G::flipPivot(world_parentFliped_childRect, local_parentFliped_childPivot, childFlip);
    const H2DE_Pivot world_childFliped_parentFliped_childPivot = local_childFliped_parentFliped_childPivot + world_parentFliped_childRect.getTranslate();
    const float local_childFliped_parentFliped_childRotation = G::snapRotation(G::flipRotation(local_parentFliped_childRotation, childFlip), snapAngle);

    const H2DE_LevelRect world_parentRotated_parentRect = G::applyRotationOnRect(world_flipRotated_parentRect, world_parentFliped_parentPivot, local_parentFliped_parentRotation, snapAngle);
    const H2DE_LevelRect world_parentRotated_parentFliped_childRect = G::applyRotationOnRect(world_parentFliped_childRect, world_parentFliped_parentPivot, local_parentFliped_parentRotation, snapAngle);
    const H2DE_Pivot world_parentRotated_childFliped_parentFliped_childPivot = G::applyRotationOnPivot(world_childFliped_parentFliped_childPivot, world_parentFliped_parentPivot, local_parentFliped_parentRotation);

    const H2DE_LevelRect world_childRotated_parentRotated_parentFliped_childRect = G::applyRotationOnRect(world_parentRotated_parentFliped_childRect, world_parentRotated_childFliped_parentFliped_childPivot, local_childFliped_parentFliped_childRotation, snapAngle);

    return world_childRotated_parentRotated_parentFliped_childRect;
}

float H2DE_Geometry::getRotation(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle) {
    const H2DE_Transform parentTransform = object->getTransform();
    const H2DE_Transform childTransform = transform;
    
    H2DE_Flip parentFlip = G::getFlipFromScale(parentTransform.scale);
    H2DE_Flip childFlip = G::getFlipFromScale(childTransform.scale);
    H2DE_Flip addedFlips = G::addFlip(parentFlip, childFlip);

    const float local_parentFliped_parentRotation = G::snapRotation(G::flipRotation(parentTransform.rotation, parentFlip), snapAngle);
    const float local_parentFliped_childRotation = G::snapRotation(G::flipRotation(childTransform.rotation, parentFlip), snapAngle);
    const float local_childFliped_parentFliped_childRotation = G::snapRotation(G::flipRotation(local_parentFliped_childRotation, childFlip), snapAngle);

    const float rotationCausedByFlip = G::getRotationCausedByFlip(addedFlips);
    const float world_rotation = local_parentFliped_parentRotation + local_childFliped_parentFliped_childRotation + rotationCausedByFlip;

    return G::normalizeRotation(G::snapRotation(world_rotation, snapAngle));
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

    if (flip == H2DE_FLIP_X || flip == H2DE_FLIP_Y) {
        res *= -1;
    }

    return G::normalizeRotation(res);
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
H2DE_LevelRect H2DE_Geometry::applyRotationOnRect(const H2DE_LevelRect& world_rect, const H2DE_Pivot& world_pivot, float rotation, float snapAngle) {
    if (rotation == 0.0f) {
        return world_rect;
    }

    const H2DE_Translate world_rectTranslate = world_rect.getTranslate();
    const H2DE_Translate world_rotatedRectCenter = world_rectTranslate.rotate(world_pivot, rotation);

    H2DE_Scale rectScale = world_rect.getScale();

    if (std::fmod(snapAngle, 90.0f) == 0.0f) {
        if (std::fmod(rotation, 180.0f) != 0.0f) {
            std::swap(rectScale.x, rectScale.y);
        }
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
