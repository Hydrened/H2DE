#include "H2DE/H2DE_engine.h"

// INIT
H2DE_Engine::H2DE_Engine(H2DE_EngineData d) : data(d), fps(data.fps) {
    try {
        static bool once = false;
        if (once) throw std::runtime_error("H2DE-108: Can't create more than one engine");
        once = true;

        window = new H2DE_Window(this, data.window);
        renderer = new H2DE_Renderer(this, &textures, &objects, &buttons, &fonts);
        settings = new H2DE_Settings();
        camera = new H2DE_Camera(this, data.camera);
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

H2DE_Engine* H2DE_CreateEngine(H2DE_EngineData data) {
    return new H2DE_Engine(data);
}

// CLEANUP
H2DE_Engine::~H2DE_Engine() {
    isRunning = false;
    
    for (const auto& [key, texture] : textures) if (texture != nullptr) SDL_DestroyTexture(texture);
    textures.clear();

    for (const auto& [key, sound] : sounds) if (sound != nullptr) Mix_FreeChunk(sound);
    sounds.clear();

    for (H2DE_LevelObject* object : objects) delete object;
    objects.clear();

    delete settings;
    delete gameData;
    delete camera;
    delete renderer;
    delete window;
}

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    delete engine;
}

// EVENTS
void H2DE_RunEngine(H2DE_Engine* engine) {
    Uint32 now = SDL_GetTicks();
    int frameTime;
    SDL_Event event;

    try {
        while (engine->isRunning) {
            now = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: engine->isRunning = false; break;
                    case SDL_MOUSEBUTTONDOWN: engine->click(event.button.x, event.button.y); break;
                    default: break;
                }

                if (engine->handleEvents) engine->handleEvents(event);
            }

            if (!engine->paused) {
                if (engine->update) engine->update();
                engine->updateLevelObjects();
                engine->camera->update();
            }

            engine->renderer->render();
            
            frameTime = SDL_GetTicks() - now;
            engine->currentFPS = 1000.0f / static_cast<float>((frameTime > 0) ? frameTime : 1);
            int timePerFrame = 1000 / engine->fps;
            if (timePerFrame >= frameTime) SDL_Delay(timePerFrame - frameTime);
        }
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
    }
}

void H2DE_StopEngine(H2DE_Engine* engine) {
    engine->isRunning = false;
}

void H2DE_DebugObjectNumber(H2DE_Engine* engine, bool state) {
    engine->renderer->debugObjectNumber(state);
}

void H2DE_DebugObjectHitboxes(H2DE_Engine* engine, bool state) {
    engine->renderer->debugObjectHitboxes(state);
}

void H2DE_Engine::H2DE_Delay(unsigned int ms, std::function<void()> callback) {
    std::thread([ms, callback]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        callback();
    }).detach();
}

void H2DE_Engine::click(int x, int y) {
    H2DE_AbsPos clickPos = { x, y };
    int blockSize = renderer->getBlockSize();

    for (H2DE_Button* button : buttons) {
        H2DE_ButtonData* btnData = H2DE_GetButtonData(button);
        if (!btnData->onClick) continue;

        H2DE_TextureData* btnTextureData = H2DE_GetTextureData(btnData->texture);

        H2DE_AbsPos pos = H2DE_AbsPos{ static_cast<int>(btnData->pos.x * blockSize), static_cast<int>(btnData->pos.y * blockSize) };
        H2DE_AbsSize size = H2DE_AbsSize{ static_cast<int>(btnTextureData->size.w * blockSize), static_cast<int>(btnTextureData->size.h * blockSize) };
        H2DE_AbsRect btnRect = pos.makeRect(size);

        if (btnRect.contains(clickPos)) btnData->onClick();
    }
}

void H2DE_Pause(H2DE_Engine* engine) {
    engine->paused = true;
}

void H2DE_Resume(H2DE_Engine* engine) {
    engine->paused = false;
}

bool H2DE_IsPaused(H2DE_Engine* engine) {
    return engine->paused;
}

// UPDATE
void H2DE_Engine::updateLevelObjects() {
    for (H2DE_LevelObject* object : objects) object->update();
}

