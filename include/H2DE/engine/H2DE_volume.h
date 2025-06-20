#ifndef H2DE_VOLUME_H
#define H2DE_VOLUME_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_Volume {
public:
    void playSong(const std::string& name, uint32_t loops, bool pauseSensitive = true);
    int playSfx(const std::string& name, uint32_t loops, bool pauseSensitive = true);

    inline void stopSong() const { stopSfx(0); }
    inline void stopSfx(H2DE_ChannelID id) const { Mix_HaltChannel(id); }
    void stopAll() const;

    inline void pauseSong() const { pauseSfx(0); }
    inline void pauseSfx(H2DE_ChannelID id) const { Mix_Pause(id); }
    void pauseAll() const;

    inline void resumeSong() const { resumeSfx(0); }
    inline void resumeSfx(H2DE_ChannelID id) const { Mix_Resume(id); }
    void resumeAll() const;

    inline bool isSongPlaying() const { return isSfxPlaying(0); }
    inline bool isSfxPlaying(H2DE_ChannelID id) const { return (Mix_Playing(id) == 1); }

    void setSongVolume(int volume);
    void setSfxVolume(int volume);

    friend class H2DE_Engine;
    friend class H2DE_AssetLoaderManager;

private:
    H2DE_Engine* engine;

    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<int, bool> playingChannelsPauseSensitive = {}; 
    int songVolume = -1;
    int sfxVolume = -1;

    H2DE_Volume(H2DE_Engine* engine);
    ~H2DE_Volume() = default;

    void initSettings() const;
    void loadData();

    void pause();
    void resume();

    int playChunk(bool isSong, const std::string& soundName, uint32_t loops, bool pauseSensitive = true);

    Mix_Chunk* getChunk(const std::string& soundName) const;
    static int lerpVolume(int volume);
};

#endif
