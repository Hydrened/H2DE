#ifndef H2DE_BAR_OBJECT_H
#define H2DE_BAR_OBJECT_H

/**
 * @file H2DE_bar_object.h
 * @brief Defines the `H2DE_BarObject` class for creating and manipulating bar objects.
 * 
 * This file includes functions to modify the properties of bar objects, such as
 * setting the value, minimum, and maximum range. It also provides functions to
 * manage the surfaces of the bar's front and background. The `H2DE_BarObject`
 * class is derived from `H2DE_Object`, allowing it to inherit general object
 * functionality.
 */

#include <H2DE/objects/H2DE_object.h>

/**
 * @brief A class representing a bar object in the H2DE engine.
 * 
 * This class manages a bar object, typically used to display progress or values
 * with visual representation. It allows control over the value, minimum, and
 * maximum range, as well as adding or removing surfaces for the bar's front
 * and background.
 * 
 * Instances of this class are created using `H2DE_CreateBarObject` and can be
 * customized through various setter functions.
 */
class H2DE_BarObject : public H2DE_Object {
private:
    H2DE_BarObjectData bod;
    float percentage = 0.0f;
    float value;

    H2DE_BarObject(H2DE_Engine* engine, H2DE_ObjectData od, H2DE_BarObjectData bod);
    ~H2DE_BarObject() override;

    void refreshPercentage();
    void resetSurfaceBuffers() override;

public:
    /**
     * @brief Creates a bar object.
     * 
     * This function creates and initializes a new `H2DE_BarObject` with the given
     * engine, object data, and bar-specific data.
     * 
     * @param engine A pointer to the H2DE engine instance.
     * @param objectData The general object data for initialization.
     * @param barObjectData The specific data for the bar object.
     * @return A pointer to the created `H2DE_BarObject`.
     */
    friend H2DE_BarObject* H2DE_CreateBarObject(H2DE_Engine* engine, const H2DE_ObjectData& objectData, const H2DE_BarObjectData& barObjectData);

    /**
     * @brief Gets the front surface of the bar.
     * 
     * This function retrieves the front surface of the bar object, identified by
     * the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param name The name of the front surface.
     * @return A pointer to the surface of the bar's front.
     */
    friend H2DE_Surface* H2DE_GetBarFrontSurface(const H2DE_BarObject* bar, const std::string& name);
    /**
     * @brief Gets the background surface of the bar.
     * 
     * This function retrieves the background surface of the bar object, identified
     * by the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param name The name of the background surface.
     * @return A pointer to the surface of the bar's background.
     */
    friend H2DE_Surface* H2DE_GetBarBackgroundSurface(const H2DE_BarObject* bar, const std::string& name);
    /**
     * @brief Adds a surface to the front of the bar.
     * 
     * This function adds a surface to the front part of the bar, identified by
     * the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param surface A pointer to the surface to be added.
     * @param name The name of the surface.
     */
    friend void H2DE_AddSurfaceToBarObjectFront(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name);
    /**
     * @brief Adds a surface to the background of the bar.
     * 
     * This function adds a surface to the background part of the bar, identified
     * by the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param surface A pointer to the surface to be added.
     * @param name The name of the surface.
     */
    friend void H2DE_AddSurfaceToBarObjectBackground(H2DE_BarObject* bar, H2DE_Surface* surface, const std::string& name);
    /**
     * @brief Removes a surface from the front of the bar.
     * 
     * This function removes a surface from the front part of the bar, identified
     * by the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param name The name of the surface to be removed.
     */
    friend void H2DE_RemoveSurfaceFromBarObjectFront(H2DE_BarObject* bar, const std::string& name);
    /**
     * @brief Removes a surface from the background of the bar.
     * 
     * This function removes a surface from the background part of the bar,
     * identified by the given name.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param name The name of the surface to be removed.
     */
    friend void H2DE_RemoveSurfaceFromBarObjectBackground(H2DE_BarObject* bar, const std::string& name);

    /**
     * @brief Sets the value of the bar.
     * 
     * This function sets the value of the bar object, updating the visual
     * representation accordingly.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param value The new value to set for the bar.
     */
    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value);
    /**
     * @brief Sets the value of the bar with animation.
     * 
     * This function sets the value of the bar object, updating the visual
     * representation with an optional easing animation.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param value The new value to set for the bar.
     * @param duration The duration of the animation.
     * @param easing The easing type to use for the animation.
     * @param pauseSensitive If the animation should be sensitive to pause states.
     */
    friend void H2DE_SetBarValue(H2DE_BarObject* bar, float value, unsigned int duration, H2DE_Easing easing, bool pauseSensitive);
    /**
     * @brief Sets the minimum value of the bar.
     * 
     * This function sets the minimum value of the bar, which defines the lower
     * bound for the bar's value.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param min The minimum value to set for the bar.
     */
    friend void H2DE_SetBarMin(H2DE_BarObject* bar, float min);
    /**
     * @brief Sets the maximum value of the bar.
     * 
     * This function sets the maximum value of the bar, which defines the upper
     * bound for the bar's value.
     * 
     * @param bar A pointer to the `H2DE_BarObject`.
     * @param max The maximum value to set for the bar.
     */
    friend void H2DE_SetBarMax(H2DE_BarObject* bar, float max);
};

#endif