// ASSETS
void H2DE_LoadAssets(H2DE_Engine* engine, const std::filesystem::path& dir) {
    if (std::filesystem::exists(dir)) {
        engine->loadedData = 0;
        if (std::filesystem::is_directory(dir)) {
            engine->dataToLoad = H2DE_Engine::countFilesToLoad(dir);
            engine->importFiles(dir);
            std::cout << "H2DE => Loading complete" << std::endl;
        } else std::cerr << "H2DE => ERROR: Path isn't a directory" << std::endl;
    } else std::cerr << "H2DE => ERROR: Asset directory not found" << std::endl;
}

void H2DE_LoadAsset(H2DE_Engine* engine, const std::filesystem::path& file) {
    if (std::filesystem::exists(file)) {
        engine->loadedData = 0;
        engine->dataToLoad = 1;
        engine->importFile(file);
        std::cout << "H2DE => Loading complete" << std::endl;
    } else std::cerr << "H2DE => ERROR: file not found" << std::endl;
}

void H2DE_RemoveAssets(H2DE_Engine* engine) {
    for (const auto& [name, texture] : engine->textures) H2DE_RemoveAsset(engine, name);
    for (const auto& [name, sound] : engine->sounds) H2DE_RemoveAsset(engine, name);
}

void H2DE_RemoveAsset(H2DE_Engine* engine, const std::filesystem::path& file) {
    std::filesystem::path extension = file.extension();
    if (extension == ".png" || extension == ".jpg") {
        if (engine->textures.erase(file.string())) {
            std::cout << "H2DE => Removed texture " << file << " from engine" << std::endl;
        }
    } else if (extension == ".mp3" || extension == ".ogg") {
        if (engine->sounds.erase(file.string())) {
            std::cout << "H2DE => Removed sound " << file << " from engine" << std::endl;
        }
    }
}

int H2DE_Engine::countFilesToLoad(const std::filesystem::path& dir) {
    int count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (std::filesystem::is_regular_file(entry.status())) {
            std::string extension = entry.path().extension().string();
            if (extension == ".png" || extension == ".jpg"  || extension == ".mp3" || extension == ".ogg") count++;
        } else if (std::filesystem::is_directory(entry.status())) count += countFilesToLoad(entry.path());
    } return count;
}

void H2DE_Engine::importFiles(const std::filesystem::path& dir) {
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (std::filesystem::is_directory(entry.status())) importFiles(entry.path());
        else if (std::filesystem::is_regular_file(entry.status())) importFile(entry.path());
    }
}

void H2DE_Engine::importFile(const std::filesystem::path& file) {
    std::filesystem::path extension = file.extension();
    if (extension == ".png" || extension == ".jpg") importTexture(file);
    else if (extension == ".mp3" || extension == ".ogg") importSound(file);
}

