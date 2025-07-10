#include "H2DE/objects/H2DE_timer_object.h"
#include "H2DE/engine/H2DE_lerp_manager.h"

// INIT
H2DE_TimerObject::H2DE_TimerObject(H2DE_Engine* e, const H2DE_ObjectData& od, const H2DE_TimerObjectData& tod) : H2DE_Object(e, od), timerObjectData(tod) {
    refreshSurfaceBuffers();
    refreshMaxRadius();
    initChrono();
}

void H2DE_TimerObject::initChrono() {
    chrono = engine->createChrono(timerObjectData.time, timerObjectData.increasing, timerObjectData.pauseSensitive);
}

// CLEANUP
H2DE_TimerObject::~H2DE_TimerObject() {
    destroyChrono();

    if (textObject != nullptr) {
        if (engine->destroyObject(textObject)) {
            textObject = nullptr;
        }
    }

    H2DE_Object::destroySurfaces(surfaces);
}

void H2DE_TimerObject::destroyChrono() {
    chrono->stop();
}

// UPDATE
void H2DE_TimerObject::update() {
    timerObjectData.time = chrono->getTime();
    textObject->setText(getStringifiedTime());
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
    
    maxRadius = H2DE::max(maxHitboxesRadius, maxSurfaceRadius);
}

void H2DE_TimerObject::onReach(const H2DE_Time& target, const std::function<void()>& callback, bool once) {
    chrono->onReach(target, callback, once);
}

void H2DE_TimerObject::pause() {
    chrono->pause();
}

void H2DE_TimerObject::resume() {
    chrono->resume();
}

void H2DE_TimerObject::togglePause() {
    chrono->togglePause();
}

// GETTER
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
    return chrono->isPaused();
}

// SETTER

// -- non lerp
void H2DE_TimerObject::setTime(const H2DE_Time& time) {
    timerObjectData.time = time;
    chrono->setTime(time);
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setHours(uint8_t hours) {
    timerObjectData.time.hours = hours;
    chrono->setHours(hours);
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMinutes(uint8_t minutes) {
    timerObjectData.time.minutes = minutes;
    chrono->setMinutes(minutes);
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setSeconds(uint8_t seconds) {
    timerObjectData.time.seconds = seconds;
    chrono->setSeconds(seconds);
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setMilliseconds(uint16_t milliseconds) {
    timerObjectData.time.milliseconds = milliseconds;
    chrono->setMilliseconds(milliseconds);
    refreshSurfaceBuffers();
}

void H2DE_TimerObject::setIncreasing(bool increasing) {
    timerObjectData.increasing = increasing;
    chrono->setSetIncreasing(increasing);
}

void H2DE_TimerObject::setPauseSensitive(bool pauseSensitive) {
    timerObjectData.pauseSensitive = pauseSensitive;
    chrono->setPauseSensitive(pauseSensitive);
}

// lerp
H2DE_Timeline* H2DE_TimerObject::setTime(const H2DE_Time& time, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp(engine, timerObjectData.time, time, duration, easing, [this](H2DE_Time iv) {
        setTime(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setHours(uint8_t hours, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.hours, hours, duration, easing, [this](uint8_t iv) {
        setHours(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setMinutes(uint8_t minutes, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.minutes, minutes, duration, easing, [this](uint8_t iv) {
        setMinutes(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setSeconds(uint8_t seconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint8_t>(engine, timerObjectData.time.seconds, seconds, duration, easing, [this](uint8_t iv) {
        setSeconds(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}

H2DE_Timeline* H2DE_TimerObject::setMilliseconds(uint16_t milliseconds, uint32_t duration, H2DE_Easing easing, const std::function<void()>& completed, bool pauseSensitive) {
    H2DE_Timeline* timeline = H2DE_LerpManager::lerp<uint16_t>(engine, timerObjectData.time.milliseconds, milliseconds, duration, easing, [this](uint8_t iv) {
        setMilliseconds(iv);
    }, completed, pauseSensitive);

    addTimelineToTimelines(timeline);
    return timeline;
}
