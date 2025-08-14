#include "H2DE/engine/H2DE_asset_loader_manager.h"

#include <thread>
#include "H2DE/engine/H2DE_error.h"

// UPDATE
void H2DE_AssetLoaderManager::update() {
    loadTexturesFromBuffer();
    loadSoundsFromBuffer();
}

void H2DE_AssetLoaderManager::loadTexturesFromBuffer() {
    static std::unordered_map<std::string, SDL_Texture*>& textures = engine->renderer->textures;

    if (surfaceBuffer.empty()) {
        return;
    }

    std::unordered_map<std::string, SDL_Surface*> surfaceBufferToProcess;

    {
        std::lock_guard<std::mutex> lock(loadMutex);
        if (!surfaceBuffer.empty()) {
            surfaceBufferToProcess = std::move(surfaceBuffer);
            surfaceBuffer.clear();
        }
    }

    for (const auto& [name, surface] : surfaceBufferToProcess) {
        textures[name] = SDL_CreateTextureFromSurface(renderer, surface);
    }
}

void H2DE_AssetLoaderManager::loadSoundsFromBuffer() {
    static std::unordered_map<std::string, Mix_Chunk*>& sounds = engine->audio->sounds;

    if (soundBuffer.empty()) {
        return;
    }

    std::unordered_map<std::string, Mix_Chunk*> soundBufferToProcess;

    {
        std::lock_guard<std::mutex> lock(loadMutex);
        if (!soundBuffer.empty()) {
            soundBufferToProcess = std::move(soundBuffer);
            soundBuffer.clear();
        }
    }

    for (const auto& [name, sound] : soundBufferToProcess) {
        sounds[name] = sound;
    }
}

// ACTIONS

// -- load assets
void H2DE_AssetLoaderManager::loadAssets(const std::filesystem::path& directory, const std::function<void(float)>& progress, const std::function<void()>& completed, bool sync) {
    if (loading) {
        if (!silentLoad) {
            H2DE_Error::logWarning("Engine is already loading assets");
        }
        return;
    }

    if (!std::filesystem::exists(directory)) {
        if (!silentLoad) {
            H2DE_Error::logWarning("Asset directory not found");
        }
        return;
    }

    if (!std::filesystem::is_directory(directory)) {
        if (!silentLoad) {
            H2DE_Error::logError("Path isn't a directory");
        }
        return;
    }

    filesToLoad = getFilesToLoad(directory);
    assetsToLoad = filesToLoad.size();
    loadedAssets = 0;
    loading = true;

    if (sync) {
        loadAssetsSync(directory);
    } else {
        loadAssetsAsync(directory, progress, completed);
    }
}

void H2DE_AssetLoaderManager::loadAssetsSync(const std::filesystem::path& directory) {
    for (const std::filesystem::path& file : filesToLoad) {
        const H2DE_AssetLoaderManager::H2DE_LoadedAsset& loadedAsset = loadFile(file);

        switch (loadedAsset.type) {
            case H2DE_AssetLoaderManager::H2DE_ASSET_TYPE_SURFACE: surfaceBuffer[loadedAsset.name] = loadedAsset.surface; break;
            case H2DE_AssetLoaderManager::H2DE_ASSET_TYPE_SOUND: soundBuffer[loadedAsset.name] = loadedAsset.sound; break;
            default: break;
        }

        update();
        assetLoadedSync();
    }

    if (!silentLoad) {
        std::cout << std::endl;
        H2DE::print("H2DE => Loading complete");
    }

    loadedAssets = 0;
    assetsToLoad = 0;
    loading = false;
}

void H2DE_AssetLoaderManager::loadAssetsAsync(const std::filesystem::path& directory, const std::function<void(float)>& p, const std::function<void()>& completed) {
    progress = p;

    std::thread([this, completed]() {
        for (const std::filesystem::path& file : filesToLoad) {
            const H2DE_AssetLoaderManager::H2DE_LoadedAsset& loadedAsset = loadFile(file);
            
            {
                std::lock_guard<std::mutex> lock(loadMutex);

                if (loadedAsset.surface != nullptr) {
                    surfaceBuffer[loadedAsset.name] = loadedAsset.surface;
                }

                if (loadedAsset.sound != nullptr) {
                    soundBuffer[loadedAsset.name] = loadedAsset.sound;
                }
            }

            assetLoadedAsync();
        }

        {
            std::lock_guard<std::mutex> lock(loadMutex);
            loadedAssets = 0;
            assetsToLoad = 0;
            loading = false;
            progress = nullptr;

            if (completed) {
                completed();
            }

        }
    }).detach();
}

