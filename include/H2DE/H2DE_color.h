#ifndef H2DE_COLOR_H
#define H2DE_COLOR_H

#include <H2DE/H2DE_types.h>

/**
 * Adds a hue value to a rgb color
 * \param rgb a rgb color
 * \param hue hue to add
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_AddHueToRGB(H2DE_RGB rgb, float hue);
/**
 * Adds saturation to a rgb color
 * \param rgb a rgb color
 * \param saturation saturation to add
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_AddSaturationToRGB(H2DE_RGB rgb, float saturation);
/**
 * Adds value to a rgb color
 * \param rgb a rgb color
 * \param value value to add
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_AddValueToRGB(H2DE_RGB rgb, float value);

/**
 * Substracts a hue value to a rgb color
 * \param rgb a rgb color
 * \param hue hue to substract
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_SubstractHueToRGB(H2DE_RGB rgb, float hue);
/**
 * Substracts saturation to a rgb color
 * \param rgb a rgb color
 * \param saturation saturation to substract
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_SubstractSaturationToRGB(H2DE_RGB rgb, float saturation);
/**
 * Substracts value to a rgb color
 * \param rgb a rgb color
 * \param value value to substract
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_SubstractValueToRGB(H2DE_RGB rgb, float value);

/**
 * Multiplies hue value to a rgb color
 * \param rgb a rgb color
 * \param multiplier hue multiplier
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_MultiplyRGBHue(H2DE_RGB rgb, float multiplier);
/**
 * Multiplies saturation to a rgb color
 * \param rgb a rgb color
 * \param multiplier saturation multiplier
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_MultiplyRGBSaturation(H2DE_RGB rgb, float multiplier);
/**
 * Multiplies value to a rgb color
 * \param rgb a rgb color
 * \param multiplier value multiplier
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_MultiplyRGBValue(H2DE_RGB rgb, float multiplier);

/**
 * Divides hue value to a rgb color
 * \param rgb a rgb color
 * \param divider hue divider
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_DivideRGBHue(H2DE_RGB rgb, float divider);
/**
 * Divides saturation to a rgb color
 * \param rgb a rgb color
 * \param divider saturation divider
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_DivideRGBSaturation(H2DE_RGB rgb, float divider);
/**
 * Divides value to a rgb color
 * \param rgb a rgb color
 * \param divider value divider
 * \since H2DE-1.2.0
 */
H2DE_RGB H2DE_DivideRGBValue(H2DE_RGB rgb, float divider);

#endif
