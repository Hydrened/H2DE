#include "H2DE/H2DE_asset_loader.h"
#include "H2DE/H2DE_renderer.h"

// INIT
H2DE_Engine::H2DE_AssetLoader::H2DE_AssetLoader(H2DE_Engine* e, SDL_Renderer* r) : engine(e), renderer(r) {

}

// CLEANUP
H2DE_Engine::H2DE_AssetLoader::~H2DE_AssetLoader() {

}

// LOAD
void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory)) {
        std::cerr << "H2DE => ERROR: Asset directory not found" << std::endl;
        return;
    }

    if (!std::filesystem::is_directory(directory)) {
        std::cerr << "H2DE => ERROR: Path isn't a directory" << std::endl;
        return;
    }

    std::vector<std::filesystem::path> filesToLoad = engine->assetLoader->getFilesToLoad(directory);
    engine->assetLoader->assetsToLoad = filesToLoad.size();

    for (std::filesystem::path file : filesToLoad) {
        engine->assetLoader->importFile(file);
    }

    engine->renderer->textures = engine->assetLoader->textureBuffer;
    engine->renderer->sounds = engine->assetLoader->soundBuffer;
    
    std::cout << "H2DE => Loading complete" << std::endl;
}

// IMPORT
void H2DE_Engine::H2DE_AssetLoader::importFile(const std::filesystem::path& file) {
    std::filesystem::path extension = file.extension();

    if (extension == ".png" || extension == ".jpg") {
        importTexture(file);
    } else if (extension == ".mp3" || extension == ".ogg") {
        importSound(file);
    }
}

void H2DE_Engine::H2DE_AssetLoader::importTexture(const std::filesystem::path& file) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.string().c_str());
    std::string name = file.filename().string();

    if (texture != nullptr) {
        if (textureBuffer.find(name) != textureBuffer.end()) {
            std::cout << "H2DE => Warning: Texture " << '"' << name << '"' << " has been overridden" << std::endl;
        }

        textureBuffer[name] = texture;
        
    } else std::cerr << "H2DE => ERROR: Texture " << '"' << name << '"' << " can't be loaded" << std::endl;
    assetImported();
}

void H2DE_Engine::H2DE_AssetLoader::importSound(const std::filesystem::path& file) {
    Mix_Chunk* sound = Mix_LoadWAV(file.string().c_str());
    std::string name = file.filename().string();

    if (sound != nullptr) {
        if (soundBuffer.find(name) != soundBuffer.end()) {
            std::cout << "H2DE => Warning: Sound " << '"' << name << '"' << " has been overridden" << std::endl;
        }
    
        soundBuffer[name] = sound;

    } else std::cerr << "H2DE => ERROR: Sound " << '"' << name << '"' << " can't be loaded" << std::endl;
    assetImported();
}

void H2DE_Engine::H2DE_AssetLoader::assetImported() {
    loadedAssets++;
    float percentage = static_cast<int>(static_cast<float>(loadedAssets) / assetsToLoad * 10000.0f) / 100.0f;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << percentage;
    std::cout << "H2DE => Loading: " << oss.str() << "%" << std::endl;
}

// GETTER
std::vector<std::filesystem::path> H2DE_Engine::H2DE_AssetLoader::getFilesToLoad(const std::filesystem::path& directory) const {
    std::vector<std::filesystem::path> res = {};

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            std::string extension = entry.path().extension().string();
            
            if (extension == ".png" || extension == ".jpg"  || extension == ".mp3" || extension == ".ogg") {
                res.push_back(entry.path());
            }

        } else if (std::filesystem::is_directory(entry.status())) {
            std::vector<std::filesystem::path> children = getFilesToLoad(entry.path());
            res.insert(res.end(), children.begin(), children.end());
        }
    }

    return res;
}
