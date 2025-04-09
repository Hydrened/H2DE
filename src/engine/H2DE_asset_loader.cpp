#include "H2DE/H2DE/H2DE_asset_loader.h"
#include "H2DE/H2DE/H2DE_renderer.h"
#include "H2DE/H2DE/H2DE_volume.h"

// INIT
H2DE_Engine::H2DE_AssetLoader::H2DE_AssetLoader(H2DE_Engine* e, SDL_Renderer* r) : engine(e), renderer(r) {

}

// CLEANUP
H2DE_Engine::H2DE_AssetLoader::~H2DE_AssetLoader() {

}

// LOAD
void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& directory) {
    if (!std::filesystem::exists(directory)) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: Asset directory not found" << std::endl;
        return;
    }

    if (!std::filesystem::is_directory(directory)) {
        std::cerr << "H2DE => \033[31mERROR\033[0m: Path isn't a directory" << std::endl;
        return;
    }

    std::vector<std::filesystem::path> filesToLoad = engine->assetLoader->getFilesToLoad(directory);
    engine->assetLoader->assetsToLoad = filesToLoad.size();

    for (std::filesystem::path file : filesToLoad) {
        engine->assetLoader->importFile(file);
    }

    engine->renderer->textures = engine->assetLoader->textureBuffer;
    engine->volume->sounds = engine->assetLoader->soundBuffer;
    
    std::cout << std::endl << "H2DE => Loading complete" << std::endl;
}

void H2DE_InitFont(H2DE_Engine* engine, const std::string& name, const H2DE_Font& font) {
    if (engine->renderer->fonts.find(name) != engine->renderer->fonts.end()) {
        std::cout << "H2DE => \033[33mWarning\033[0m: Font " << '"' << name << '"' << " has been overridden" << std::endl;
    }

    engine->renderer->fonts[name] = font;
}

// IMPORT
void H2DE_Engine::H2DE_AssetLoader::importFile(const std::filesystem::path& file) {
    std::filesystem::path extension = file.extension();

    if (std::find(supportedImg.begin(), supportedImg.end(), extension) != supportedImg.end()) {
        importTexture(file);
    } else if (std::find(supportedSound.begin(), supportedSound.end(), extension) != supportedSound.end()) {
        importSound(file);
    }
}

void H2DE_Engine::H2DE_AssetLoader::importTexture(const std::filesystem::path& file) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.string().c_str());
    std::string name = file.filename().string();

    if (texture != nullptr) {
        if (textureBuffer.find(name) != textureBuffer.end()) {
            std::cout << "H2DE => \033[33mWarning\033[0m: Texture " << '"' << name << '"' << " has been overridden" << std::endl;
        }

        textureBuffer[name] = texture;
        
    } else std::cerr << "H2DE => \033[31mERROR\033[0m: Texture " << '"' << name << '"' << " can't be loaded: " << SDL_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::H2DE_AssetLoader::importSound(const std::filesystem::path& file) {
    Mix_Chunk* sound = Mix_LoadWAV(file.string().c_str());
    std::string name = file.filename().string();

    if (sound != nullptr) {
        if (soundBuffer.find(name) != soundBuffer.end()) {
            std::cout << "H2DE => \033[33mWarning\033[0m: Sound " << '"' << name << '"' << " has been overridden" << std::endl;
        }
    
        soundBuffer[name] = sound;

    } else std::cerr << "H2DE => \033[31mERROR\033[0m: Sound " << '"' << name << '"' << " can't be loaded" << std::endl;
    assetImported();
}

void H2DE_Engine::H2DE_AssetLoader::assetImported() {
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

// GETTER
std::vector<std::filesystem::path> H2DE_Engine::H2DE_AssetLoader::getFilesToLoad(const std::filesystem::path& directory) const {
    std::vector<std::filesystem::path> res = {};

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            std::string extension = entry.path().extension().string();

            bool supported = (
                std::find(supportedImg.begin(), supportedImg.end(), extension) != supportedImg.end() ||
                std::find(supportedSound.begin(), supportedSound.end(), extension) != supportedSound.end()
            );

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
