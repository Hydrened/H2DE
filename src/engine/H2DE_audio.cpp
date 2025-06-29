#include "H2DE/engine/H2DE_audio.h"

#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Audio::H2DE_Audio(H2DE_Engine* e) : engine(e) {
    initSettings();
    loadData();
}

void H2DE_Audio::initSettings() const {
    static H2DE_Settings* settings = engine->getSettings();

    settings->addSection("VOLUME");
    settings->addKey("VOLUME", "song", "100");
    settings->addKey("VOLUME", "sfx", "100");
}

void H2DE_Audio::loadData() {
    static H2DE_Settings* settings = engine->getSettings();

    int songVolume = settings->getKeyInteger("VOLUME", "song", 100);
    int sfxVolume = settings->getKeyInteger("VOLUME", "sfx", 100);

    setSongVolume(songVolume);
    setSfxVolume(sfxVolume);
}

// CLEANUP
H2DE_Audio::~H2DE_Audio() {
    saveData();
}

void H2DE_Audio::saveData() const {
    static H2DE_Settings* settings = engine->getSettings();
    settings->setKeyValue("VOLUME", "song", std::to_string(std::clamp(songVolume, H2DE_MIN_VOLUME, H2DE_MAX_VOLUME)));
    settings->setKeyValue("VOLUME", "sfx", std::to_string(std::clamp(sfxVolume, H2DE_MIN_VOLUME, H2DE_MAX_VOLUME)));
}

// UPDATE
void H2DE_Audio::update() {
    bool engineIsPaused = engine->isPaused();

    for (const auto& [channel, data] : channels) {

        if (!data.pauseSensitive) {
            continue;
        }

        if (data.manuallyPaused) {
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
void H2DE_Audio::playSong(const std::string& name, uint32_t loops, bool pauseSensitive) {
    playChunk(true, name, loops, pauseSensitive);
}

H2DE_ChannelID H2DE_Audio::playSfx(const std::string& name, uint32_t loops, bool pauseSensitive) {
    return playChunk(false, name, loops, pauseSensitive);
}

void H2DE_Audio::pause() {
    for (const auto& [channel, data] : channels) {
        if (data.pauseSensitive) {
            Mix_Pause(channel);
        }
    }
}

void H2DE_Audio::resume() {
    for (const auto& [channel, data] : channels) {
        Mix_Resume(channel);
    }
}

void H2DE_Audio::stopAll() {
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        Mix_HaltChannel(i);
    }

    channels.clear();
}

void H2DE_Audio::stopSfx(H2DE_ChannelID id) {
    auto it = channels.find(id);

    if (it != channels.end()) {
        Mix_HaltChannel(id);
        channels.erase(it);
    }
}

void H2DE_Audio::pauseAll() {
    for (auto& [channel, data] : channels) {
        data.manuallyPaused = true;
        Mix_Pause(channel);
    }
}

void H2DE_Audio::pauseSfx(H2DE_ChannelID id) {
    auto it = channels.find(id);

    if (it != channels.end()) {
        Mix_Pause(id);
        it->second.manuallyPaused = true;
    }
}

void H2DE_Audio::resumeAll() {
    for (auto& [channel, data] : channels) {
        data.manuallyPaused = false;
        Mix_Resume(channel);
    }
}

void H2DE_Audio::resumeSfx(H2DE_ChannelID id) {
    auto it = channels.find(id);

    if (it != channels.end()) {
        Mix_Resume(id);
        it->second.manuallyPaused = false;
    }
}

H2DE_ChannelID H2DE_Audio::playChunk(bool isSong, const std::string& soundName, uint32_t loops, bool pauseSensitive) {
    Mix_Chunk* chunk = getChunk(soundName);
    if (chunk == nullptr) {
        H2DE_Error::logWarning("Sound name \"" + soundName + "\" not found.");
        return H2DE_INVALID_CHANNEL_ID;
    }

    int channel = Mix_PlayChannel((isSong) ? 0 : getNextFreeChannel(), chunk, loops);

    if (isSong) {
        setSongVolume(songVolume);
    } else {
        setSfxVolume(sfxVolume);
    }

    if (channel != -1) {
        channels[channel] = { pauseSensitive, false };
    }

    return channel;
}

// GETTER
Mix_Chunk* H2DE_Audio::getChunk(const std::string& soundName) const {
    const auto it = sounds.find(soundName);

    if (it == sounds.end()) {
        return nullptr;
    }

    return it->second;
}

int H2DE_Audio::getNextFreeChannel() const {
    int totalChannels = Mix_AllocateChannels(-1);

    for (int i = 1; i < totalChannels; ++i) {
        if (!Mix_Playing(i)) {
            return i;
        }
    }

    return -1;
}

int H2DE_Audio::lerpVolume(int volume) {
    volume = std::clamp(volume, H2DE_MIN_VOLUME, H2DE_MAX_VOLUME);
    float blend = volume * 0.01f;
    return H2DE::lerp(0, MIX_MAX_VOLUME, blend, H2DE_EASING_LINEAR);
}

// SETTER
void H2DE_Audio::setSongVolume(int volume) {
    songVolume = volume;
    Mix_Volume(0, lerpVolume(volume));
}

void H2DE_Audio::setSfxVolume(int volume) {
    sfxVolume = volume;
    int v = lerpVolume(volume);

    for (int i = 1; i < Mix_AllocateChannels(-1); i++) {
        if (Mix_Playing(i)) {
            Mix_Volume(i, v);
        }
    }
}
