#ifndef H2DE_AUIDIO_H
#define H2DE_AUIDIO_H

/**
 * @file H2DE_audio.h
 * @brief Audio manager for songs and sound effects in the H2DE engine.
 * 
 * This file declares the H2DE_Audio class, responsible for controlling audio playback,
 * pause/resume logic, volume settings, and more, within the context of the H2DE game engine.
 * 
 * It handles both songs (usually played on channel 0) and sound effects (played on dynamic channels),
 * and supports pause-sensitive audio for in-game pausing logic.
 */

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

/**
 * @class H2DE_Audio
 * @brief Audio system for managing songs and sound effects in H2DE.
 * 
 * This class wraps SDL_mixer to provide simple and flexible playback of music and sound effects.
 * It allows playing, stopping, pausing, and resuming sounds across different channels.
 * 
 * Supports:
 * - Channel-based playback (channel 0 is reserved for songs)
 * - Pause-sensitive logic (sounds can auto-pause/resume when the game does)
 * - Volume control for both songs and SFX
 * - Integration with H2DE_Engine and asset management
 * 
 * Only one song is expected to be played at a time, while multiple sound effects can play simultaneously.
 * Internally manages chunk caching and playback tracking.
 */
class H2DE_Audio {
public:
    /**
     * @brief Play a song by its name.
     * 
     * Loops the song the specified number of times.
     * The pauseSensitive flag controls whether the sound pauses when the game is paused.
     * 
     * @param name Name of the song to play.
     * @param loops Number of times to loop the song.
     * @param pauseSensitive If true, the song pauses automatically when the game pauses.
     */
    void playSong(const std::string& name, uint32_t loops, bool pauseSensitive = true);
    /**
     * @brief Play a sound effect (SFX) by its name.
     * 
     * Loops the SFX the specified number of times.
     * The pauseSensitive flag controls whether the SFX pauses when the game pauses.
     * 
     * @param name Name of the sound effect to play.
     * @param loops Number of times to loop the sound.
     * @param pauseSensitive If true, the sound effect pauses automatically when the game pauses.
     * @return The channel number where the sound is played, or H2DE_INVALID_CHANNEL_ID (-1) if playback failed.
     */
    H2DE_ChannelID playSfx(const std::string& name, uint32_t loops, bool pauseSensitive = true);

    /**
     * @brief Stop the currently playing song.
     * 
     * Stops the sound playing on the channel reserved for songs (usually channel 0).
     */
    inline void stopSong() {
        stopSfx(0);
    }
    /**
     * @brief Stop the sound effect playing on a specific channel.
     * 
     * Immediately halts playback on the given channel.
     * 
     * @param id The channel ID to stop.
     */
    void stopSfx(H2DE_ChannelID id);
    /**
     * @brief Stop all currently playing sounds.
     * 
     * Stops playback on all active audio channels.
     */
    void stopAll();

    /**
     * @brief Pause the currently playing song.
     * 
     * Pauses the playback on the channel reserved for songs (usually channel 0).
     */
    inline void pauseSong() {
        pauseSfx(0);
    }
    /**
     * @brief Pause the sound effect playing on a specific channel.
     * 
     * Pauses playback on the given channel.
     * 
     * @param id The channel ID to pause.
     */
    void pauseSfx(H2DE_ChannelID id);
    /**
     * @brief Pause all currently playing sounds.
     * 
     * Pauses playback on all active audio channels.
     */
    void pauseAll();

    /**
     * @brief Resume playback of the currently paused song.
     * 
     * Resumes playback on the channel reserved for songs (usually channel 0).
     */
    inline void resumeSong() {
        resumeSfx(0); 
    }
    /**
     * @brief Resume playback of the paused sound effect on a specific channel.
     * 
     * Resumes playback on the given channel.
     * 
     * @param id The channel ID to resume.
     */
    void resumeSfx(H2DE_ChannelID id);
    /**
     * @brief Resume all paused sounds.
     * 
     * Resumes playback on all audio channels that were paused.
     */
    void resumeAll();

    /**
     * @brief Check if a song is currently playing.
     * 
     * Returns true if audio is playing on the song channel (usually channel 0).
     * 
     * @return true if a song is playing, false otherwise.
     */
    inline bool isSongPlaying() const {
        return isSfxPlaying(0); 
    }
    /**
     * @brief Check if a sound effect is playing on a specific channel.
     * 
     * @param id The channel ID to check.
     * @return true if the channel is currently playing a sound, false otherwise.
     */
    inline bool isSfxPlaying(H2DE_ChannelID id) const {
        return (Mix_Playing(id) == 1); 
    }

    /**
     * @brief Set the volume for all songs.
     * 
     * Updates the internal song volume and applies it to the song channel (usually channel 0)
     * if a song is currently playing.
     * 
     * @param volume Volume level (H2DE_MIN_VOLUME (0) to H2DE_MAX_VOLUME (100)).
     */
    void setSongVolume(int volume);
    /**
     * @brief Set the volume for all sound effects.
     * 
     * Updates the internal SFX volume. The volume is applied to new sound effects as they are played.
     * 
     * @param volume Volume level (from H2DE_MIN_VOLUME (0) to H2DE_MAX_VOLUME (100)).
     */
    void setSfxVolume(int volume);

    friend class H2DE_Engine;
    friend class H2DE_AssetLoaderManager;

private:
    struct H2DE_AudioChannel {
        bool pauseSensitive = true;
        bool manuallyPaused = false;
    };

private:
    H2DE_Engine* engine;

    std::unordered_map<std::string, Mix_Chunk*> sounds;
    std::unordered_map<H2DE_ChannelID, H2DE_AudioChannel> channels = {};

    int songVolume = 100;
    int sfxVolume = 100;

    H2DE_Audio(H2DE_Engine* engine);
    ~H2DE_Audio();

    void initSettings() const;
    void loadData();

    void saveData() const;

    void update();

    void pause();
    void resume();

    H2DE_ChannelID playChunk(bool isSong, const std::string& soundName, uint32_t loops, bool pauseSensitive = true);

    Mix_Chunk* getChunk(const std::string& soundName) const;
    int getNextFreeChannel() const;
    static int lerpVolume(int volume);
};

#endif
