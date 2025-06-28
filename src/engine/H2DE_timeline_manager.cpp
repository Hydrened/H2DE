#include "H2DE/engine/H2DE_timeline_manager.h"

// INIT
H2DE_TimelineManager::H2DE_TimelineManager(H2DE_Engine* e) : engine(e) {
    timelines.reserve(1000);
}

// UPDATE
void H2DE_TimelineManager::update() {
    for (auto it = timelines.begin(); it != timelines.end(); ) {
        auto& [timelineId, timeline] = *it;
    
        if ((timeline.pauseSensitive && engine->isPaused()) || (timeline.paused)) {
            ++it;
            continue;
        }

        timeline.current += engine->getDeltaTime() * 1000.0f;

        if (timeline.update) {
            float blend = std::clamp(timeline.current, 0.0f, timeline.duration) / timeline.duration;
            blend = H2DE::lerp(0.0f, 1.0f, blend, timeline.easing);
            timeline.update(blend);
        }

        if (timeline.current >= timeline.duration) {
            if (timeline.completed) {
                timeline.completed();
            }

            timeline.currentLoop++;
            timeline.current = 0.0f;

            if (timeline.loops == H2DE_INFINITE_LOOP) {
                ++it;
                continue;
            }

            if (timeline.currentLoop >= timeline.loops) {
                it = timelines.erase(it);
            } 

        } else {
            ++it;
        }
    }
}

// ACTIONS
H2DE_TimelineID H2DE_TimelineManager::create(uint32_t duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, uint32_t loops, bool pauseSensitive) {
    H2DE_TimelineManager::H2DE_Timeline timeline = H2DE_TimelineManager::H2DE_Timeline();
    timeline.duration = std::max(static_cast<float>(duration) + engine->getDeltaTime() + 1, 1.0f);
    timeline.loops = loops;
    timeline.easing = easing;
    timeline.update = update;
    timeline.completed = completed;
    timeline.pauseSensitive = pauseSensitive;
    timeline.paused = false;

    id++;
    timelines[id] = timeline;

    return id;
}

void H2DE_TimelineManager::pause(H2DE_TimelineID id) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        it->second.paused = true;
    }
}

void H2DE_TimelineManager::resume(H2DE_TimelineID id) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        it->second.paused = false;
    }
}

void H2DE_TimelineManager::togglePause(H2DE_TimelineID id) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        it->second.paused = !it->second.paused;
    }
}

void H2DE_TimelineManager::reset(H2DE_TimelineID id) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        it->second.current = 0;
        it->second.currentLoop = 0;
    }
}

void H2DE_TimelineManager::stop(H2DE_TimelineID id, bool callCompleted) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        if (callCompleted) {
            if (it->second.update) {
                it->second.update(1.0f);
            }

            if (it->second.completed) {
                it->second.completed();
            }
        }

        timelines.erase(it);
    }
}

// GETTER
bool H2DE_TimelineManager::isPaused(H2DE_TimelineID id) const {
    const auto it = timelines.find(id);
    return (it != timelines.end()) ? it->second.paused : false;
}

bool H2DE_TimelineManager::isStoped(H2DE_TimelineID id) const {
    const auto it = timelines.find(id);
    return (it == timelines.end());
}
