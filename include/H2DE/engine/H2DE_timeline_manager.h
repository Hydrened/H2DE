#ifndef H2DE_TIMELINE_MANAGER_H
#define H2DE_TIMELINE_MANAGER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_TimelineManager {
private:
    H2DE_Engine* engine;

    struct H2DE_Timeline {
        float current = 0.0f;
        float duration;
        uint32_t loops;
        uint32_t currentLoop = 0;
        H2DE_Easing easing;
        std::function<void(float)> update;
        std::function<void()> completed;
        bool pauseSensitive;
        bool paused = false;
    };

    std::unordered_map<H2DE_TimelineID, H2DE_Timeline> timelines;
    H2DE_TimelineID id = 0;

    H2DE_TimelineManager(H2DE_Engine* engine);
    ~H2DE_TimelineManager() noexcept = default;

    void update();

    H2DE_TimelineID create(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);
    void pause(H2DE_TimelineID id);
    void resume(H2DE_TimelineID id);
    void togglePause(H2DE_TimelineID id);
    void reset(H2DE_TimelineID id);
    void stop(H2DE_TimelineID id, bool callCompleted);
    bool isPaused(H2DE_TimelineID id) const;
    bool isStoped(H2DE_TimelineID id) const;

    friend class H2DE_Engine;
};

#endif
