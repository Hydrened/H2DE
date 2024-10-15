#ifndef H2DE_TIMELINE_H
#define H2DE_TIMELINE_H

#include <H2DE/H2DE.h>
#include <cmath>
#include <functional>

class H2DE_Engine;

/**
 * The type used for a timeline
 * \since H2DE-1.0.9
 */
class H2DE_Timeline {
private:
    H2DE_Engine* engine;
    unsigned int duration;
    H2DE_TimelineEffect effect;
    std::function<void(float)> update;
    std::function<void()> completed;
    int index = 0;
    int loop;

    /**
     * Obtains the number of steps required for a given duration
     * \param ms the duration in ms
     * \return the number of steps
     * \since H2DE-1.0.0
     */
    unsigned int getSteps(unsigned int ms);

public:
    H2DE_Timeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loop);
    ~H2DE_Timeline();

    /**
     * Calls the update function with the next value from the timeline as a float parameter
     * \param timeline a pointer to a timeline
     * \return true if it ticks
     * \since H2DE-1.0.9
     */
    friend bool H2DE_TickTimeline(H2DE_Timeline* timeline);
    /**
     * Restarts the current loop
     * \param timeline a pointer to a timeline
     * \since H2DE-1.2.1
     */
    friend void H2DE_ResetTimeline(H2DE_Timeline* timeline);
};

/**
 * The type used to identify a timeline manager
 * \since H2DE-1.0.14
 */
class H2DE_TimelineManager {
private:
    std::vector<H2DE_Timeline*> timelines;

public:
    H2DE_TimelineManager();

    /**
     * Adds a timeline to a manager
     * \param manager a pointer to a manager
     * \param timeline a pointer to a timeline
     * \since H2DE-1.0.14
     */
    friend void H2DE_AddTimelineToManager(H2DE_TimelineManager* manager, H2DE_Timeline* timeline);

    /**
     * Removes a timeline from a manager
     * \param manager a pointer to a manager
     * \param timeline a reference to a timeline
     * \since H2DE-1.0.17
     */
    friend void H2DE_RemoveTimelineFromManager(H2DE_TimelineManager* manager, H2DE_Timeline* timeline);

    /**
     * Ticks each timelines of a manager 
     * \param manager a pointer to a manager
     * \since H2DE-1.0.14
     */
    friend void H2DE_TickTimelineManager(H2DE_TimelineManager* manager);

    /**
     * Clears each timeline of a manager
     * \param manager a pointer to a manager
     * \since H2DE-1.0.15
     */
    friend void H2DE_ClearTimelineManager(H2DE_TimelineManager* manager);

    /**
     * Resets each timeline of a manager
     * \param manager a pointer to a manager
     * \since H2DE-1.2.1
     */
    friend void H2DE_ResetTimelineManager(H2DE_TimelineManager* manager);

    /**
     * Destroys a timeline manager
     * \param timelineManager a pointer to a timeline manager
     * \since H2DE-1.1.4
     */
    friend void H2DE_DestroyTimelineManager(H2DE_TimelineManager* timelineManager);
};

/**
 * Creates a timeline manager
 * \return a pointer to a timeline manager
 * \since H2DE-1.1.4
 */
extern H2DE_TimelineManager* H2DE_CreateTimelineManager();

#endif
