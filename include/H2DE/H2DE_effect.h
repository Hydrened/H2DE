#ifndef H2DE_EFFECT_H
#define H2DE_EFFECT_H

#include <SDL2/SDL_types.h>
#include <H2DE/H2DE_types.h>
#include <cmath>
#include <vector>

/**
 * Contains the methods to make effects
 * 
 * \since H2DE-1.0.0
 */
class H2DE_Effect {
public:
    /**
     * Obtain the number of steps required for a given duration
     * 
     * \param fps the fps the window is running at
     * \param ms the duration
     * 
     * \return the number of steps
     * 
     * \since H2DE-1.0.0
     */
    static int getSteps(int fps, int ms);
    /**
     * Get an int linear effect from a point a to b
     * 
     * \param start the starting point
     * \param end the ending point
     * \param steps the length of the vector
     * 
     * \return a vector with the transition from the point a to b
     * 
     * \since H2DE-1.0.0
     */
    static std::vector<int> linear(int start, int end, int steps);
    /**
     * Get a flaot linear effect from a point a to b
     * 
     * \param start the starting point
     * \param end the ending point
     * \param steps the length of the vector
     * 
     * \return a vector with the transition from the point a to b
     * 
     * \since H2DE-1.0.0
     */
    static std::vector<float> linear(float start, float end, int steps);
    /**
     * Get a float ease in out effect from a point a to b
     * 
     * \param start the starting point
     * \param end the ending point
     * \param steps the length of the vector
     * 
     * \return a vector with the transition from the point a to b
     * 
     * \since H2DE-1.0.0
     */
    static std::vector<float> easeInOut(float start, float end, int steps);
};

#endif
