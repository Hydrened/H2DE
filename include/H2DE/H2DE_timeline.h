#ifndef H2DE_TIMELINE_H
#define H2DE_TIMELINE_H

#include <H2DE_engine.h>

/**
 * Type used to identify a timeline
 * \since H2DE-1.0.9
 */
class H2DE_Timeline {
private:
    unsigned int duration;
    H2DE_TimelineEffect effect;
    std::function<void(float)> update;
    std::function<void()> completed;
    int loops = 0;

    int frame = 0;
    int loopSave = 0;
    bool finished = false;

public:
    H2DE_Timeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loops = 1);
    ~H2DE_Timeline();

    /**
     * Ticks a timeline
     * \param timeline pointer to a timeline
     * \returns true of false whether it ticks or not
     * \since H2DE-1.0.9
     */
    friend bool H2DE_TickTimeline(H2DE_Timeline* timeline);

    /**
     * Restarts a timeline
     * \param timeline pointer to a timeline
     * \since H2DE-1.2.1
     */
    friend void H2DE_ResetTimeline(H2DE_Timeline* timeline);
};

/**
 * Creates a timeline
 * \param engine pointer to an engine
 * \param duration duration in ms
 * \param effect timeline effect
 * \param update function called every frame
 * \param completed function called once the timeline is finished
 * \param loops number of loop
 * \returns a pointer to a timeline
 * \since H2DE-1.0.9
 */
extern H2DE_Timeline* H2DE_CreateTimeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loops = 1);

#endif
