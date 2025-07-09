#ifndef H2DE_TIMELINE_H
#define H2DE_TIMELINE_H

#include <H2DE/utils/H2DE_utils.h>
class H2DE_Engine;
class H2DE_TimelineManager;

class H2DE_Timeline {
public:
    inline void pause() noexcept { paused = true; };
    inline void resume() noexcept { paused = false; };
    inline void togglePause() noexcept { paused = !paused; };
    void reset() noexcept;
    void stop(bool callCompleted) noexcept;

    constexpr bool isPaused() const noexcept { return paused; }
    
private:
    H2DE_Engine* engine;
    H2DE_TimelineManager* manager;

    float current = 0.0f;
    float duration = 0.0f;
    uint32_t loops = 0;
    uint32_t currentLoop = 0;
    H2DE_Easing easing = H2DE_EASING_LINEAR;
    std::function<void(float)> updateCall = nullptr;
    std::function<void()> completedCall = nullptr;
    bool pauseSensitive = true;
    bool paused = false;

    H2DE_Timeline(H2DE_Engine* engine, H2DE_TimelineManager* manager, uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive = true);
    ~H2DE_Timeline() noexcept = default;

    bool update();

    friend class H2DE_TimelineManager;
};

#endif