void H2DE_Engine::importTexture(const std::filesystem::path& img) {
    size_t startNameIndex = img.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = img.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    SDL_Texture* texture = H2DE_AssetLoader::loadTexture(H2DE_GetWindowsRenderer(window), img.string().c_str());
    if (texture != nullptr) {
        if (textures.find(file) != textures.end()) std::cerr << "H2DE => WARNING: Asset " << '"' << file << '"' << " has been replaced" << std::endl;
        textures[file] = texture;
    } else std::cerr << "H2DE => IMG_Load failed: " << SDL_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::importSound(const std::filesystem::path& song) {
    size_t startNameIndex = song.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = song.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    Mix_Chunk* music = H2DE_AssetLoader::loadSound(song.string().c_str());
    if (music != nullptr) {
        if (sounds.find(file) != sounds.end()) std::cerr << "H2DE => WARNING: Asset " << '"' << file << '"' << " has been replaced" << std::endl;
        sounds[file] = music;
    } else std::cerr << "H2DE => Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::assetImported() {
    loadedData++;
    float percentage = round(static_cast<float>(loadedData) / static_cast<float>(dataToLoad) * 100 * 100) / 100;
    std::cout << "H2DE => Loading: " << std::to_string(percentage).substr(0, std::to_string(percentage).size() - 4) << "%" << std::endl;
}

// OBJECTS
H2DE_LevelObject* H2DE_CreateLevelObject(H2DE_Engine* engine, H2DE_LevelObjectData data) {
    H2DE_LevelObject* obj = new H2DE_LevelObject(engine, &engine->objects, data);
    engine->objects.push_back(obj);
    return obj;
}

void H2DE_DestroyLevelObject(H2DE_Engine* engine, H2DE_LevelObject* object) {
    auto it = std::find(engine->objects.begin(), engine->objects.end(), object);
    if (it != engine->objects.end()) {
        delete object;
        engine->objects.erase(it);
    }
}

// BUTTONS
H2DE_Button* H2DE_CreateButton(H2DE_Engine* engine, H2DE_ButtonData data) {
    H2DE_Button* btn = new H2DE_Button(engine, data);
    engine->buttons.push_back(btn);
    return btn;
}

void H2DE_DestroyButton(H2DE_Engine* engine, H2DE_Button* button) {
    auto it = std::find(engine->buttons.begin(), engine->buttons.end(), button);
    if (it != engine->buttons.end()) {
        delete button;
        engine->buttons.erase(it);
    }
}

// SOUNDS
void H2DE_SetVolumeSound(int channel, int volume) {
    volume = std::clamp(volume, 0, 100);
    Mix_Volume(channel, static_cast<int>((volume / 100.0f) * 128));
}

int H2DE_PlaySound(H2DE_Engine* engine, int channel, std::string sound, int loop) {
    std::unordered_map<std::string, Mix_Chunk*> sounds = engine->sounds;

    int res = -1;
    if (sounds.find(sound) != sounds.end()) {
        res = Mix_PlayChannel(channel, sounds[sound], loop);
        if (res == -1) std::cerr << "H2DE => Mix_PlayChannel failed: " << Mix_GetError() << std::endl;
    } else std::cerr << "H2DE => Sound '" << sound << "' not found" << std::endl;
    return res;
}

void H2DE_PauseSound(H2DE_Engine* engine, int channel) {
    Mix_Pause(channel);
}

void H2DE_ResumeSound(H2DE_Engine* engine, int channel) {
    Mix_Resume(channel);
}

// FONTS
void H2DE_InitFont(H2DE_Engine* engine, std::string name, H2DE_Font* font) {
    if (engine->fonts.find(name) != engine->fonts.end()) std::cerr << "H2DE => WARNING: Font " << '"' << name << '"' << " has been replaced" << std::endl;
    engine->fonts[name] = font;
}

// GETTER
H2DE_Window* H2DE_GetWindow(H2DE_Engine* engine) {
    return engine->window;
}

int H2DE_GetFps(H2DE_Engine* engine) {
    return engine->fps;
}

int H2DE_GetSteps(H2DE_Engine* engine, unsigned int ms) {
    return std::round(engine->fps / 1000.0f * ms);
}

int H2DE_GetCurrentFps(H2DE_Engine* engine) {
    return engine->currentFPS;
}

H2DE_GameData* H2DE_GetGameData(H2DE_Engine* engine) {
    return engine->gameData;
}

H2DE_Settings* H2DE_GetSettings(H2DE_Engine* engine) {
    return engine->settings;
}

H2DE_Camera* H2DE_GetCamera(H2DE_Engine* engine) {
    return engine->camera;
}

H2DE_LevelPos H2DE_ConvertToLevelPos(H2DE_Engine* engine, H2DE_AbsPos pos) {
    int blockSize = engine->renderer->getBlockSize();
    H2DE_LevelPos camPos = H2DE_GetCameraPos(engine->camera);

    return {
        (float)pos.x / blockSize + camPos.x,
        (float)pos.y / blockSize + camPos.y
    };
}

// SETTER
void H2DE_SetFps(H2DE_Engine* engine, unsigned int fps) {
    engine->fps = fps;
}

void H2DE_SetGameHandleEventCall(H2DE_Engine* engine, std::function<void(SDL_Event)> call) {
    engine->handleEvents = call;
}

void H2DE_SetGameUpdateCall(H2DE_Engine* engine, std::function<void()> call) {
    engine->update = call;
}
