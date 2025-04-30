#ifndef H2DE_BASIC_OBJECT_H
#define H2DE_BASIC_OBJECT_H

/**
 * @file H2DE_basic_object.h
 * @brief Header file for the `H2DE_BasicObject` class.
 * 
 * This file contains the declaration of the `H2DE_BasicObject` class, which is
 * used to manage basic object types within the H2DE engine. These objects can 
 * have surfaces and can interact with them through various functions such as 
 * adding or removing surfaces.
 * 
 * The class inherits from `H2DE_Object`, which provides common object 
 * functionality such as position, size, and rendering, while the basic object
 * focuses on the management of surface-related data.
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @brief A basic object class within the H2DE engine.
 * 
 * This class is a specialization of `H2DE_Object`, specifically designed
 * for objects that can have associated surfaces. It provides functionality to
 * manage these surfaces, including adding, removing, and retrieving them.
 * 
 * Instances of this class should be created through the `H2DE_CreateBasicObject`
 * function and should be destroyed using standard object management practices.
 */
class H2DE_BasicObject : public H2DE_Object {
private:
    H2DE_BasicObjectData bod;

    H2DE_BasicObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BasicObjectData bod);
    ~H2DE_BasicObject() override;

    void resetSurfaceBuffers() override;

public:
    /**
     * @brief Creates a basic object.
     * 
     * This function creates and initializes a new `H2DE_BasicObject` with the provided
     * engine, object data, and basic object-specific data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param objectData The general object data for initialization.
     * @param basicObjectData The specific data for the basic object.
     * @return A pointer to the created `H2DE_BasicObject`.
     */
    friend H2DE_BasicObject* H2DE_CreateBasicObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BasicObjectData& basicObjectData);
    /**
     * @brief Retrieves a surface from the basic object.
     * 
     * This function retrieves a surface associated with the `H2DE_BasicObject`,
     * identified by the given name.
     * 
     * @param basicObject A pointer to the `H2DE_BasicObject`.
     * @param name The name of the surface to retrieve.
     * @return A pointer to the requested surface.
     */
    friend H2DE_Surface* H2DE_GetBasicObjectSurface(const H2DE_BasicObject* basicObject, const std::string& name);

    /**
     * @brief Adds a surface to the basic object.
     * 
     * This function adds a surface to the `H2DE_BasicObject`, identified by the
     * provided name.
     * 
     * @param basicObject A pointer to the `H2DE_BasicObject`.
     * @param surface A pointer to the surface to be added.
     * @param name The name to associate with the surface.
     */
    friend void H2DE_AddSurfaceToBasicObject(H2DE_BasicObject* basicObject, H2DE_Surface* surface, const std::string& name);
    /**
     * @brief Removes a surface from the basic object.
     * 
     * This function removes a surface from the `H2DE_BasicObject`, identified by
     * the given name.
     * 
     * @param basicObject A pointer to the `H2DE_BasicObject`.
     * @param name The name of the surface to remove.
     */
    friend void H2DE_RemoveSurfaceFromBasicObject(H2DE_BasicObject* basicObject, const std::string& name);
};

#endif
