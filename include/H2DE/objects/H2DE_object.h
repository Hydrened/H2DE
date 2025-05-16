#ifndef H2DE_OBJECT_H
#define H2DE_OBJECT_H

/**
 * @file H2DE_object.h
 * @brief Declaration of the H2DE_Object class and its associated functionalities.
 * 
 * This file contains the declaration of the `H2DE_Object` class, which represents a generic object in the H2DE game engine. 
 * Objects can be of various types (basic, buttons, bars and text), and this class provides common methods to manipulate 
 * position, size, hitboxes, rotation, visibility, and other properties related to objects.
 * 
 * The `H2DE_Object` class serves as a parent class for other specific object types in the engine. The methods include handling 
 * hitboxes, transformations (position, size, rotation), and states (such as visibility and index changes).
 * 
 * The following functionalities are provided by this file:
 * - Manipulation of position, size, and rotation of objects.
 * - Adding, removing, and managing hitboxes associated with an object.
 * - Control of an object's visibility.
 * - Management of animated transformations (e.g., position or size changing over time with an easing function).
 * 
 * This file is an integral part of the H2DE 2D engine and allows developers to create, manipulate, and interact with objects in the game world.
 */

#include <sstream>
#include <H2DE/H2DE_engine.h>
class H2DE_Engine;

/**
 * @brief The base class for all objects in the H2DE engine.
 * 
 * This class defines essential properties and methods for managing objects in the 2D game world,
 * including position, size, rotation, flip state, and hitboxes.
 * It serves as the foundation for more specific object types like bar objects, button objects, and more.
 * 
 * Instances of this class should be manipulated through specific derived object classes.
 */
class H2DE_Object {
private:
    bool hidden = false;

    void update();
    void updateCollision();
    void snap(const H2DE_LevelRect& rect, const H2DE_LevelRect& otherRect, H2DE_Face face);

    std::vector<H2DE_SurfaceBuffer> getSurfaceBuffers() const;
    H2DE_Hitbox& getHitbox(const std::string& hitboxName);

protected:
    H2DE_Engine* engine;
    H2DE_ObjectData od;

    std::vector<H2DE_SurfaceBuffer> surfaceBuffers = {};
    
    H2DE_Object(H2DE_Engine* engine, H2DE_ObjectData od);
    virtual ~H2DE_Object();

    static void destroySurfaces(std::unordered_map<std::string, H2DE_Surface*>& surfaces);
    static H2DE_Surface* getSurface(const std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

    virtual void resetSurfaceBuffers() = 0;
    inline void clearSurfaceBuffers() { surfaceBuffers.clear(); }

    static const std::vector<H2DE_Surface*> getSortedSurfaces(const std::unordered_map<std::string, H2DE_Surface*>& surfaces);

    void addSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, H2DE_Surface* surface, const std::string& name);
    void removeSurface(std::unordered_map<std::string, H2DE_Surface*>& surfaces, const std::string& name);

public:
    /**
	 * @brief Destroys a specified object in the engine.
	 * 
	 * This function removes the object from the engine, freeing its resources.
	 * After calling this function, the object should not be used again.
	 * 
	 * @param engine A pointer to the H2DE engine instance.
	 * @param object A pointer to the object to destroy.
	 */
    friend void H2DE_DestroyObject(H2DE_Engine* engine, H2DE_Object* object);

    /**
	 * @brief Adds a hitbox to a specified object.
	 * 
	 * This function associates a hitbox with the object, allowing for collision detection.
	 * A unique name can be assigned to the hitbox to easily identify and manage it.
	 * 
	 * @param object A pointer to the object to which the hitbox will be added.
	 * @param hitbox The hitbox to add to the object.
	 * @param hitboxName The name to assign to the hitbox for identification.
	 */
    friend void H2DE_AddHitboxToObject(H2DE_Object* object, const H2DE_Hitbox& hitbox, const std::string& hitboxName);
    /**
	 * @brief Removes a hitbox from a specified object.
	 * 
	 * This function removes the hitbox from the object, disabling any collision detection
	 * associated with that hitbox. The hitbox is identified by its name.
	 * 
	 * @param object A pointer to the object from which the hitbox will be removed.
	 * @param hitboxName The name of the hitbox to remove from the object.
	 */
    friend void H2DE_RemoveHitboxFromObject(H2DE_Object* object, const std::string& hitboxName);

