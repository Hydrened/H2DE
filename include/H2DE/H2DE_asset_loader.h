#ifndef H2DE_ASSET_LOADER_H
#define H2DE_ASSET_LOADER_H

#include <iomanip>
#include <sstream>
#include <H2DE/H2DE_engine.h>

class H2DE_Engine::H2DE_AssetLoader {
private:
    H2DE_Engine* engine;
    SDL_Renderer* renderer;

    std::unordered_map<std::string, SDL_Texture*> textureBuffer = {};
    std::unordered_map<std::string, Mix_Chunk*> soundBuffer = {};

    int loadedAssets = 0;
    int assetsToLoad = -1;

    std::vector<std::filesystem::path> getFilesToLoad(const std::filesystem::path& directory) const;

    void importFile(const std::filesystem::path& file);
    void importTexture(const std::filesystem::path& file);
    void importSound(const std::filesystem::path& file);
    void assetImported();

public:
    H2DE_AssetLoader(H2DE_Engine* engine, SDL_Renderer* renderer);
    ~H2DE_AssetLoader();

    void load(const std::filesystem::path& directory);

    std::unordered_map<std::string, SDL_Texture*> getLoadedTextures();
    std::unordered_map<std::string, Mix_Chunk*> getLoadedSounds();
};

#endif
