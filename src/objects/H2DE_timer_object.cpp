#include "H2DE/objects/H2DE_timer_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TimerObject::H2DE_TimerObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TimerObjectData& tod) : H2DE_Object(e, od), timerObjectData(tod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
    initElapsedTime();
    initTimeline();
}

void H2DE_TimerObject::initElapsedTime() noexcept {
    float ms = timerObjectData.time.milliseconds / 1000.0f;
    float sec = timerObjectData.time.seconds;
    float min = timerObjectData.time.minutes * 60.0f;
    float hrs = timerObjectData.time.hours * 3600.0f;
    elapsed = ms + sec + min + hrs;
}

void H2DE_TimerObject::initTimeline() {
    timelineId = engine->createTimeline(0, H2DE_EASING_LINEAR, nullptr, [this]() {
        int deltaTimeMultiplier = (timerObjectData.increasing) ? 1 : -1;
        
        elapsed += engine->getDeltaTime() * deltaTimeMultiplier;
        elapsed = std::max(elapsed, 0.0f);
        timerObjectData.time = H2DE_TimerObject::elapsedToTime(elapsed);

        textObject->setText(getStringifiedTime());

    }, H2DE_INFINITE_LOOP, timerObjectData.pauseSensitive);
}

// CLEANUP
H2DE_TimerObject::~H2DE_TimerObject() {
    destroyTimeline();

    if (textObject != nullptr) {
        if (engine->destroyObject(textObject)) {
            textObject = nullptr;
        }
    }

    H2DE_Object::destroySurfaces(surfaces);
}

void H2DE_TimerObject::destroyTimeline() {
    engine->stopTimeline(timelineId, false);
}

