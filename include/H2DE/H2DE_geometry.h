#ifndef H2DE_GEOMETRY_H
#define H2DE_GEOMETRY_H

#include <H2DE/H2DE_engine.h>
class H2DE_Object;

class H2DE_Geometry {
private:
    enum H2DE_Flip {
        H2DE_FLIP_NONE  = 0b00,
        H2DE_FLIP_X     = 0b01,
        H2DE_FLIP_Y     = 0b10,
        H2DE_FLIP_XY    = 0b11,
    };

    static H2DE_LevelRect getRect(const H2DE_Object* object, const H2DE_Transform& transform, float snapAngle);

public:
    inline static H2DE_LevelRect getHitboxRect(const H2DE_Object* object, const H2DE_Hitbox& hitbox) {
        return H2DE_Geometry::getRect(object, hitbox.transform, 90.0f);
    }
    inline static H2DE_LevelRect getSurfaceRect(const H2DE_Object* object, const H2DE_Transform& transform) {
        return H2DE_Geometry::getRect(object, transform, 1.0f);
    }

    static H2DE_Geometry::H2DE_Flip getFlipFromScale(const H2DE_Scale& scale);
    static inline float getRotationCausedByFlip(H2DE_Geometry::H2DE_Flip flip) {
        return ((flip == H2DE_FLIP_XY) ? 180.0f : 0.0f);
    }

    static H2DE_LevelRect flipRect(const H2DE_LevelRect& world_parentRect, const H2DE_LevelRect& local_childRect, H2DE_Geometry::H2DE_Flip flip);
    static float flipRotation(float rotation, H2DE_Geometry::H2DE_Flip flip);
    static H2DE_Pivot flipPivot(const H2DE_LevelRect& world_parentRect, const H2DE_Pivot& local_pivot, H2DE_Geometry::H2DE_Flip flip);

    static H2DE_Geometry::H2DE_Flip addFlip(H2DE_Geometry::H2DE_Flip flip1, H2DE_Geometry::H2DE_Flip flip2);

    static H2DE_LevelRect applyRotationOnRect(const H2DE_LevelRect& world_rect, const H2DE_Pivot& world_pivot, float rotation);
    inline static H2DE_Pivot applyRotationOnPivot(const H2DE_Pivot& world_defaultPivot, const H2DE_Pivot& world_pivot, float rotation) {
        return world_defaultPivot.rotate(world_pivot, rotation);
    }
    inline static float applyRotationOnRotation(float defaultRotation, float rotation) {
        return H2DE_Geometry::normalizeRotation(defaultRotation + rotation);
    }
    
    static float normalizeRotation(float rotation);
    inline static float snapRotation(float rotation, float angle) {
        return std::round(rotation  / angle) * angle;
    }
};

using G = H2DE_Geometry;

#endif
