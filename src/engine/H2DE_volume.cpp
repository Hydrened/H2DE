#include "H2DE/H2DE_volume.h"

// INIT
H2DE_Engine::H2DE_Volume::H2DE_Volume(H2DE_Engine* e) : engine(e) {
    Mix_ReserveChannels(1);
    initSettings();
}

void H2DE_Engine::H2DE_Volume::initSettings() const {
    H2DE_SettingsAddSection(engine, "VOLUME");
    
    H2DE_SettingsAddKey(engine, "VOLUME", "song", "100");
    H2DE_SettingsAddKey(engine, "VOLUME", "sfx", "100");
}

// CLEANUP
H2DE_Engine::H2DE_Volume::~H2DE_Volume() {

}

// EVENTS
void H2DE_Engine::H2DE_Volume::loadData() {
    int songVolume = H2DE_SettingsGetKeyInteger(engine, "VOLUME", "song", 100);
    int sfxVolume = H2DE_SettingsGetKeyInteger(engine, "VOLUME", "sfx", 100);

    H2DE_SetSongVolume(engine, songVolume);
    H2DE_SetSfxVolume(engine, sfxVolume);
}

void H2DE_Engine::H2DE_Volume::pause() const {
    for (const auto& [channel, ps] : playingChannelsPauseSensitive) {
        if (ps) {
            Mix_Pause(channel);
        }
    }
}

void H2DE_Engine::H2DE_Volume::resume() const {
    for (const auto& [channel, ps] : playingChannelsPauseSensitive) {
        Mix_Resume(channel);
    }
}

void H2DE_PlaySong(const H2DE_Engine* engine, const std::string& songName, int loops, bool pauseSensitive) {
    engine->volume->playChunk(true, songName, loops, pauseSensitive);
}

int H2DE_PlaySfx(const H2DE_Engine* engine, const std::string& sfxName, int loops, bool pauseSensitive) {
    return engine->volume->playChunk(false, sfxName, loops, pauseSensitive);
}

int H2DE_Engine::H2DE_Volume::playChunk(bool isSong, const std::string& soundName, int loops, bool pauseSensitive) {
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

void H2DE_StopSong(const H2DE_Engine* engine) {
    Mix_HaltChannel(0);
}

void H2DE_StopSfx(const H2DE_Engine* engine, int sfxId) {
    Mix_HaltChannel(sfxId);
}

void H2DE_PauseSong(const H2DE_Engine* engine) {
    Mix_Pause(0);
}

void H2DE_PauseSfx(const H2DE_Engine* engine, int sfxId) {
    Mix_Pause(sfxId);
}

void H2DE_ResumeSong(const H2DE_Engine* engine) {
    Mix_Resume(0);
}

void H2DE_ResumeSfx(const H2DE_Engine* engine, int sfxId) {
    Mix_Resume(sfxId);
}

// GETTER
Mix_Chunk* H2DE_Engine::H2DE_Volume::getChunk(const std::string& soundName) const {
    auto it = engine->volume->sounds.find(soundName);

    if (it == engine->volume->sounds.end()) {
        return nullptr;
    }

    return it->second;
}

// SETTER
void H2DE_SetSongVolume(const H2DE_Engine* engine, int volume) {
    volume = std::clamp(volume, 0, 128);
    engine->volume->songVolume = volume;

    Mix_Volume(0, volume);
}

void H2DE_SetSfxVolume(const H2DE_Engine* engine, int volume) {
    volume = std::clamp(volume, 0, 128);
    engine->volume->sfxVolume = volume;

    for (int i = 1; i < Mix_AllocateChannels(-1); i++) {
        if (Mix_Playing(i)) {
            Mix_Volume(i, volume);
        }
    }
}
