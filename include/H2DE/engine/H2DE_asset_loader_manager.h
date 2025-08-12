#pragma once

#include <mutex>
#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_AssetLoaderManager {
private:
    enum H2DE_AssetType {
        H2DE_ASSET_TYPE_NULL,
        H2DE_ASSET_TYPE_SURFACE,
        H2DE_ASSET_TYPE_SOUND,
    };

    struct H2DE_LoadedAsset {
        H2DE_AssetType type = H2DE_ASSET_TYPE_NULL;
        std::string name = "";
        SDL_Surface* surface = nullptr;
        Mix_Chunk* sound = nullptr;
    };

private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;

    const std::array<std::string, 4> supportedImg = { ".png", ".jpg", ".jpeg", ".bmp" };
    const std::array<std::string, 3> supportedSound = { ".mp3", ".ogg", ".wav" };

    std::vector<std::filesystem::path> filesToLoad;
    std::unordered_map<std::string, SDL_Surface*> surfaceBuffer = {};
    std::unordered_map<std::string, Mix_Chunk*> soundBuffer = {};

    std::function<void(float)> progress = nullptr;
    bool loading = false;

    uint32_t loadedAssets = 0;
    uint32_t assetsToLoad = 0;

    std::mutex loadMutex;

    bool silentLoad = false;

    H2DE_AssetLoaderManager(H2DE_Engine* engine, SDL_Renderer* renderer) noexcept : engine(engine), renderer(renderer) {};
    ~H2DE_AssetLoaderManager() noexcept = default;

    void update();

    H2DE_LoadedAsset loadFile(const std::filesystem::path& file);
    H2DE_LoadedAsset loadTexture(const std::filesystem::path& file);
    H2DE_LoadedAsset loadSound(const std::filesystem::path& file);

    void assetLoadedSync();
    void assetLoadedAsync();

    void loadAssets(const std::filesystem::path& directory, const std::function<void(float)>& progress, const std::function<void()>& completed, bool sync);
    void loadAssetsSync(const std::filesystem::path& directory);
    void loadAssetsAsync(const std::filesystem::path& directory, const std::function<void(float)>& progress, const std::function<void()>& completed);
    void loadFont(const std::string& name, const H2DE_Font& font);
    
    void loadTexturesFromBuffer();
    void loadSoundsFromBuffer();

    void refreshObjectsSurfaceBuffers();
    
    const std::vector<std::filesystem::path> getFilesToLoad(const std::filesystem::path& directory) const;

    friend class H2DE_Engine;
    friend class H2DE_Window;
};
