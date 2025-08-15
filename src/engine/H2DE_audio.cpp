#include "H2DE/engine/H2DE_audio.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_Audio::H2DE_Audio(H2DE_Engine* e) : _engine(e) {
    _initSettings();
    _loadData();
}

void H2DE_Audio::_initSettings() const {
    static H2DE_Settings* settings = _engine->getSettings();

    settings->addSection("VOLUME");
    settings->addKey("VOLUME", "song", "100");
    settings->addKey("VOLUME", "sfx", "100");
}

void H2DE_Audio::_loadData() {
    static H2DE_Settings* settings = _engine->getSettings();

    int songVolume = settings->getKeyInteger("VOLUME", "song", 100);
    int sfxVolume = settings->getKeyInteger("VOLUME", "sfx", 100);

    setSongVolume(songVolume);
    setSfxVolume(sfxVolume);
}

// CLEANUP
H2DE_Audio::~H2DE_Audio() {
    _saveData();
}

void H2DE_Audio::_saveData() const {
    static H2DE_Settings* settings = _engine->getSettings();
    settings->setKeyValue("VOLUME", "song", std::to_string(H2DE::clamp(_songVolume, H2DE_VOLUME_MIN, H2DE_VOLUME_MAX)));
    settings->setKeyValue("VOLUME", "sfx", std::to_string(H2DE::clamp(_sfxVolume, H2DE_VOLUME_MIN, H2DE_VOLUME_MAX)));
}

// UPDATE
void H2DE_Audio::_update() {
    bool engineIsPaused = _engine->isPaused();

    for (const auto& [channel, data] : _channels) {

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
    _playChunk(true, name, loops, pauseSensitive);
}

H2DE_ChannelID H2DE_Audio::playSfx(const std::string& name, uint32_t loops, bool pauseSensitive) {
    return _playChunk(false, name, loops, pauseSensitive);
}

void H2DE_Audio::_pause() {
    for (const auto& [channel, data] : _channels) {
        if (data.pauseSensitive) {
            Mix_Pause(channel);
        }
    }
}

void H2DE_Audio::_resume() {
    for (const auto& [channel, data] : _channels) {
        Mix_Resume(channel);
    }
}

void H2DE_Audio::stopAll() {
    for (int i = 0; i < Mix_AllocateChannels(-1); i++) {
        Mix_HaltChannel(i);
    }

    _channels.clear();
}

void H2DE_Audio::stopSfx(H2DE_ChannelID id) {
    auto it = _channels.find(id);

    if (it != _channels.end()) {
        Mix_HaltChannel(id);
        _channels.erase(it);
    }
}

void H2DE_Audio::pauseAll() {
    for (auto& [channel, data] : _channels) {
        data.manuallyPaused = true;
        Mix_Pause(channel);
    }
}

void H2DE_Audio::pauseSfx(H2DE_ChannelID id) {
    auto it = _channels.find(id);

    if (it != _channels.end()) {
        Mix_Pause(id);
        it->second.manuallyPaused = true;
    }
}

void H2DE_Audio::resumeAll() {
    for (auto& [channel, data] : _channels) {
        data.manuallyPaused = false;
        Mix_Resume(channel);
    }
}

void H2DE_Audio::resumeSfx(H2DE_ChannelID id) {
    auto it = _channels.find(id);

    if (it != _channels.end()) {
        Mix_Resume(id);
        it->second.manuallyPaused = false;
    }
}

H2DE_ChannelID H2DE_Audio::_playChunk(bool isSong, const std::string& soundName, uint32_t loops, bool pauseSensitive) {
    Mix_Chunk* chunk = _getChunk(soundName);
    if (chunk == nullptr) {
        H2DE_Error::logWarning("Sound name \"" + soundName + "\" not found.");
        return H2DE_INVALID_CHANNEL_ID;
    }

    int channel = Mix_PlayChannel((isSong) ? 0 : _getNextFreeChannel(), chunk, loops);

    if (isSong) {
        setSongVolume(_songVolume);
    } else {
        setSfxVolume(_sfxVolume);
    }

    if (channel != -1) {
        _channels[channel] = { pauseSensitive, false };
    }

    return channel;
}

// GETTER
Mix_Chunk* H2DE_Audio::_getChunk(const std::string& soundName) const {
    const auto it = _sounds.find(soundName);

    if (it == _sounds.end()) {
        return nullptr;
    }

    return it->second;
}

int H2DE_Audio::_getNextFreeChannel() const {
    int totalChannels = Mix_AllocateChannels(-1);

    for (int i = 1; i < totalChannels; ++i) {
        if (!Mix_Playing(i)) {
            return i;
        }
    }

    return -1;
}

int H2DE_Audio::_lerpVolume(int volume) {
    volume = H2DE::clamp(volume, H2DE_VOLUME_MIN, H2DE_VOLUME_MAX);
    float blend = volume * 0.01f;
    return H2DE::lerp(0, MIX_MAX_VOLUME, blend, H2DE_EASING_LINEAR);
}

// SETTER
void H2DE_Audio::setSongVolume(int volume) {
    _songVolume = volume;
    Mix_Volume(0, _lerpVolume(volume));
}

void H2DE_Audio::setSfxVolume(int volume) {
    _sfxVolume = volume;
    int v = _lerpVolume(volume);

    for (int i = 1; i < Mix_AllocateChannels(-1); i++) {
        if (Mix_Playing(i)) {
            Mix_Volume(i, v);
        }
    }
}
