#ifndef H2DE_VOLUME_H
#define H2DE_VOLUME_H

#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_Volume {
private:
    H2DE_Engine* engine;

    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<int, bool> playingChannelsPauseSensitive = {}; 
    int songVolume = -1;
    int sfxVolume = -1;

    void initSettings() const;
    int playChunk(bool isSong, const std::string& soundName, int loops, bool pauseSensitive);
    Mix_Chunk* getChunk(const std::string& soundName) const;

public:
    H2DE_Volume(H2DE_Engine* engine);
    ~H2DE_Volume();

    void loadData();

    void pause() const;
    void resume() const;

    friend void H2DE_PlaySong(const H2DE_Engine* engine, const std::string& name, int loops, bool pauseSensitive);
    friend int H2DE_PlaySfx(const H2DE_Engine* engine, const std::string& name, int loops, bool pauseSensitive);
    friend void H2DE_StopSong(const H2DE_Engine* engine);
    friend void H2DE_StopSfx(const H2DE_Engine* engine, int id);
    friend void H2DE_PauseSong(const H2DE_Engine* engine);
    friend void H2DE_PauseSfx(const H2DE_Engine* engine, int id);
    friend void H2DE_ResumeSong(const H2DE_Engine* engine);
    friend void H2DE_ResumeSfx(const H2DE_Engine* engine, int id);

    friend void H2DE_SetSongVolume(const H2DE_Engine* engine, int volume);
    friend void H2DE_SetSfxVolume(const H2DE_Engine* engine, int volume);

    friend bool H2DE_IsSongPlaying(const H2DE_Engine* engine);
    friend bool H2DE_IsSfxPlaying(const H2DE_Engine* engine, int id);

    friend void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory);
};

#endif
