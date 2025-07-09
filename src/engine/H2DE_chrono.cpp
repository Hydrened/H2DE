#include "H2DE/engine/H2DE_chrono_manager.h"

#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Chrono::H2DE_Chrono(H2DE_Engine* e, H2DE_ChronoManager* m, const H2DE_Time& s, bool i, bool p) : engine(e), manager(m), current(s), increasing(i), pauseSensitive(p) {

}

// UPDATE
void H2DE_Chrono::update() {
    updateCurrentTime();
    updateOnReachEvents();
}

void H2DE_Chrono::updateCurrentTime() {
    if (pauseSensitive && engine->isPaused()) {
        return;
    }

    if (paused) {
        return;
    }

    int deltaTimeMultiplier = (increasing) ? 1 : -1;
    
    float elapsed = current.toElapsed();
    elapsed += engine->getDeltaTime() * deltaTimeMultiplier;
    elapsed = std::max(elapsed, 0.0f);
    current = H2DE_Time::toTime(elapsed);
}

void H2DE_Chrono::updateOnReachEvents() {
    float epsilon = 0.5f / engine->getFPS();
    float elapsed = current.toElapsed();

    for (auto it = onReachEvents.begin(); it != onReachEvents.end(); ) {
        float elapsedDifferenceWithTarget = H2DE::abs(it->target.toElapsed() - elapsed);

        if (elapsedDifferenceWithTarget <= epsilon) {
            it->callback();

            if (it->once) {
                it = onReachEvents.erase(it);

            } else {
                ++it;
            }

        } else {
            ++it;
        }
    }
}

// ACTIONS
void H2DE_Chrono::onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once) {
    if (!callback) {
        H2DE_Error::logWarning("On reach function does not have a callback function.");
        return;
    }

    H2DE_Chrono::H2DE_OnReachEvent onReachEvent = H2DE_Chrono::H2DE_OnReachEvent();
    onReachEvent.target = target;
    onReachEvent.callback = callback;
    onReachEvent.once = once;
    onReachEvents.push_back(onReachEvent);
}

void H2DE_Chrono::stop() noexcept {
    manager->destroyChrono(this); 
}
