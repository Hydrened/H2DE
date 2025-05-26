#include "H2DE/H2DE_geometry.h"

// HITBOX
H2DE_LevelRect H2DE_Geometry::getRect(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle) {
    const H2DE_Transform objTransform = object->getTransform();
    const H2DE_Transform hitTransform = transform;

    const H2DE_LevelRect world_objRect = objTransform.translate.makeRect(objTransform.scale);
    const H2DE_LevelRect local_hitRect = hitTransform.translate.makeRect(hitTransform.scale);

    G::H2DE_Flip objFlip = G::getFlipFromScale(objTransform.scale);
    G::H2DE_Flip hitFlip = G::getFlipFromScale(hitTransform.scale);
    G::H2DE_Flip addedFlips = G::addFlip(objFlip, hitFlip);

    const H2DE_Pivot local_objFliped_objPivot = G::flipPivot(world_objRect, objTransform.pivot, objFlip);
    const H2DE_Pivot world_objFliped_objPivot = local_objFliped_objPivot + objTransform.translate;
    float objFliped_objRotation = G::snapRotation(G::flipRotation(objTransform.rotation, objFlip), snapAngle);

    const H2DE_LevelRect local_objFliped_hitRect = G::flipRect(world_objRect, local_hitRect, objFlip);
    const H2DE_LevelRect world_objFliped_hitRect = local_objFliped_hitRect.addTranslate(objTransform.translate);
    const H2DE_Pivot local_objFliped_hitPivot = G::flipPivot(local_hitRect, hitTransform.pivot, objFlip);
    float objFliped_hitRotation = G::snapRotation(G::flipRotation(hitTransform.rotation, objFlip), snapAngle);

    const H2DE_Pivot local_hitFliped_objFliped_hitPivot = G::flipPivot(local_hitRect, local_objFliped_hitPivot, hitFlip);
    const H2DE_Pivot world_hitFliped_objFliped_hitPivot = local_hitFliped_objFliped_hitPivot + objTransform.translate;
    float hitFliped_objFliped_hitRotation = G::flipRotation(objFliped_hitRotation, hitFlip);

    const H2DE_LevelRect world_objRotated_objFliped_hitRect = G::applyRotationOnRect(world_objFliped_hitRect, world_objFliped_objPivot, objFliped_objRotation);
    const H2DE_Pivot world_objRotated_hitFliped_objFliped_hitPivot = G::applyRotationOnPivot(world_hitFliped_objFliped_hitPivot, world_objFliped_objPivot, objFliped_objRotation);

    return G::applyRotationOnRect(world_objRotated_objFliped_hitRect, world_objRotated_hitFliped_objFliped_hitPivot, hitFliped_objFliped_hitRotation);
}

// FLIP
H2DE_LevelRect H2DE_Geometry::flipRect(const H2DE_LevelRect& world_parentRect, const H2DE_LevelRect& local_childRect, G::H2DE_Flip flip) {
    H2DE_LevelRect res = local_childRect;

    if (flip & H2DE_FLIP_X) {
        res.x = -res.x;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = -res.y;
    }

    return res;
}

float H2DE_Geometry::flipRotation(float rotation, G::H2DE_Flip flip) {
    float res = rotation;

    switch (flip) {
        case H2DE_FLIP_X: res = -res; break;
        case H2DE_FLIP_Y: res = 180.0f - res; break;
        case H2DE_FLIP_XY: res = 180.0f + res; break;
        default: break;
    }

    return G::normalizeRotation(res);
}

H2DE_Pivot H2DE_Geometry::flipPivot(const H2DE_LevelRect& world_parentRect, const H2DE_Pivot& local_pivot, G::H2DE_Flip flip) {
    H2DE_Pivot res = local_pivot;

    if (flip & H2DE_FLIP_X) {
        res.x = -local_pivot.x;
    }

    if (flip & H2DE_FLIP_Y) {
        res.y = -local_pivot.y;
    }

    return res;
}

// -- getter
G::H2DE_Flip H2DE_Geometry::getFlipFromScale(const H2DE_Scale& scale) {
    G::H2DE_Flip res = H2DE_FLIP_NONE;

    if (scale.x < 0.0f) {
        res = static_cast<G::H2DE_Flip>(res | H2DE_FLIP_X);
    }

    if (scale.y < 0.0f) {
        res = static_cast<G::H2DE_Flip>(res | H2DE_FLIP_Y);
    }

    return res;
}

// -- operations
G::H2DE_Flip H2DE_Geometry::addFlip(G::H2DE_Flip flip1, G::H2DE_Flip flip2) {
    if (flip1 == flip2) {
        return H2DE_FLIP_NONE;
    }

    return static_cast<G::H2DE_Flip>(flip1 | flip2);
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
