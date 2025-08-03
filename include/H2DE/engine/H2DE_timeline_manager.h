#pragma once

#include <H2DE/engine/H2DE_engine.h>
#include <H2DE/engine/H2DE_timeline.h>
class H2DE_Engine;

/**
 * @class H2DE_Delay
 * @brief Represents a one-shot time delay or timeout handler.
 * 
 * The H2DE_Delay class provides a simple mechanism for executing a callback 
 * after a specified duration. It is useful for scheduling one-time events, 
 * timeouts, or delayed actions without interpolation or animation.
 * 
 * Features include:
 * 
 * - Configurable delay duration.
 * - Pause, resume, reset, and stop control.
 * - Callback execution upon completion.
 * 
 * Delays are updated by the H2DE_TimelineManager and are automatically 
 * removed once completed or manually stopped.
 */
using H2DE_Delay = H2DE_Timeline;

class H2DE_TimelineManager {
private:
    H2DE_Engine* engine;

    std::vector<H2DE_Timeline*> timelines;

    H2DE_TimelineManager(H2DE_Engine* engine);
    ~H2DE_TimelineManager();

    std::vector<H2DE_Timeline*>::iterator destroyTimeline(H2DE_Timeline* timeline, bool callCompleted);

    void update();

    H2DE_Timeline* create(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);

    bool isStoped(H2DE_Timeline* timeline) const;

    friend class H2DE_Engine;
    friend class H2DE_Timeline;
    friend class H2DE_Object;
};