// UPDATE
void H2DE_TimerObject::update() {
    float epsilon = 0.5f / engine->getFPS();

    for (auto it = onReachEvents.begin(); it != onReachEvents.end(); ) {
        float elapsedDifferenceWithTarget = H2DE::abs(H2DE_TimerObject::getElapsed(it->target) - elapsed);

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
void H2DE_TimerObject::refreshTextObject() {
    if (textObject != nullptr) {
        if (engine->destroyObject(textObject)) {
            textObject = nullptr;
        }
    }

    H2DE_ObjectData od = objectData;
    od.index++;

    H2DE_TextObjectData tod = H2DE_TextObjectData();
    timerObjectData.text.text = getStringifiedTime();
    tod.text = timerObjectData.text;

    textObject = engine->createObject<H2DE_TextObject>(od, tod);
}

void H2DE_TimerObject::refreshSurfaceBuffers() {
    refreshTextObject();

    const std::vector<H2DE_Surface*> sortedSurfaces = H2DE_Object::getSortedSurfaces(surfaces);

    surfaceBuffers.clear();
    surfaceBuffers.reserve(sortedSurfaces.size());
    surfaceBuffers.insert(surfaceBuffers.end(), sortedSurfaces.begin(), sortedSurfaces.end());
    rescaleSurfaceBuffers();
}

void H2DE_TimerObject::refreshMaxRadius() {
    float maxHitboxesRadius = getMaxHitboxRadius();
    float maxSurfaceRadius = getMaxSurfaceRadius(surfaces);
    
    maxRadius = std::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_TimerObject::refreshTimeline() {
    destroyTimeline();
    initTimeline();
}

void H2DE_TimerObject::onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once) {
    if (!callback) {
        return;
    }

    H2DE_TimerObject::H2DE_OnReachEvent onReachEvent = H2DE_TimerObject::H2DE_OnReachEvent();
    onReachEvent.target = target;
    onReachEvent.callback = callback;
    onReachEvent.once = once;
    onReachEvents.push_back(onReachEvent);
}

void H2DE_TimerObject::pause() {
    engine->pauseTimeline(timelineId);
}

void H2DE_TimerObject::resume() {
    engine->resumeTimeline(timelineId);
}

void H2DE_TimerObject::togglePause() {
    engine->togglePauseTimeline(timelineId);
}

// GETTER
H2DE_Time H2DE_TimerObject::elapsedToTime(float elapsed) {
    H2DE_Time res = H2DE_Time();

    res.milliseconds = std::clamp(H2DE::round((elapsed - H2DE::floor(elapsed)) * 1000.0f), 0, 999);
    res.seconds = std::clamp(static_cast<int>(elapsed) % 60, 0, 59);
    res.minutes = std::clamp((static_cast<int>(elapsed) % 3600) / 60, 0, 59);
    res.hours = static_cast<int>(elapsed) / 3600;

    return res;
}

std::string H2DE_TimerObject::intToStr(int value, int nbDigits) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(nbDigits) << value;
    return oss.str();
}

const std::string H2DE_TimerObject::getStringifiedTime() const {
    std::string res = "";

    if (timerObjectData.displayHours) {
        res += std::to_string(timerObjectData.time.hours) + timerObjectData.separator;
    }

    if (timerObjectData.displayMinutes) {
        res += H2DE_TimerObject::intToStr(timerObjectData.time.minutes, 2) + timerObjectData.separator;
    }

    if (timerObjectData.displaySeconds) {
        res += H2DE_TimerObject::intToStr(timerObjectData.time.seconds, 2) + timerObjectData.separator;
    }

    if (timerObjectData.displayMilliseconds) {
        res += H2DE_TimerObject::intToStr(timerObjectData.time.milliseconds, 3) + timerObjectData.separator;
    }

    res.pop_back();
    return res;
}

bool H2DE_TimerObject::isPaused() const {
    return engine->isTimelinePaused(timelineId);
}

// SETTER

// -- non lerp
void H2DE_TimerObject::setTime(const H2DE_Time& time) {
    timerObjectData.time = time;
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setHours(uint8_t hours) {
    timerObjectData.time.hours = hours;
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMinutes(uint8_t minutes) {
    timerObjectData.time.minutes = minutes;
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setSeconds(uint8_t seconds) {
    timerObjectData.time.seconds = seconds;
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMilliseconds(uint16_t milliseconds) {
    timerObjectData.time.milliseconds = milliseconds;
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setIncreasing(bool increasing) {
    timerObjectData.increasing = increasing;
    refreshTimeline();
}

void H2DE_TimerObject::setPauseSensitive(bool pauseSensitive) {
    timerObjectData.pauseSensitive = pauseSensitive;
    refreshTimeline();
}

// lerp
H2DE_TimelineID H2DE_TimerObject::setTime(const H2DE_Time& time, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_TimelineID id = H2DE_LerpManager::lerp(engine, timerObjectData.time, time, duration, easing, [this](H2DE_Time iv) {
        setTime(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(id);
    return id;
}

H2DE_TimelineID H2DE_TimerObject::setHours(uint8_t hours, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_TimelineID id = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.hours, hours, duration, easing, [this](uint8_t iv) {
        setHours(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(id);
    return id;
}

H2DE_TimelineID H2DE_TimerObject::setMinutes(uint8_t minutes, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_TimelineID id = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.minutes, minutes, duration, easing, [this](uint8_t iv) {
        setMinutes(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(id);
    return id;
}

H2DE_TimelineID H2DE_TimerObject::setSeconds(uint8_t seconds, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_TimelineID id = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.seconds, seconds, duration, easing, [this](uint8_t iv) {
        setSeconds(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(id);
    return id;
}

H2DE_TimelineID H2DE_TimerObject::setMilliseconds(uint16_t milliseconds, H2DE_TimelineID duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_TimelineID id = H2DE_LerpManager::lerp<uint16_t>(engine, timerObjectData.time.milliseconds, milliseconds, duration, easing, [this](uint8_t iv) {
        setMilliseconds(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(id);
    return id;
}
