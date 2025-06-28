#include "H2DE/engine/H2DE_volume.h"

// INIT
H2DE_Volume::H2DE_Volume(H2DE_Engine* e) : engine(e) {
    initSettings();
    loadData();
}

void H2DE_Volume::initSettings() const {
    static H2DE_Settings* settings = engine->getSettings();

    settings->addSection("VOLUME");
    settings->addKey("VOLUME", "song", "100");
    settings->addKey("VOLUME", "sfx", "100");
}

void H2DE_Volume::loadData() {
    static H2DE_Settings* settings = engine->getSettings();

    int songVolume = settings->getKeyInteger("VOLUME", "song", 100);
    int sfxVolume = settings->getKeyInteger("VOLUME", "sfx", 100);

    setSongVolume(songVolume);
    setSfxVolume(sfxVolume);
}

// UPDATE
void H2DE_Volume::update() {
    bool engineIsPaused = engine->isPaused();

    for (const auto& [channel, pauseSensitive] : playingChannelsPauseSensitive) {
        if (!pauseSensitive) {
            continue;
        }

        if (engineIsPaused) {
            Mix_Pause(channel);
        } else {
            Mix_Resume(channel);
        }
    }
}

// ACTIONS
void H2DE_Volume::playSong(const std::string& name, uint32_t loops, bool pauseSensitive) {
    playChunk(true, name, loops, pauseSensitive);
}

int H2DE_Volume::playSfx(const std::string& name, uint32_t loops, bool pauseSensitive) {
    return playChunk(false, name, loops, pauseSensitive);
}

void H2DE_Volume::pause() {
    for (const auto& [channel, pauseSensitive] : playingChannelsPauseSensitive) {
        if (pauseSensitive) {
            Mix_Pause(channel);
        }
    }
}

void H2DE_Volume::resume() {
    for (const auto& [channel, pauseSensitive] : playingChannelsPauseSensitive) {
        Mix_Resume(channel);
    }
}

void H2DE_Volume::stopAll() const {
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        Mix_HaltChannel(i);
    }
}

void H2DE_Volume::pauseAll() const {
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        Mix_Pause(i);
    }
}

void H2DE_Volume::resumeAll() const {
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        Mix_Resume(i);
    }
}

int H2DE_Volume::playChunk(bool isSong, const std::string& soundName, uint32_t loops, bool pauseSensitive) {
    Mix_Chunk* chunk = getChunk(soundName);
    if (!chunk) {
        return -1;
    }

    int channel = Mix_PlayChannel((isSong) ? 0 : -1, chunk, loops);

    if (channel != -1) {
        Mix_Volume(channel, (isSong) ? songVolume : sfxVolume);
        playingChannelsPauseSensitive[channel] = pauseSensitive;
    }

    return channel;
}

// GETTER
Mix_Chunk* H2DE_Volume::getChunk(const std::string& soundName) const {
    const auto it = sounds.find(soundName);

    if (it == sounds.end()) {
        return nullptr;
    }

    return it->second;
}

int H2DE_Volume::lerpVolume(int volume) {
    volume = std::clamp(volume, 0, 100);
    float blend = volume * 100.0f;
    return H2DE::lerp(0, MIX_MAX_VOLUME, blend, H2DE_EASING_LINEAR);
}

// SETTER
void H2DE_Volume::setSongVolume(int volume) {
    songVolume = lerpVolume(volume);
    Mix_Volume(0, songVolume);
}

void H2DE_Volume::setSfxVolume(int volume) {
    sfxVolume = lerpVolume(volume);

    for (int i = 1; i < Mix_AllocateChannels(-1); i++) {
        if (Mix_Playing(i)) {
            Mix_Volume(i, sfxVolume);
        }
    }
}
