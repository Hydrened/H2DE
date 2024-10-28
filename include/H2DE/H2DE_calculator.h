#ifndef H2DE_POSITIONS_H
#define H2DE_POSITIONS_H

#include <H2DE/H2DE_types.h>
#include <H2DE/H2DE_graphic.h>
#include <cmath>

/**
 * Contains the methods required by the engine to calculate positions or rotations
 * \since H2DE-1.0.0
 */
class H2DE_Calculator {
public:
    /**
     * Pick the `H2DE_GraphicObject` with the greater index
     * \param s1 a pointer to a `H2DE_GraphicObject`
     * \param s2 a pointer to a `H2DE_GraphicObject`
     * \return true if `s2` is greater than `s1` or false otherwise
     * \since H2DE-1.0.0
     */
    static bool isIndexGreater(const H2DE_GraphicObject* s1, const H2DE_GraphicObject* s2);
    /**
     * Convert a rotation in degress into radians 
     * \param rotation a float value corresponding to a rotation in degrees
     * \return a float value corresponding to a rotation in radians
     * \since H2DE-1.0.0
     */
    static float convertToRadians(float rotation);

    /**
     * Get the width and height of a polygon
     * \param points a `SDL_Point` vector for each polygon point
     * \return the width and height of the polygon
     * \since H2DE-1.0.0
     */
    static H2DE_Size getPolygonSize(std::vector<SDL_Point> points);
    /**
     * Calculates the position of a point according to the object's parents
     * \param g a pointer to a `H2DE_GraphicObject`
     * \return the position
     * \since H2DE-1.3.0
     */
    static H2DE_Pos getPosFromParents(H2DE_GraphicObject* g);

    /**
     * \since H2DE-1.3.5
     */
    static H2DE_Pos getCenter(H2DE_Pos pos, H2DE_Size size, H2DE_Scale scale);

    /**
     * \since H2DE-1.3.5
     */
    static H2DE_Pos getFlipedPos(H2DE_Pos pos, H2DE_Pos center, SDL_RendererFlip flip);

    /**
     * \since H2DE-1.3.5
     */ 
    static H2DE_Pos getRotationOrigin(H2DE_Pos pos, H2DE_Pos origin);

    /**
     * \since H2DE-1.3.5
     */ 
    static H2DE_Pos getScaleOrigin(H2DE_Pos pos, H2DE_Pos origin);

    /**
     * \since H2DE-1.0.0
     */
    static H2DE_Pos getRotatedPos(H2DE_Pos pos, H2DE_Pos rotationOrigin, float rotation);

    /**
     * \since H2DE-1.0.0
     */
    static H2DE_Pos getRescaledPos(H2DE_Pos pos, H2DE_Size size, H2DE_Pos origin, H2DE_Scale scale);






    /**
     * Calculates the position of a point according to a scale and a scale origin
     * \param pos default position
     * \param size element size
     * \param scaleOrigin position of the scale point
     * \param scale scale multiplier
     * \return the rescaled position
     * \since H2DE-1.0.0
     */
    static H2DE_Pos OLDgetRescaledPos(H2DE_Pos pos, H2DE_Size size, H2DE_Pos scaleOrigin, H2DE_Scale scale);
    /**
     * Calculates the size of an element according to a scale
     * \param size default size
     * \param scale scale multiplier
     * \return the rescaled size
     * \since H2DE-1.0.0
     */
    static H2DE_Size OLDgetRescaledSize(H2DE_Size size, H2DE_Scale scale);
    /**
     * Calculates the position of the rotation origin according to a scale
     * \param rotationOrigin position of the rotation origin
     * \param scale scale multiplier
     * \return the rescaled rotation origin position
     * \since H2DE-1.0.0
     */
    static H2DE_Pos OLDgetRescaledRotationOrigin(H2DE_Pos rotationOrigin, H2DE_Scale scale);
    /**
     * Calculates the position of a point according to a rotation origin and a rotation
     * \param rotationOrigin position of the rotation origin
     * \param rotation rotation
     * \return the rotated position of the point
     * \since H2DE-1.0.0
     */
    static H2DE_Pos OLDgetRotatedPos(H2DE_Pos pos, H2DE_Pos rotationOrigin, float rotation);
    /**
     * Calculates the position of a point according to a flip origin
     * \param pos position of the point to flip
     * \param objPos position of the object
     * \param size size of the object
     * \param flip flip
     * \return the fliped position
     * \since H2DE-1.3.5
     */
    static H2DE_Pos OLDgetFlipedPos(H2DE_Pos pos, H2DE_Pos objPos, H2DE_Size size, SDL_RendererFlip flip);















};

#endif
