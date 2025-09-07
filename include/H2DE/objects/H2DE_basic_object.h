#pragma once

/**
 * @file H2DE_basic_object.h
 * @brief Minimal object with a single surface and no special behavior.
 * 
 * This file defines the H2DE_BasicObject class, which extends
 * H2DE_SingleSurfaceObject to provide a basic drawable entity.
 * 
 * H2DE_BasicObject serves as the simplest object type in the
 * engine, intended for cases where only a single surface needs
 * to be displayed without additional logic or interactivity.
 * 
 * It is useful for static graphics, placeholders, or lightweight
 * elements that don’t require text, toggling, or advanced behavior.
 */

#include <H2DE/objects/parents/H2DE_single_surface_object.h>

/**
 * @class H2DE_BasicObject
 * @brief Simple drawable object with a single surface.
 * 
 * H2DE_BasicObject is the most lightweight implementation of
 * an object in the engine. It inherits rendering capabilities
 * from H2DE_SingleSurfaceObject but does not introduce any
 * extra features.
 * 
 * Responsibilities:
 * - Store and render surfaces
 * - Refresh its surface buffer when necessary
 * 
 * Typical use cases:
 * - Static images or sprites
 * - Background or decorative elements
 * - Placeholder objects for testing
 * 
 * This class is a final layer object and can be instantiated
 * directly when no advanced behavior is required.
 */
class H2DE_BasicObject : public H2DE_SingleSurfaceObject {
private:
    H2DE_BasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData) : H2DE_Object(engine, objectData), H2DE_SingleSurfaceObject(engine, objectData) {}
    ~H2DE_BasicObject() override = default;

    void _refreshSurfaceBuffers() override;

    friend class H2DE_Engine;
};