    /**
	 * @brief Retrieves the position of the object.
	 * 
	 * This function returns the current position of the object in the game world.
	 * 
	 * @param object A pointer to the object whose position is being retrieved.
	 * @return The position of the object as an `H2DE_LevelPos` structure.
	 */
    friend H2DE_LevelPos H2DE_GetObjectPos(const H2DE_Object* object);
    /**
	 * @brief Retrieves the size of the object.
	 * 
	 * This function returns the current size of the object.
	 * 
	 * @param object A pointer to the object whose size is being retrieved.
	 * @return The size of the object as an `H2DE_LevelSize` structure.
	 */
    friend H2DE_LevelSize H2DE_GetObjectSize(const H2DE_Object* object);
    /**
	 * @brief Retrieves the bounding rectangle of the object.
	 * 
	 * This function returns the bounding rectangle of the object, which includes its position and size.
	 * 
	 * @param object A pointer to the object whose bounding rectangle is being retrieved.
	 * @return The bounding rectangle of the object as an `H2DE_LevelRect` structure.
	 */
    friend H2DE_LevelRect H2DE_GetObjectRect(const H2DE_Object* object);
    /**
	 * @brief Retrieves the pivot point of the object.
	 * 
	 * This function returns the pivot point of the object, which is used for transformations like rotation.
	 * 
	 * @param object A pointer to the object whose pivot point is being retrieved.
	 * @return The pivot point of the object as an `H2DE_LevelPos` structure.
	 */
    friend H2DE_LevelPos H2DE_GetObjectPivot(const H2DE_Object* object);
    /**
	 * @brief Retrieves the rotation angle of the object.
	 * 
	 * This function returns the current rotation angle of the object in degrees.
	 * 
	 * @param object A pointer to the object whose rotation angle is being retrieved.
	 * @return The rotation angle of the object as a float.
	 */
    friend float H2DE_GetObjectRotation(const H2DE_Object* object);
    /**
	 * @brief Retrieves the flip status of the object.
	 * 
	 * This function returns whether the object is flipped horizontally or vertically.
	 * 
	 * @param object A pointer to the object whose flip status is being retrieved.
	 * @return The flip status of the object as an `H2DE_Flip` enum value.
	 */
    friend H2DE_Flip H2DE_GetObjectFlip(const H2DE_Object* object);
    /**
	 * @brief Retrieves the hitboxes of the object.
	 * 
	 * This function returns a map of all the hitboxes associated with the object, where the key is the hitbox name
	 * and the value is the corresponding `H2DE_Hitbox` structure.
	 * 
	 * @param object A pointer to the object whose hitboxes are being retrieved.
	 * @return A map of hitboxes associated with the object, where the key is the hitbox name.
	 */
    friend std::unordered_map<std::string, H2DE_Hitbox> H2DE_GetObjectHitboxes(const H2DE_Object* object);
	/**
	 * @brief Retrieves a specific hitbox of the object by name.
	 * 
	 * This function returns the hitbox associated with the specified name from the object. If the hitbox does not exist,
	 * the behavior is undefined and may result in an exception or invalid data. It is recommended to verify that the
	 * hitbox exists before calling this function.
	 * 
	 * @param object A pointer to the object containing the hitboxes.
	 * @param name The name of the hitbox to retrieve.
	 * @return The `H2DE_Hitbox` associated with the given name.
	 */
	friend H2DE_Hitbox H2DE_GetObjectHitbox(const H2DE_Object* object, const std::string& name);
	/**
	 * @brief Retrieves the world-space rectangle of a specific hitbox.
	 * 
	 * This function returns the rectangle representing the position and size of the specified hitbox in world coordinates.
	 * It takes into account the object's current position, as well as any transformations applied to the hitbox,
	 * such as flipping or rotation.
	 * 
	 * @param object A pointer to the object containing the hitbox.
	 * @param name The name of the hitbox to retrieve.
	 * @return The `H2DE_LevelRect` representing the world-space rectangle of the hitbox.
	 */
	friend H2DE_LevelRect H2DE_GetObjectHitboxWorldRect(const H2DE_Object* object, const std::string& name);
    /**
	 * @brief Retrieves the index of the object.
	 * 
	 * This function returns the index of the object in the object container or scene, if applicable.
	 * 
	 * @param object A pointer to the object whose index is being retrieved.
	 * @return The index of the object as an integer.
	 */
    friend int H2DE_GetObjectIndex(const H2DE_Object* object);
    /**
	 * @brief Checks if the object is absolute.
	 * 
	 * This function checks whether the object's position is absolute or relative, determining if it's affected
	 * by the camera or not.
	 * 
	 * @param object A pointer to the object being checked.
	 * @return `true` if the object is absolute, `false` otherwise.
	 */
    friend bool H2DE_IsObjectAbsolute(const H2DE_Object* object);
    /**
	 * @brief Checks if the object is hidden.
	 * 
	 * This function checks whether the object is currently hidden (invisible) or not.
	 * 
	 * @param object A pointer to the object being checked.
	 * @return `true` if the object is hidden, `false` otherwise.
	 */
    friend bool H2DE_IsObjectHidden(const H2DE_Object* object);

