#ifndef H2DE_TIMELINE__H
#define H2DE_TIMELINE__H

#include <H2DE/H2DE_engine.h>

#undef max

class H2DE_Engine::H2DE_Timeline {
private:
    H2DE_Engine* engine;

    struct SpecificTimeline {
        unsigned int current = 0;
        unsigned int max;
        int loops;
        unsigned int currentLoop = 0;
        H2DE_Easing easing;
        std::function<void(float)> update;
        std::function<void()> completed;
        bool pauseSensitive;
    };

    std::unordered_map<int, SpecificTimeline> timelines;
    unsigned int id = 0;

public:
    H2DE_Timeline(H2DE_Engine* engine);
    ~H2DE_Timeline();

    void update();

    friend unsigned int H2DE_CreateTimeline(const H2DE_Engine* engine, unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive);
    friend void H2DE_ResetTimeline(const H2DE_Engine* engine, unsigned int id);
    friend void H2DE_StopTimeline(const H2DE_Engine* engine, unsigned int id, bool call);
};

#endif
