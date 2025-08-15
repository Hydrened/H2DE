#include "H2DE/engine/H2DE_timeline_manager.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_TimelineManager::H2DE_TimelineManager(H2DE_Engine* e) : engine(e) {
    timelines.reserve(1000);
}

// CLEANUP
H2DE_TimelineManager::~H2DE_TimelineManager() {
    for (H2DE_Timeline* timeline : timelines) {
        delete timeline;
    }
}

// UPDATE
void H2DE_TimelineManager::update() {
    for (auto it = timelines.begin(); it != timelines.end(); ) {
        H2DE_Timeline* timeline = *it;

        if (!timeline->_update()) {
            it = destroyTimeline(timeline, false);
            
        } else {
            ++it;
        }
    }
}

// ACTIONS
H2DE_Timeline* H2DE_TimelineManager::create(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive) {
    float fixedDuration = H2DE::max(static_cast<float>(duration) + engine->getDeltaTime() + 1, 1.0f);
    H2DE_Timeline* timeline = new H2DE_Timeline(engine, this, fixedDuration, easing, update, completed, loops, pauseSensitive);
    timelines.push_back(timeline);
    return timeline;
}

std::vector<H2DE_Timeline*>::iterator H2DE_TimelineManager::destroyTimeline(H2DE_Timeline* timeline, bool callCompleted) {
    auto it = std::find(timelines.begin(), timelines.end(), timeline);

    if (it == timelines.end()) {
        return timelines.end();
    }

    H2DE_Timeline* t = *it;

    if (callCompleted) {
        if (t->_updateCall) {
            t->_updateCall(1.0f);
        }

        if (t->_completedCall) {
            t->_completedCall();
        }
    }

    delete t;
    return timelines.erase(it);
}

// GETTER
bool H2DE_TimelineManager::isStoped(H2DE_Timeline* timeline) const {
    if (timeline == nullptr) {
        return true;
    }

    const auto it = std::find(timelines.begin(), timelines.end(), timeline);
    return (it == timelines.end());
}