// -- load asset
H2DE_AssetLoaderManager::H2DE_LoadedAsset H2DE_AssetLoaderManager::loadFile(const std::filesystem::path& file) {
    const std::filesystem::path extension = file.extension();

    const bool isImg = (std::find(supportedImg.begin(), supportedImg.end(), extension) != supportedImg.end());
    const bool isSound = (std::find(supportedSound.begin(), supportedSound.end(), extension) != supportedSound.end());

    if (isImg) {
        return loadTexture(file);

    } else if (isSound) {
        return loadSound(file);

    } else {
        return H2DE_AssetLoaderManager::H2DE_LoadedAsset();
    }
}

H2DE_AssetLoaderManager::H2DE_LoadedAsset H2DE_AssetLoaderManager::loadTexture(const std::filesystem::path& file) {
    std::unordered_map<std::string, SDL_Texture*>& textures = engine->renderer->textures;
    SDL_Surface* surface = IMG_Load(file.string().c_str());
    const std::string name = file.filename().string();

    if (surface != nullptr) {
        if (textures.find(name) != textures.end()) {
            if (!silentLoad) {
                H2DE_Error::logWarning("Texture \"" + name + "\" has been overridden");
            }
        }
        
    } else {
        H2DE_Error::throwError("Texture \"" + name + "\" can't be loaded: " + SDL_GetError());
        return H2DE_AssetLoaderManager::H2DE_LoadedAsset();
    }

    return { H2DE_ASSET_TYPE_SURFACE, name, surface, nullptr };
}

H2DE_AssetLoaderManager::H2DE_LoadedAsset H2DE_AssetLoaderManager::loadSound(const std::filesystem::path& file) {
    std::unordered_map<std::string, Mix_Chunk*>& sounds = engine->audio->sounds;
    Mix_Chunk* sound = Mix_LoadWAV(file.string().c_str());
    const std::string name = file.filename().string();

    if (sound != nullptr) {
        if (sounds.find(name) != sounds.end()) {
            if (!silentLoad) {
                H2DE_Error::logWarning("Sound \"" + name + "\" has been overridden");
            }
        }

    } else {
        H2DE_Error::throwError("Sound \"" + name + "\" can't be loaded");
        return H2DE_AssetLoaderManager::H2DE_LoadedAsset();
    }

    return { H2DE_ASSET_TYPE_SOUND, name, nullptr, sound };
}

// -- asset loaded
void H2DE_AssetLoaderManager::assetLoadedSync() {
    constexpr uint8_t barWidth = 30;

    loadedAssets++;

    if (silentLoad) {
        return;
    }

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

void H2DE_AssetLoaderManager::assetLoadedAsync() {
    refreshObjectsSurfaceBuffers();

    if (!progress) {
        return;
    }

    loadedAssets++;
    const float blend = static_cast<float>(loadedAssets) / assetsToLoad;
    progress(blend);
}

// -- load font
void H2DE_AssetLoaderManager::loadFont(const std::string& name, const H2DE_Font& font) {
    if (engine->fonts.find(name) != engine->fonts.end()) {
        if (!silentLoad) {
            H2DE_Error::logWarning("Font \"" + name + "\" has been overridden");
        }
    }

    buildFontCache(font);
    engine->fonts[name] = font;
}

void H2DE_AssetLoaderManager::buildFontCache(const H2DE_Font& font) {
    const H2DE_PixelSize textureSize = engine->getTextureSize(font.textureName);
    const int& spacing = font.spacing;

    int lastSrcX = 0;

    for (const H2DE_Font::H2DE_Char& character : font.characters) {
        font._characters[std::string(1, character.character)] = { lastSrcX, 0, character.width, textureSize.y };
        lastSrcX += character.width + spacing;
    }
    
    font._characterHeight = textureSize.y;
    font._fixedCharacterHeight = textureSize.y - (font.ascent + font.descent);
}

// -- refresh
void H2DE_AssetLoaderManager::refreshObjectsSurfaceBuffers() {
    for (H2DE_Object* object : engine->objects) {
        object->refreshSurfaceBuffers();
    }
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
