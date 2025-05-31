#include "H2DE/engine/H2DE_asset_loader_manager.h"
#include "H2DE/engine/H2DE_error.h"

// INIT
H2DE_AssetLoaderManager::H2DE_AssetLoaderManager(H2DE_Engine* e, SDL_Renderer* r) : engine(e), renderer(r) {

}

// CLEANUP
H2DE_AssetLoaderManager::~H2DE_AssetLoaderManager() {

}

// ACTIONS
void H2DE_AssetLoaderManager::loadAssets(const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory)) {
        H2DE_Error::logWarning("Asset directory not found");
        return;
    }

    if (!std::filesystem::is_directory(directory)) {
        H2DE_Error::logError("Path isn't a directory");
        return;
    }

    const std::vector<std::filesystem::path> filesToLoad = getFilesToLoad(directory);
    assetsToLoad = filesToLoad.size();

    for (const std::filesystem::path& file : filesToLoad) {
        importFile(file);
    }

    std::unordered_map<std::string, SDL_Texture*>& textures = engine->renderer->textures;
    for (const auto& [name, texture] : textureBuffer) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            H2DE_Error::logWarning("Texture \"" + name + "\" has been overridden");
        }

        textures[name] = texture;
    }

    std::unordered_map<std::string, Mix_Chunk*>& sounds = engine->volume->sounds;
    for (const auto& [name, sound] : soundBuffer) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            H2DE_Error::logWarning("Sound \"" + name + "\" has been overridden");
        }

        sounds[name] = sound;
    }

    textureBuffer.clear();
    soundBuffer.clear();

    std::cout << std::endl << "H2DE => Loading complete" << std::endl;
}

void H2DE_AssetLoaderManager::importFile(const std::filesystem::path& file) {
    const std::filesystem::path extension = file.extension();

    const bool isImg = (std::find(supportedImg.begin(), supportedImg.end(), extension) != supportedImg.end());
    const bool isSound = (std::find(supportedSound.begin(), supportedSound.end(), extension) != supportedSound.end());

    if (isImg) {
        importTexture(file);
    } else if (isSound) {
        importSound(file);
    }
}

void H2DE_AssetLoaderManager::importTexture(const std::filesystem::path& file) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.string().c_str());
    const std::string name = file.filename().string();

    if (texture != nullptr) {
        if (textureBuffer.find(name) != textureBuffer.end()) {
            H2DE_Error::logWarning("Texture \"" + name + "\" has been overridden");
        }

        textureBuffer[name] = texture;
        
    } else {
        H2DE_Error::logError("Texture \"" + name + "\" can't be loaded: " + SDL_GetError());
    }

    assetImported();
}

void H2DE_AssetLoaderManager::importSound(const std::filesystem::path& file) {
    Mix_Chunk* sound = Mix_LoadWAV(file.string().c_str());
    const std::string name = file.filename().string();

    if (sound != nullptr) {
        if (soundBuffer.find(name) != soundBuffer.end()) {
            H2DE_Error::logWarning("Sound \"" + name + "\" has been overridden");
        }
    
        soundBuffer[name] = sound;

    } else {
        H2DE_Error::logError("Sound \"" + name + "\" can't be loaded");
    }

    assetImported();
}

void H2DE_AssetLoaderManager::assetImported() {
    constexpr int barWidth = 30;

    loadedAssets++;
    const float blend = static_cast<float>(loadedAssets) / assetsToLoad;
    const int percentage = static_cast<int>(blend * 100);
    const int squares = static_cast<int>(blend * barWidth);

    std::cout << "H2DE => Loading: [";
    for (int i = 0; i < barWidth; i++) {
        std::cout << ((i <= squares) ? '#' : '.');
    }

    std::cout << "] " << percentage << "%\r";
    std::cout.flush();
}

void H2DE_AssetLoaderManager::loadFont(const std::string& name, const H2DE_Font& font) {
    if (engine->fonts.find(name) != engine->fonts.end()) {
        H2DE_Error::logWarning("Font \"" + name + "\" has been overridden");
    }

    engine->fonts[name] = font;
}

// GETTER
const std::vector<std::filesystem::path> H2DE_AssetLoaderManager::getFilesToLoad(const std::filesystem::path& directory) const {
    std::vector<std::filesystem::path> res = {};

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            const std::string extension = entry.path().extension().string();

            const bool isImg = (std::find(supportedImg.begin(), supportedImg.end(), extension) != supportedImg.end());
            const bool isSound = (std::find(supportedSound.begin(), supportedSound.end(), extension) != supportedSound.end());
            const bool supported = (isImg || isSound);

            if (supported) {
                res.push_back(entry.path());
            }

        } else if (std::filesystem::is_directory(entry.status())) {
            std::vector<std::filesystem::path> children = getFilesToLoad(entry.path());
            res.insert(res.end(), children.begin(), children.end());
        }
    }

    return res;
}
