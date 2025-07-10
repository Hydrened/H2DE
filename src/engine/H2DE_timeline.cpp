#include "H2DE/engine/H2DE_timeline.h"

#include "H2DE/engine/H2DE_engine.h"

// INIT
H2DE_Timeline::H2DE_Timeline(H2DE_Engine* en, H2DE_TimelineManager* m, uint32_t d, H2DE_Easing e, const std::function<void(float)>& u, const std::function<void()>& c, uint32_t l, bool p) : engine(en), manager(m) {
    duration = d;
    updateCall = u;
    completedCall = c;
    loops = l;
    pauseSensitive = p;
}

// UPDATE
bool H2DE_Timeline::update() {
    if ((pauseSensitive && engine->isPaused()) || (paused)) {
        return true;
    }

    current += engine->getDeltaTime() * 1000.0f;

    if (updateCall) {
        float blend = H2DE::clamp(current, 0.0f, duration) / duration;
        blend = H2DE::lerp(0.0f, 1.0f, blend, easing);
        updateCall(blend);
    }

    if (current >= duration) {
        if (completedCall) {
            completedCall();
        }

        currentLoop++;
        current = 0.0f;

        if (loops == H2DE_INFINITE_LOOP) {
            return true;
        }

        if (currentLoop >= loops) {
            return false;
        } 
    } 

    return true;
}

// ACTIONS
void H2DE_Timeline::reset() noexcept {
    current = 0.0f;
    currentLoop = 0;
}

void H2DE_Timeline::stop(bool callCompleted) noexcept {
    manager->destroyTimeline(this, callCompleted); 
}
