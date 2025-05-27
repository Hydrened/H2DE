#include "H2DE/H2DE_timeline_manager.h"

// INIT
H2DE_TimelineManager::H2DE_TimelineManager(H2DE_Engine* e) : engine(e) {

}

// CLEANUP
H2DE_TimelineManager::~H2DE_TimelineManager() {

}

// UPDATE
void H2DE_TimelineManager::update() {
    for (auto it = timelines.begin(); it != timelines.end(); ) {
        auto& [timelineId, timeline] = *it;
    
        if (timeline.pauseSensitive && engine->isPaused()) {
            ++it;
            continue;
        }

        timeline.current += engine->getDeltaTime() * 1000;

        if (timeline.update) {
            float blend = std::clamp(timeline.current, 0u, timeline.duration) / static_cast<float>(timeline.duration);
            blend = H2DE_Lerp(0.0f, 1.0f, blend, timeline.easing);
            timeline.update(blend);
        }
    
        if (timeline.current >= timeline.duration) {
            if (timeline.completed) {
                timeline.completed();
            }

            timeline.currentLoop++;
            timeline.current = 0;

            if (timeline.loops == -1) {
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

// ACIOTNS
unsigned int H2DE_TimelineManager::create(unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive) {
    H2DE_TimelineManager::H2DE_Timeline timeline = H2DE_TimelineManager::H2DE_Timeline();
    timeline.duration = duration;
    timeline.loops = loops;
    timeline.easing = easing;
    timeline.update = update;
    timeline.completed = completed;
    timeline.pauseSensitive = pauseSensitive;

    id++;
    timelines[id] = timeline;

    return id;
}

void H2DE_TimelineManager::reset(unsigned int id) {
    const auto it = timelines.find(id);

    if (it != timelines.end()) {
        it->second.current = 0;
        it->second.currentLoop = 0;
    }
}

void H2DE_TimelineManager::stop(unsigned int id, bool callCompleted) {
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
