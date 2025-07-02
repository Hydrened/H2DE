#ifndef H2DE_GEOMETRY_H
#define H2DE_GEOMETRY_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Object;

enum H2DE_Flip {
    H2DE_FLIP_NONE  = 0b00,
    H2DE_FLIP_X     = 0b01,
    H2DE_FLIP_Y     = 0b10,
    H2DE_FLIP_XY    = 0b11,
};

class H2DE_Geometry {
public:
    inline static H2DE_LevelRect getHitboxRect(const H2DE_Object* object, const H2DE_Hitbox& hitbox, bool xIsInverted = false, bool yIsInverted = false) {
        return H2DE_Geometry::getRect(object, hitbox.transform, 90.0f, xIsInverted, yIsInverted, false);
    }
    inline static H2DE_LevelRect getSurfaceRect(const H2DE_Object* object, H2DE_Surface* surface, bool xIsInverted = false, bool yIsInverted = false) {
        return H2DE_Geometry::getRect(object, surface->getTransform(), 1.0f, xIsInverted, yIsInverted, surface->fromText);
    }

    inline static float getHitboxRotation(const H2DE_Object* object, const H2DE_Hitbox& hitbox) noexcept {
        return H2DE_Geometry::getRotation(object, hitbox.transform, 90.0f);
    }
    inline static float getSurfaceRotation(const H2DE_Object* object, H2DE_Surface* surface) noexcept {
        return H2DE_Geometry::getRotation(object, surface->getTransform(), 1.0f);
    }

    static H2DE_LevelRect flipRect(const H2DE_LevelRect& local_childRect, H2DE_Flip flip) noexcept;
    static float flipRotation(float rotation, H2DE_Flip flip) noexcept;
    static H2DE_Pivot flipPivot(const H2DE_LevelRect& world_parentRect, const H2DE_Pivot& local_pivot, H2DE_Flip flip) noexcept;
    static H2DE_Flip getFlipFromScale(const H2DE_Scale& scale) noexcept;
    static H2DE_Flip addFlip(H2DE_Flip flip1, H2DE_Flip flip2) noexcept;

    static H2DE_LevelRect applyRotationOnRect(const H2DE_LevelRect& world_rect, const H2DE_Pivot& world_pivot, float rotation, float snapAngle);
    inline static H2DE_Pivot applyRotationOnPivot(const H2DE_Pivot& world_defaultPivot, const H2DE_Pivot& world_pivot, float rotation) {
        return world_defaultPivot.rotate(world_pivot, rotation);
    }
    inline static float applyRotationOnRotation(float defaultRotation, float rotation) noexcept {
        return H2DE_Geometry::normalizeRotation(defaultRotation + rotation);
    }

    static float normalizeRotation(float rotation) noexcept;

    static constexpr float getRotationCausedByFlip(H2DE_Flip flip) noexcept {
        return ((flip == H2DE_FLIP_XY) ? 180.0f : 0.0f);
    }
    inline static float snapRotation(float rotation, float angle) noexcept {
        return H2DE::round(rotation / angle) * angle;
    }

private:
    static H2DE_LevelRect getRect(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle, bool xIsInverted, bool yIsInverted, bool childCantBeInverted);
    static float getRotation(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle) noexcept;
};

using G = H2DE_Geometry;

#endif