    /**
	 * @brief Sets the position of the object.
	 * 
	 * This function updates the position of the object in the game world.
	 * 
	 * @param object A pointer to the object whose position is being updated.
	 * @param pos The new position to set for the object as an `H2DE_LevelPos` structure.
	 */
    friend void H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos);
    /**
	 * @brief Sets the position of the object with an optional duration, easing, and pause sensitivity.
	 * 
	 * This function updates the position of the object in the game world, with the ability to animate the transition
	 * over a specified duration using an easing function, and optionally, make it pause-sensitive.
	 * 
	 * @param object A pointer to the object whose position is being updated.
	 * @param pos The new position to set for the object as an `H2DE_LevelPos` structure.
	 * @param duration The duration of the transition in milliseconds.
	 * @param easing The easing function to use for the transition.
     * @param completed The callback called when the timeline ends
	 * @param pauseSensitive Whether the transition should be pause-sensitive.
     * @return The ID of the created timeline.
	 */
    friend unsigned int H2DE_SetObjectPos(H2DE_Object* object, const H2DE_LevelPos& pos, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
	 * @brief Sets the size of the object.
	 * 
	 * This function updates the size of the object.
	 * 
	 * @param object A pointer to the object whose size is being updated.
	 * @param size The new size to set for the object as an `H2DE_LevelSize` structure.
	 */
    friend void H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size);
    /**
	 * @brief Sets the size of the object with an optional duration, easing, and pause sensitivity.
	 * 
	 * This function updates the size of the object, with the ability to animate the transition
	 * over a specified duration using an easing function, and optionally, make it pause-sensitive.
	 * 
	 * @param object A pointer to the object whose size is being updated.
	 * @param size The new size to set for the object as an `H2DE_LevelSize` structure.
	 * @param duration The duration of the transition in milliseconds.
	 * @param easing The easing function to use for the transition.
     * @param completed The callback called when the timeline ends
	 * @param pauseSensitive Whether the transition should be pause-sensitive.
     * @return The ID of the created timeline.
	 */
    friend unsigned int H2DE_SetObjectSize(H2DE_Object* object, const H2DE_LevelSize& size, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
	 * @brief Sets the bounding rectangle of the object.
	 * 
	 * This function updates the bounding rectangle of the object, which includes its position and size.
	 * 
	 * @param object A pointer to the object whose bounding rectangle is being updated.
	 * @param rect The new bounding rectangle to set for the object as an `H2DE_LevelRect` structure.
	 */
    friend void H2DE_SetObjectRect(H2DE_Object* object, const H2DE_LevelRect& rect);
    /**
	 * @brief Sets the bounding rectangle of the object with an optional duration, easing, and pause sensitivity.
	 * 
	 * This function updates the bounding rectangle of the object, with the ability to animate the transition
	 * over a specified duration using an easing function, and optionally, make it pause-sensitive.
	 * 
	 * @param object A pointer to the object whose bounding rectangle is being updated.
	 * @param rect The new bounding rectangle to set for the object as an `H2DE_LevelRect` structure.
	 * @param duration The duration of the transition in milliseconds.
	 * @param easing The easing function to use for the transition.
     * @param completed The callback called when the timeline ends
	 * @param pauseSensitive Whether the transition should be pause-sensitive.
     * @return The ID of the created timeline.
	 */
    friend unsigned int H2DE_SetObjectRect(H2DE_Object* object, const H2DE_LevelRect& rect, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
	 * @brief Sets the index of the object.
	 * 
	 * This function updates the index of the object in the object container or scene, if applicable.
	 * 
	 * @param object A pointer to the object whose index is being updated.
	 * @param index The new index to set for the object.
	 */
    friend void H2DE_SetObjectIndex(H2DE_Object* object, int index);
    /**
	 * @brief Sets the pivot point of the object.
	 * 
	 * This function updates the pivot point of the object, which is used for transformations like rotation.
	 * 
	 * @param object A pointer to the object whose pivot point is being updated.
	 * @param pivot The new pivot point to set for the object as an `H2DE_LevelPos` structure.
	 */
    friend void H2DE_SetObjectPivot(H2DE_Object* object, const H2DE_LevelPos& pivot);
    /**
	 * @brief Sets the rotation angle of the object.
	 * 
	 * This function updates the rotation angle of the object in degrees.
	 * 
	 * @param object A pointer to the object whose rotation angle is being updated.
	 * @param rotation The new rotation angle to set for the object, in degrees.
	 */
    friend void H2DE_SetObjectRotation(H2DE_Object* object, float rotation);
    /**
	 * @brief Sets the rotation angle of the object with an optional duration, easing, and pause sensitivity.
	 * 
	 * This function updates the rotation angle of the object, with the ability to animate the transition
	 * over a specified duration using an easing function, and optionally, make it pause-sensitive.
	 * 
	 * @param object A pointer to the object whose rotation angle is being updated.
	 * @param rotation The new rotation angle to set for the object, in degrees.
	 * @param duration The duration of the transition in milliseconds.
	 * @param easing The easing function to use for the transition.
     * @param completed The callback called when the timeline ends
	 * @param pauseSensitive Whether the transition should be pause-sensitive.
     * @return The ID of the created timeline.
	 */
    friend unsigned int H2DE_SetObjectRotation(H2DE_Object* object, float rotation, unsigned int duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive);
    /**
	 * @brief Sets the flip status of the object.
	 * 
	 * This function updates the flip status of the object, determining if it is flipped horizontally or vertically.
	 * 
	 * @param object A pointer to the object whose flip status is being updated.
	 * @param flip The new flip status to set for the object as an `H2DE_Flip` enum value.
	 */
    friend void H2DE_SetObjectFlip(H2DE_Object* object, H2DE_Flip flip);
    /**
	 * @brief Sets the bounding rectangle of a specific hitbox for the object.
	 * 
	 * This function updates the bounding rectangle of a specific hitbox associated with the object.
	 * 
	 * @param object A pointer to the object whose hitbox is being updated.
	 * @param hitboxName The name of the hitbox to update.
	 * @param rect The new bounding rectangle to set for the hitbox as an `H2DE_LevelRect` structure.
	 */
    friend void H2DE_SetObjectHitboxRect(H2DE_Object* object, const std::string& hitboxName, const H2DE_LevelRect& rect);
    /**
	 * @brief Sets the color of a specific hitbox for the object.
	 * 
	 * This function updates the color of a specific hitbox associated with the object.
	 * 
	 * @param object A pointer to the object whose hitbox color is being updated.
	 * @param hitboxName The name of the hitbox to update.
	 * @param color The new color to set for the hitbox as an `H2DE_ColorRGB` structure.
	 */
    friend void H2DE_SetObjectHitboxColor(H2DE_Object* object, const std::string& hitboxName, const H2DE_ColorRGB& color);
    /**
	 * @brief Sets the collision index of a specific hitbox for the object.
	 * 
	 * This function updates the collision index of a specific hitbox associated with the object.
	 * 
	 * @param object A pointer to the object whose hitbox collision index is being updated.
	 * @param hitboxName The name of the hitbox to update.
	 * @param index The new collision index to set for the hitbox.
	 */
    friend void H2DE_SetObjectHitboxCollisionIndex(H2DE_Object* object, const std::string& hitboxName, int index);
    /**
	 * @brief Sets the snap status of a specific hitbox for the object.
	 * 
	 * This function updates the snap status of a specific hitbox associated with the object.
	 * 
	 * @param object A pointer to the object whose hitbox snap status is being updated.
	 * @param hitboxName The name of the hitbox to update.
	 * @param snap The new snap status to set for the hitbox.
	 */
    friend void H2DE_SetObjectHitboxSnap(H2DE_Object* object, const std::string& hitboxName, bool snap);
    /**
	 * @brief Sets the callback function to be called when a specific hitbox of the object collides.
	 * 
	 * This function sets a callback that is triggered when a specific hitbox of the object collides with another object.
	 * 
	 * @param object A pointer to the object whose hitbox collision callback is being set.
	 * @param hitboxName The name of the hitbox to set the collision callback for.
	 * @param call The callback function to be called when the hitbox collides.
	 */
    friend void H2DE_SetObjectHitboxOnCollide(H2DE_Object* object, const std::string& hitboxName, const std::function<void(H2DE_Object*, H2DE_Face)>& call);
    /**
	 * @brief Makes the object visible.
	 * 
	 * This function makes the object visible in the game world if it was previously hidden.
	 * 
	 * @param object A pointer to the object to make visible.
	 */
    friend void H2DE_ShowObject(H2DE_Object* object);
    /**
	 * @brief Hides the object.
	 * 
	 * This function hides the object in the game world, making it invisible.
	 * 
	 * @param object A pointer to the object to hide.
	 */
    friend void H2DE_HideObject(H2DE_Object* object);

    friend bool H2DE_CameraContainsObject(const H2DE_Engine* engine, const H2DE_Object* object);

    friend class H2DE_Engine;
};

#include <H2DE/objects/H2DE_bar_object.h>
#include <H2DE/objects/H2DE_basic_object.h>
#include <H2DE/objects/H2DE_button_object.h>
#include <H2DE/objects/H2DE_text_object.h>

#endif
