#include "H2DE/engine/H2DE_chrono_manager.h"

#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_ChronoManager::H2DE_ChronoManager(H2DE_Engine* e) : engine(e) {
    chronos.reserve(1000);
}

// CLEANUP
H2DE_ChronoManager::~H2DE_ChronoManager() {
    for (H2DE_Chrono* chrono : chronos) {
        delete chrono;
    }
}

void H2DE_ChronoManager::destroyChrono(H2DE_Chrono* chrono) {
    auto it = std::find(chronos.begin(), chronos.end(), chrono);
    if (it != chronos.end()) {
        chronos.erase(it);
    }
}

// UPDATE
void H2DE_ChronoManager::update() {
    for (H2DE_Chrono* chrono : chronos) {
        chrono->update();
    }
}

// ACTIONS
H2DE_Chrono* H2DE_ChronoManager::create(const H2DE_Time& start, bool increasing, bool pauseSensitive) {
    H2DE_Chrono* chrono = new H2DE_Chrono(engine, this, start, increasing, pauseSensitive);
    chronos.push_back(chrono);
    return chrono;
}

// GETTER
H2DE_Time H2DE_ChronoManager::elapsedToTime(float elapsed) {
    H2DE_Time res = H2DE_Time();

    res.milliseconds = std::clamp(H2DE::round((elapsed - H2DE::floor(elapsed)) * 1000.0f), 0, 999);
    res.seconds = std::clamp(static_cast<int>(elapsed) % 60, 0, 59);
    res.minutes = std::clamp((static_cast<int>(elapsed) % 3600) / 60, 0, 59);
    res.hours = static_cast<int>(elapsed) / 3600;

    return res;
}
