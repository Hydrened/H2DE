#ifndef H2DE_TIMELINE_H
#define H2DE_TIMELINE_H

#include <H2DE/H2DE.h>
#include <cmath>
#include <functional>

class H2DE_Engine;

/**
 * The type used to identify a timeline
 * 
 * \since H2DE-1.0.9
 */
class H2DE_Timeline {
private:
    H2DE_Engine* engine;
    float start;
    float end;
    int duration;
    H2DE_TimelineEffect effect;
    std::function<void(float)> update;
    std::function<void()> completed;
    int index = 0;

    /**
     * Gets a value betwwen two values from a blend value
     * 
     * \param a the minimum value
     * \param b the maximum value
     * \param blend a value between 0 and 1
     * 
     * \return a value between `a` and `b`
     * 
     * \since H2DE-1.0.9
     */
    static float lerp(float a, float b, float blend);
    /**
     * Obtains the number of steps required for a given duration
     * 
     * \param ms the duration in ms
     * 
     * \return the number of steps
     * 
     * \since H2DE-1.0.0
     */
    int getSteps(int ms);

public:
    H2DE_Timeline(H2DE_Engine* engine, float start, float end, int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed);
    ~H2DE_Timeline();

    /**
     * Calls the update function with the next value from the timeline as parameter
     * Notes: Ticks one time when the `H2DE_Engine::render()` method is called
     * 
     * \return true if it ticks
     * 
     * \since H2DE-1.0.9
     */
    bool tick();
};

#endif
