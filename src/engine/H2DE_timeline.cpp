#include "H2DE/H2DE/H2DE_timeline.h"

// INIT
H2DE_Engine::H2DE_Timeline::H2DE_Timeline(H2DE_Engine* e) : engine(e) {

}

// CLEANUP
H2DE_Engine::H2DE_Timeline::~H2DE_Timeline() {

}

// EVENTS
unsigned int H2DE_CreateTimeline(const H2DE_Engine* engine, unsigned int duration, H2DE_Easing easing, const std::function<void(float)>& update, const std::function<void()>& completed, int loops, bool pauseSensitive) {
    H2DE_Engine::H2DE_Timeline::SpecificTimeline timeline = H2DE_Engine::H2DE_Timeline::SpecificTimeline();
    timeline.max = (duration == 0) ? 1 : engine->fps / (1000.0f / duration);
    timeline.loops = loops;
    timeline.easing = easing;
    timeline.update = update;
    timeline.completed = completed;
    timeline.pauseSensitive = pauseSensitive;

    engine->timeline->id++;
    engine->timeline->timelines[engine->timeline->id] = timeline;

    return engine->timeline->id;
}

void H2DE_ResetTimeline(const H2DE_Engine* engine, unsigned int id) {
    auto it = engine->timeline->timelines.find(id);

    if (it != engine->timeline->timelines.end()) {
        it->second.current = 0;
        it->second.currentLoop = 0;
    }
}

void H2DE_StopTimeline(const H2DE_Engine* engine, unsigned int id, bool call) {
    auto it = engine->timeline->timelines.find(id);

    if (it != engine->timeline->timelines.end()) {

        if (call) {
            if (it->second.update) {
                it->second.update(1.0f);
            }

            if (it->second.completed) {
                it->second.completed();
            }
        }
        
        engine->timeline->timelines.erase(it);
    }
}

// UPDATE
void H2DE_Engine::H2DE_Timeline::update() {
    for (auto it = timelines.begin(); it != timelines.end(); ) {
        auto& [timelineId, timeline] = *it;
    
        if (timeline.pauseSensitive && H2DE_IsPaused(engine)) {
            ++it;
            continue;
        }
    
        timeline.current++;

        if (timeline.update) {
            float blend = std::clamp(timeline.current, 0u, timeline.max) / static_cast<float>(timeline.max);
            blend = H2DE_Lerp(0.0f, 1.0f, blend, timeline.easing);
            timeline.update(blend);
        }
    
        if (timeline.current >= timeline.max) {
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
