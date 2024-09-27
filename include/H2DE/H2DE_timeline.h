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
    int duration;
    H2DE_TimelineEffect effect;
    std::function<void(float)> update;
    std::function<void()> completed;
    int index = 0;

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
    H2DE_Timeline(H2DE_Engine* engine, int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed);
    ~H2DE_Timeline();

    /**
     * Calls the update function with the next value from the timeline as a float parameter
     * 
     * \return true if it ticks
     * 
     * \since H2DE-1.0.9
     */
    bool tick();
};

/**
 * The type used to identify a timeline manager
 * 
 * \since H2DE-1.0.14
 */
class H2DE_TimelineManager {
private:
    std::vector<H2DE_Timeline*> timelines;

public:
    H2DE_TimelineManager();
    H2DE_TimelineManager(std::vector<H2DE_Timeline*> timelines);

    /**
     * Adds a timeline to a manager
     * 
     * \param manager a pointer to a manager
     * \param timeline a pointer to a timeline
     * 
     * \since H2DE-1.0.14
     */
    friend void H2DE_AddTimelineToManager(H2DE_TimelineManager* manager, H2DE_Timeline* timeline);

    /**
     * Ticks each timelines of a manager 
     * 
     * \param manager a pointer to a manager
     * 
     * \since H2DE-1.0.14
     */
    friend void H2DE_TickTimelineManager(H2DE_TimelineManager* manager);
};

#endif
