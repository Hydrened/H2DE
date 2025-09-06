#pragma once

/**
 * @file H2DE_basic_object.h
 * @brief Defines H2DE_BasicObject, a simple single-surface object.
 * 
 * This file contains the H2DE_BasicObject class, which inherits from H2DE_SingleSurfaceObject
 * and represents a minimal object with only one surface.
 * 
 * It is suitable for static or simple game objects that do not require additional functionality
 * like text, dual surfaces, or animations.
 * 
 * @note Surface management is handled by the inherited H2DE_SingleSurfaceObject functions.
 * @note This object is typically used for basic visual elements or placeholders.
 */

#include <H2DE/objects/parents/H2DE_single_surface_object.h>

/**
 * @class H2DE_BasicObject
 * @brief A minimal object with a single drawable surface.
 * 
 * H2DE_BasicObject provides:
 * - A single surface for rendering (texture, sprite, or color)
 * - Basic object behavior inherited from H2DE_SingleSurfaceObject
 * 
 * This class is intended for simple, static, or placeholder objects in the game world.
 */
class H2DE_BasicObject : public H2DE_SingleSurfaceObject {
private:
    H2DE_BasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData) : H2DE_Object(engine, objectData), H2DE_SingleSurfaceObject(engine, objectData) {}
    ~H2DE_BasicObject() override = default;

    void _refreshSurfaceBuffers() override;

    friend class H2DE_Engine;
};
