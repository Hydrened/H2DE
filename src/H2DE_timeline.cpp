#include <H2DE/H2DE_timeline.h>

H2DE_Timeline::H2DE_Timeline(H2DE_Engine* eg, unsigned int d, H2DE_TimelineEffect ef, std::function<void(float)> u, std::function<void()> c, int l) : engine(eg), duration(getSteps(d)), effect(ef), update(u), completed(c), loop(l) {

}

H2DE_Timeline::~H2DE_Timeline() {

}

bool H2DE_TickTimeline(H2DE_Timeline* timeline) {
    if (timeline->index < timeline->duration && timeline->duration != 0) {
        timeline->index++;

        float blend;
        switch (timeline->effect) {
            case LINEAR: blend = (float)timeline->index / (float)timeline->duration; break;
            case EASE_IN_OUT: blend = 0.5f * (1.0f - cos(M_PI * (float)timeline->index / (float)timeline->duration)); break;
            case EASE_IN: blend = pow((float)timeline->index / (float)timeline->duration, 2.0f); break;
            case EASE_OUT: blend = 1 - pow(1 - (float)timeline->index / (float)timeline->duration, 2.0f); break;
            default: blend = 1.0f; break;
        }

        if (timeline->update) timeline->update(blend);
        return true;
    } else {
        if (timeline->update) timeline->update(1.0f);
        if (timeline->completed) timeline->completed();
        
        if (timeline->loop != 0) {
            timeline->index = 0;
            timeline->loop--;
            return true;
        } else return false;
    }
}

void H2DE_ResetTimeline(H2DE_Timeline* timeline) {
    timeline->index = 0;
}

unsigned int H2DE_Timeline::getSteps(unsigned int ms) {
    return round(H2DE_GetEngineFPS(engine) / 1000.0f * ms);
}


H2DE_TimelineManager* H2DE_CreateTimelineManager() {
    return new H2DE_TimelineManager();
}

void H2DE_DestroyTimelineManager(H2DE_TimelineManager* timelineManager) {
    H2DE_ClearTimelineManager(timelineManager);
    delete timelineManager;
}

H2DE_TimelineManager::H2DE_TimelineManager() {

}

void H2DE_AddTimelineToManager(H2DE_TimelineManager* manager, H2DE_Timeline* timeline) {
    manager->timelines.push_back(timeline);
}

void H2DE_RemoveTimelineFromManager(H2DE_TimelineManager* manager, H2DE_Timeline* timeline) {
    auto it = std::find(manager->timelines.begin(), manager->timelines.end(), timeline);
    if (it != manager->timelines.end()) {
        delete timeline;
        manager->timelines.erase(it);
    }
}

void H2DE_TickTimelineManager(H2DE_TimelineManager* manager) {
    std::vector<H2DE_Timeline*> timelinesToRemove;

    for (H2DE_Timeline* timeline : manager->timelines) if (!H2DE_TickTimeline(timeline)) {
        timelinesToRemove.push_back(timeline);
    }

    for (H2DE_Timeline* timeline : timelinesToRemove) {
        manager->timelines.erase(std::remove(manager->timelines.begin(), manager->timelines.end(), timeline), manager->timelines.end());
    }
}

void H2DE_ClearTimelineManager(H2DE_TimelineManager* manager) {
    for (H2DE_Timeline* timeline : manager->timelines) {
        delete timeline;
        manager->timelines.erase(std::remove(manager->timelines.begin(), manager->timelines.end(), timeline), manager->timelines.end());
    }
}

void H2DE_ResetTimelineManager(H2DE_TimelineManager* manager) {
    for (H2DE_Timeline* timeline : manager->timelines) H2DE_ResetTimeline(timeline);
}
