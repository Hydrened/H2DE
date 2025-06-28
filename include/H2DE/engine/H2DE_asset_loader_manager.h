#ifndef H2DE_ASSET_LOADER_MANAGER_H
#define H2DE_ASSET_LOADER_MANAGER_H

#include <H2DE/engine/H2DE_engine.h>
class H2DE_Engine;

class H2DE_AssetLoaderManager {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;

    const std::array<std::string, 4> supportedImg = { ".png", ".jpg", ".jpeg", ".bmp" };
    const std::array<std::string, 3> supportedSound = { ".mp3", ".ogg", ".wav" };

    std::unordered_map<std::string, SDL_Texture*> textureBuffer = {};
    std::unordered_map<std::string, Mix_Chunk*> soundBuffer = {};

    uint32_t loadedAssets = 0;
    uint32_t assetsToLoad = 0;

    H2DE_AssetLoaderManager(H2DE_Engine* engine, SDL_Renderer* renderer) noexcept;
    ~H2DE_AssetLoaderManager() noexcept = default;

    void importFile(const std::filesystem::path& file);
    void importTexture(const std::filesystem::path& file);
    void importSound(const std::filesystem::path& file);
    void assetImported();

    const std::vector<std::filesystem::path> getFilesToLoad(const std::filesystem::path& directory) const;

    void loadAssets(const std::filesystem::path& directory);
    void loadFont(const std::string& name, const H2DE_Font& font);

    friend class H2DE_Engine;
    friend class H2DE_Window;
};

#endif
