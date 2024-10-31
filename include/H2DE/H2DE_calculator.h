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
     * Picks the `H2DE_GraphicObject` with the greater index
     * \param s1 a pointer to a `H2DE_GraphicObject`
     * \param s2 a pointer to a `H2DE_GraphicObject`
     * \return true if `s2` is greater than `s1` or false otherwise
     * \since H2DE-1.0.0
     */
    static bool isIndexGreater(const H2DE_GraphicObject* s1, const H2DE_GraphicObject* s2);

    /**
     * Converts a rotation in degress into radians 
     * \param rotation a float value corresponding to a rotation in degrees
     * \return a float value corresponding to a rotation in radians
     * \since H2DE-1.0.0
     */
    static float convertToRadians(float rotation);

    /**
     * Gets the width and height of a polygon
     * \param points a `SDL_Point` vector for each polygon point
     * \return the width and height of the polygon
     * \since H2DE-1.0.0
     */
    static H2DE_Size getPolygonSize(std::vector<SDL_Point> points);

    /**
     * Gets the width and height of a circle
     * \param radius the radius of the circle
     * \return the width and the heigth of the circle
     * \since H2DE-1.3.7
     */
    static H2DE_Size getCircleSize(int radius);

    /**
     * Gets the rotation origin absolute position of an object
     * \param pos absolute position of the object
     * \param origin relative origin of the object
     * \return the rotation origin absolute position
     * \since H2DE-1.3.5
     */ 
    static H2DE_Pos getRotationOrigin(H2DE_Pos pos, H2DE_Pos origin);

    /**
     * Gets the scale origin absolute position of an object
     * \param pos absolute position of the object
     * \param origin relative position of the origin
     * \return the scale origin absolute position
     * \since H2DE-1.3.5
     */ 
    static H2DE_Pos getScaleOrigin(H2DE_Pos pos, H2DE_Pos origin);

    /**
     * Gets the absolute position of a point according to the object's parents
     * \param g a pointer to a `H2DE_GraphicObject`
     * \return the absolute position of the object
     * \since H2DE-1.3.0
     */
    static H2DE_Pos getPosFromParents(H2DE_GraphicObject* g);

    /**
     * Gets the center absolute position of an object
     * \param pos absolute position of the object
     * \param size size of the object
     * \return the center absolute position
     * \since H2DE-1.3.5
     */
    static H2DE_Pos getCenter(H2DE_Pos pos, H2DE_Size size);

    /**
     * Gets the filped absolute position of an object
     * \param pos absolute position of the object
     * \param center center's absolute position of the object
     * \param filp flip of the object
     * \return the fliped absolute position
     * \since H2DE-1.3.5
     */
    static H2DE_Pos getFlipedPos(H2DE_Pos pos, H2DE_Pos center, SDL_RendererFlip flip);

    /**
     * Gets the rotated absolute position according to an absolute rotation origin and a rotation
     * \param pos absolute position of the object
     * \param rotationOrigin rotation origin absolute position of the object
     * \param rotation rotation of the object
     * \return the rotated absolute position
     * \since H2DE-1.0.0
     */
    static H2DE_Pos getRotatedPos(H2DE_Pos pos, H2DE_Pos rotationOrigin, float rotation);

    /**
     * Gets the scaled absolute position according to an absolute scale origin and a scale
     * \param pos absolute position of the object
     * \param size size of the object
     * \param scaleOrigin scale origin absolute position of the object
     * \param scale scale of the object
     * \return the scaled absolute position
     * \since H2DE-1.0.0
     */
    static H2DE_Pos getRescaledPos(H2DE_Pos pos, H2DE_Size size, H2DE_Pos origin, H2DE_Scale scale);


    /**
     * Adds flips
     * \param f1 first flip
     * \param f2 second flip
     * \return the addition of two flips
     * \since H2DE-1.3.7
     */
    static SDL_RendererFlip addFlip(SDL_RendererFlip f1, SDL_RendererFlip f2);
};

#endif
