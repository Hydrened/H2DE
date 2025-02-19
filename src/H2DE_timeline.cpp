#include "H2DE/H2DE_timeline.h"
#undef max

// INIT
H2DE_Timeline::H2DE_Timeline(H2DE_Engine* en, unsigned int du, H2DE_TimelineEffect ef, std::function<void(float)> up, std::function<void()> co, int lo) : duration(H2DE_GetSteps(en, du)), effect(ef), update(up), completed(co), loops(lo), loopSave(lo) {

}

H2DE_Timeline* H2DE_CreateTimeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loops) {
    return new H2DE_Timeline(engine, duration, effect, update, completed, loops);
}

// CLEANUP
H2DE_Timeline::~H2DE_Timeline() {

}

// EVENTS
bool H2DE_TickTimeline(H2DE_Timeline* timeline) {
    if (timeline->finished) return false;

    if (timeline->frame < timeline->duration) {
        timeline->frame++;

        float blend;
        switch (timeline->effect) {
            case LINEAR: blend = (float)timeline->frame / (float)timeline->duration; break;
            case EASE_IN_OUT: blend = 0.5f * (1.0f - std::cos(M_PI * (float)timeline->frame / (float)timeline->duration)); break;
            case EASE_IN: blend = std::pow((float)timeline->frame / (float)timeline->duration, 2.0f); break;
            case EASE_OUT: blend = 1 - std::pow(1 - (float)timeline->frame / (float)timeline->duration, 2.0f); break;
            default: blend = 1.0f; break;
        }
        if (timeline->update) timeline->update(blend);
        
    } else {
        if (timeline->loops != -1) {
            timeline->loops = std::max(0, timeline->loops - 1);
            
            if (timeline->loops != 0) {
                timeline->frame = 0;
            } else timeline->finished = true;
        } else timeline->frame = 0;
        
        if (timeline->duration == 0 && timeline->update) timeline->update(1.0f);
        if (timeline->completed) timeline->completed();
    }

    return true;
}

void H2DE_ResetTimeline(H2DE_Timeline* timeline) {
    timeline->frame = 0;
    timeline->loops = timeline->loopSave;
    timeline->finished = false;
}
