#ifndef H2DE_TIMELINE_MANAGER_H
#define H2DE_TIMELINE_MANAGER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_TimelineManager {
private:
    H2DE_Engine* engine;

    struct H2DE_Timeline {
        unsigned int current = 0;
        unsigned int duration;
        int loops;
        unsigned int currentLoop = 0;
        H2DE_Easing easing;
        std::function<void(float)> update;
        std::function<void()> completed;
        bool pauseSensitive;
    };

    std::unordered_map<int, H2DE_Timeline> timelines;
    unsigned int id = 0;

    H2DE_TimelineManager(H2DE_Engine* engine);
    ~H2DE_TimelineManager();

    void update();

    unsigned int create(unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive);
    void reset(unsigned int id);
    void stop(unsigned int id, bool callCompleted);

    friend class H2DE_Engine;
};

#endif
