#include "H2DE/engine/H2DE_timeline.h"
#include "H2DE/engine/H2DE_engine.h"

// INIT
H2DE_Timeline::H2DE_Timeline(H2DE_Engine* en, H2DE_TimelineManager* m, uint32_t d, H2DE_Easing e, const std::function<void(float)>& u, const std::function<void()>& c, uint32_t l, bool p) : _engine(en), _manager(m) {
    _duration = d;
    _easing = e;
    _updateCall = u;
    _completedCall = c;
    _loops = l;
    _pauseSensitive = p;
}

// UPDATE
bool H2DE_Timeline::_update() {
    if ((_pauseSensitive && _engine->isPaused()) || (_paused)) {
        return true;
    }

    _current += _engine->getDeltaTime() * 1000.0f;

    if (_updateCall) {
        float blend = H2DE::clamp(_current, 0.0f, _duration) / _duration;
        blend = H2DE::lerp(0.0f, 1.0f, blend, _easing);
        _updateCall(blend);
    }

    if (_current >= _duration) {
        if (_completedCall) {
            _completedCall();
        }

        _currentLoop++;
        _current = 0.0f;

        if (_loops == H2DE_INFINITE_LOOP) {
            return true;
        }

        if (_currentLoop >= _loops) {
            return false;
        } 
    } 

    return true;
}

// ACTIONS
void H2DE_Timeline::reset() noexcept {
    _current = 0.0f;
    _currentLoop = 0;
}

void H2DE_Timeline::stop(bool callCompleted) noexcept {
    _manager->destroyTimeline(this, callCompleted);
}
