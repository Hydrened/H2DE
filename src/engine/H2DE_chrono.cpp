#include "H2DE/engine/H2DE_chrono_manager.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Chrono::H2DE_Chrono(H2DE_Engine* e, H2DE_ChronoManager* m, const H2DE_Time& s, bool i, bool p) : _engine(e), _manager(m), _current(s), _increasing(i), _pauseSensitive(p) {
    _current = H2DE_Time::toTime(H2DE::max(_current.toElapsed(), 0.0f));
}

// UPDATE
void H2DE_Chrono::_update() {
    _updateCurrentTime();
    _updateOnReachEvents();
}

void H2DE_Chrono::_updateCurrentTime() {
    if (_pauseSensitive && _engine->isPaused()) {
        return;
    }

    if (_paused) {
        return;
    }

    int deltaTimeMultiplier = (_increasing) ? 1 : -1;
    
    float elapsed = _current.toElapsed();
    elapsed += _engine->getDeltaTime() * deltaTimeMultiplier;
    elapsed = H2DE::max(elapsed, 0.0f);
    _current = H2DE_Time::toTime(elapsed);
}

void H2DE_Chrono::_updateOnReachEvents() {
    float epsilon = 0.5f / _engine->getFPS();
    float elapsed = _current.toElapsed();

    for (auto it = _onReachEvents.begin(); it != _onReachEvents.end(); ) {
        float elapsedDifferenceWithTarget = H2DE::abs(it->target.toElapsed() - elapsed);

        if (elapsedDifferenceWithTarget <= epsilon) {
            it->callback();

            if (it->once) {
                it = _onReachEvents.erase(it);

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

    H2DE_Chrono::_H2DE_OnReachEvent onReachEvent = H2DE_Chrono::_H2DE_OnReachEvent();
    onReachEvent.target = target;
    onReachEvent.callback = callback;
    onReachEvent.once = once;
    _onReachEvents.push_back(onReachEvent);
}

void H2DE_Chrono::stop() noexcept {
    _manager->destroyChrono(this); 
}
