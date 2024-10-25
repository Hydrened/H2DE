#include <H2DE/H2DE_engine.h>

// CONSTRUCTOR AND DECONSTRUCTOR
H2DE_Engine::H2DE_Engine(SDL_Renderer* r, int w, int h, int f) : renderer(r), size({ w, h }), fps(f) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error("SDL_Init  failed: " + std::string(SDL_GetError()));
    else if (SDL_Init(SDL_INIT_AUDIO) != 0) throw std::runtime_error("ENGINE => SDL_Init failed: " + std::string(SDL_GetError()));
    else if (!IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG) throw std::runtime_error("IMG_Init  failed: " + std::string(IMG_GetError()));
    else if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) throw std::runtime_error("Mix_Init  failed: " + std::string(Mix_GetError()));
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) throw std::runtime_error("Mix_OpenAudio  failed: " + std::string(Mix_GetError()));
    else if (TTF_Init() == -1) throw std::runtime_error("TTF_Init  failed: " + std::string(TTF_GetError()));
}

H2DE_Engine::~H2DE_Engine() {
    for (const auto& pair : textures) if (pair.second != nullptr) SDL_DestroyTexture(pair.second);
    textures.clear();
    for (const auto& pair : sounds) if (pair.second != nullptr) Mix_FreeChunk(pair.second);
    sounds.clear();
    for (const auto& pair : fonts) if (pair.second != nullptr) TTF_CloseFont(pair.second);
    fonts.clear();
    for (H2DE_GraphicObject* g : graphicObjects) H2DE_DestroyGraphicObject(g);
    graphicObjects.clear();
    IMG_Quit();
    Mix_CloseAudio();
    TTF_Quit();
}

// INIT
H2DE_Engine* H2DE_CreateEngine(SDL_Renderer* renderer, int w, int h, int fps) {
    return new H2DE_Engine(renderer, w, h, fps);
};

void H2DE_DebugEngineRendering(H2DE_Engine* engine, bool active) {
    engine->debug = active;
}

// CLEANUP
void H2DE_DestroyEngine(H2DE_Engine* engine) {
    delete engine;
}

// ASSETS
void H2DE_LoadAssets(H2DE_Engine* engine, const fs::path& dir) {
    if (fs::exists(dir)) {
        if (fs::is_directory(dir)) {
            engine->dataToLoad = H2DE_Engine::countFiles(dir);
            engine->importFiles(dir);
            std::cout << "ENGINE => Loading complete" << std::endl;
        } else std::cerr << "ENGINE => ERROR: Path isn't a directory" << std::endl;
    } else std::cerr << "ENGINE => ERROR: Asset directory not found" << std::endl;
}

int H2DE_Engine::countFiles(const fs::path& dir) {
    int count = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (fs::is_regular_file(entry.status())) {
            std::string extension = entry.path().extension().string();
            if (extension == ".png" || extension == ".mp3" || extension == ".ogg" || extension == ".ttf") count++;
        } else if (fs::is_directory(entry.status())) count += countFiles(entry.path());
    } return count;
}

void H2DE_Engine::importFiles(const fs::path& parentDir) {
    for (const auto& entry : fs::directory_iterator(parentDir)) {
        if (fs::is_directory(entry.status())) importFiles(entry.path());
        else if (fs::is_regular_file(entry.status())) {
            fs::path extension = entry.path().extension();
            if (extension == ".png") importTexture(entry.path());
            else if (extension == ".mp3" || extension == ".ogg") importSound(entry.path());
            else if (extension == ".ttf") importFont(entry.path());
        }
    }
}

void H2DE_Engine::importTexture(const fs::path& img) {
    size_t startNameIndex = img.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = img.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    SDL_Texture* texture = H2DE_Loader::loadTexture(renderer, img.string().c_str());
    if (texture != nullptr) textures[file] = texture;
    else std::cerr << "ENGINE => IMG_Load failed: " << SDL_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::importSound(const fs::path& song) {
    size_t startNameIndex = song.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = song.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    Mix_Chunk* music = H2DE_Loader::loadSound(song.string().c_str());
    if (music != nullptr) sounds[file] = music;
    else std::cerr << "ENGINE => Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::importFont(const fs::path& font) {
    size_t startNameIndex = font.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = font.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    TTF_Font* ttf = H2DE_Loader::loadFont(font.string().c_str());
    if (ttf != nullptr) fonts[file] = ttf;
    else std::cerr << "ENGINE => TTF_OpenFont failed: " << TTF_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::assetImported() {
    loadedData++;
    float percentage = round(static_cast<float>(loadedData) / static_cast<float>(dataToLoad) * 100 * 100) / 100;
    std::cout << "ENGINE => Loading: " << std::to_string(percentage).substr(0, std::to_string(percentage).size() - 4) << "%" << std::endl;
}

// REPEAT
std::vector<H2DE_GraphicObject*> H2DE_Engine::getRepeatXGraphicObjects() {
    std::vector<H2DE_GraphicObject*> res;
    for (H2DE_GraphicObject* g : graphicObjects) if (g->repeatX) {
        int leftNb = ceil(static_cast<float>(g->pos.x) / (g->size.w * g->scale.x));
        for (int i = 0; i < leftNb; i++) {
            int posX = g->pos.x - g->size.w * g->scale.x * (i + 1);
            if (posX - g->size.w >= size.w) continue;
            H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
            copy->pos.x = posX;
            copy->repeatX = false;
            res.push_back(copy);
        }
        int rightNb = ceil(static_cast<float>(size.w - g->pos.x - g->size.w * g->scale.x) / (g->size.w * g->scale.x)) + 1;
        for (int i = 0; i < rightNb; i++) {
            int posX = g->pos.x + g->size.w * g->scale.x * (i + 1);
            if (posX + g->size.w < 0) continue;
            H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
            copy->pos.x = posX;
            copy->repeatX = false;
            res.push_back(copy);
        }
    }
    return res;
}

std::vector<H2DE_GraphicObject*> H2DE_Engine::getRepeatYGraphicObjects() {
    std::vector<H2DE_GraphicObject*> res;
    for (H2DE_GraphicObject* g : graphicObjects) if (g->repeatY) {
        int topNb = ceil(static_cast<float>(g->pos.y) / (g->size.h * g->scale.y));
        for (int i = 0; i < topNb; i++) {
            int posY = g->pos.y - g->size.h * g->scale.y * (i + 1);
            if (posY - g->size.h >= size.h) continue;
            H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
            copy->pos.y = posY;
            copy->repeatY = false;
            res.push_back(copy);
        }
        int bottomNb = ceil(static_cast<float>(size.h - g->pos.y - g->size.h * g->scale.y) / (g->size.h * g->scale.y)) + 1;
        for (int i = 0; i < bottomNb; i++) {
            int posY = g->pos.y + g->size.h * g->scale.y * (i + 1);
            if (posY + g->size.h < 0) continue;
            H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
            copy->pos.y = posY;
            copy->repeatY = false;
            res.push_back(copy);
        }
    }
    return res;
}

// GRAPHIC OBJECT
void H2DE_AddGraphicObject(H2DE_Engine* engine, H2DE_GraphicObject* g) {
    bool canAdd = true;
    switch (g->type) {
        case IMAGE:
            if (g->texture == "") canAdd = false;
            if (g->size.w == 0 || g->size.h == 0) canAdd = false;
            break;
        case POLYGON:
            if (g->points.size() < 3) canAdd = false;
            break;
        case CIRCLE:
            if (g->radius < 1) canAdd = false;
            break;
        case TEXT:
            if (g->text == "") canAdd = false;
            if (g->font == "") canAdd = false;
            break;
        default: canAdd = false; break;
    }
    if (canAdd) engine->graphicObjects.push_back(g);
}

void H2DE_Click(H2DE_Engine* engine, int x, int y) {
    H2DE_SClick* c = new H2DE_SClick();
    c->x = x;
    c->y = y;

    engine->click = c;
}

bool H2DE_Engine::isElementClicked(H2DE_GraphicObject* g) {
    H2DE_Pos pos;
    switch (g->type) {
        case CIRCLE: pos = { g->pos.x - (int)(g->radius), g->pos.y - (int)(g->radius) }; break;
        default: pos = g->pos; break;
    }

    H2DE_Size size;
    switch (g->type) {
        case POLYGON: size = H2DE_Calculator::getPolygonSize(g->points); break;
        case CIRCLE: size = { (int)(g->radius) * 2, (int)(g->radius) * 2 }; break;
        default: size = g->size; break;
    }

    int x = click.value()->x;
    int y = click.value()->y;

    return (
        x > pos.x && x < pos.x + size.w &&
        y > pos.y && y < pos.y + size.h
    );
}

// RENDER
void H2DE_RenderEngine(H2DE_Engine* engine) { 
    static SDL_Renderer* renderer = engine->renderer;
    std::vector<H2DE_GraphicObject*> graphicObjects = engine->graphicObjects;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    std::vector<H2DE_GraphicObject*> repeatXGraphicObjects = engine->getRepeatXGraphicObjects();
    graphicObjects.insert(graphicObjects.end(), repeatXGraphicObjects.begin(), repeatXGraphicObjects.end());

    std::vector<H2DE_GraphicObject*> repeatYGraphicObjects = engine->getRepeatYGraphicObjects();
    graphicObjects.insert(graphicObjects.end(), repeatYGraphicObjects.begin(), repeatYGraphicObjects.end());

    sort(graphicObjects.begin(), graphicObjects.end(), &H2DE_Calculator::isIndexGreater);
    if (engine->debug) std::cout << "ENGINE => rendering " << graphicObjects.size() << " object(s)" << std::endl; 

    H2DE_GraphicObject* clickedElement = nullptr;
    for (H2DE_GraphicObject* g : graphicObjects) {
        switch (g->type) {
            case IMAGE: engine->renderImage(g); break;
            case POLYGON: engine->renderPolygon(g); break;
            case CIRCLE: engine->renderCircle(g); break;
            case TEXT: engine->renderText(g); break;
            default: break;
        }
        if (engine->debug) {
            if (g->rotation != 0.0f) engine->renderPixel({ g->pos.x + g->rotationOrigin.x, g->pos.y + g->rotationOrigin.y }, { 255, 0, 0, 255 });
            if (g->scale.x != 1.0f || g->scale.y != 1.0f) engine->renderPixel({ g->pos.x + g->scaleOrigin.x, g->pos.y + g->scaleOrigin.y }, { 0, 255, 0, 255 });
        }
        if (engine->click.has_value()) if (engine->isElementClicked(g)) clickedElement = g;
    }
    if (clickedElement) if (clickedElement->onclick.has_value()) clickedElement->onclick.value()();
    SDL_RenderPresent(renderer);
    
    engine->click = std::nullopt;

    for (H2DE_GraphicObject* g : graphicObjects) H2DE_DestroyGraphicObject(g);
    engine->graphicObjects.clear();
}

void H2DE_Engine::renderPixel(H2DE_Pos pos, H2DE_RGB rgb) {
    std::vector<Sint16> vx(4);
    std::vector<Sint16> vy(4);

    vx[0] = pos.x;
    vy[0] = pos.y;
    vx[1] = pos.x + 1;
    vy[1] = pos.y;
    vx[2] = pos.x + 1;
    vy[2] = pos.y + 1;
    vx[3] = pos.x;
    vy[3] = pos.y + 1;

    filledPolygonColor(renderer, vx.data(), vy.data(), 4, static_cast<Uint32>(rgb));
}

void H2DE_Engine::renderImage(H2DE_GraphicObject* g) {
    std::unordered_map<std::string, SDL_Texture*>::iterator textureIterator = textures.find(g->texture);
    if (textureIterator != textures.end()) {

        H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
        pos = H2DE_Calculator::getRescaledPos(pos, g->size, g->scaleOrigin, g->scale);
        H2DE_Size size = H2DE_Calculator::getRescaledSize(g->size, g->scale);
        SDL_Rect destRect = { pos.x, pos.y, size.w, size.h };

        H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
        SDL_Point convertedRotationOrigin = static_cast<SDL_Point>(rotationOrigin);

        SDL_Texture* texture = textureIterator->second;

        SDL_SetTextureColorMod(texture, g->rgb.r, g->rgb.g, g->rgb.b);
        SDL_SetTextureAlphaMod(texture, g->rgb.a);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);

        if (g->srcRect.has_value()) {
            SDL_Rect srcRect = (SDL_Rect)g->srcRect.value();
            SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, g->rotation, &convertedRotationOrigin, g->flip);
        } else SDL_RenderCopyEx(renderer, texture, NULL, &destRect, g->rotation, &convertedRotationOrigin, g->flip);
    }
}

void H2DE_Engine::renderPolygon(H2DE_GraphicObject* g) {
    size_t nbPoints = g->points.size();
    std::vector<Sint16> vx(nbPoints);
    std::vector<Sint16> vy(nbPoints);
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
    H2DE_Size size = H2DE_Calculator::getPolygonSize(g->points);


    H2DE_GraphicObject* parent = g->parent;
    H2DE_Pos posOffset = g->pos;
    while (parent) {
        H2DE_Pos rOrigin = { parent->scaleOrigin.x - posOffset.x, parent->scaleOrigin.y - posOffset.y };
        pos = H2DE_Calculator::getRescaledPos(pos, size, rOrigin, parent->scale);

        scaleX *= parent->scale.x;
        scaleY *= parent->scale.y;

        posOffset.x += parent->pos.x;
        posOffset.y += parent->pos.y;
        parent = parent->parent;
    }

    pos = H2DE_Calculator::getRescaledPos(pos, size, { (int)(g->scaleOrigin.x * scaleX), (int)(g->scaleOrigin.y * scaleY) }, g->scale);
    for (int i = 0; i < nbPoints; i++) {
        vx[i] = pos.x + g->points[i].x * scaleX * g->scale.x;
        vy[i] = pos.y + g->points[i].y * scaleY * g->scale.y;
    }



    // H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
    // rotationOrigin = { rotationOrigin.x + pos.x, rotationOrigin.y + pos.y };

    // for (int i = 0; i < nbPoints; i++) {
    //     H2DE_Pos pointPos = { static_cast<int>(pos.x + g->points[i].x * g->scale.x), static_cast<int>(pos.y + g->points[i].y * g->scale.y) };
    //     // pointPos = H2DE_Calculator::applyRotationOnPos(pointPos, rotationOrigin, g->rotation);
    //     vx[i] = pointPos.x;
    //     vy[i] = pointPos.y;
    // }

    if (g->filled) filledPolygonColor(renderer, vx.data(), vy.data(), nbPoints, static_cast<Uint32>(g->rgb));
    else polygonColor(renderer, vx.data(), vy.data(), nbPoints, static_cast<Uint32>(g->rgb));
}

void H2DE_Engine::renderCircle(H2DE_GraphicObject* g) {
    H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
    pos = H2DE_Calculator::getRescaledPos(pos, { static_cast<int>(g->radius) * 2, static_cast<int>(g->radius) * 2 }, g->scaleOrigin, g->scale);
    
    H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
    rotationOrigin = { rotationOrigin.x + pos.x, rotationOrigin.y + pos.y };

    pos = H2DE_Calculator::applyRotationOnPos(pos, rotationOrigin, g->rotation);

    if (g->filled) filledCircleColor(renderer, pos.x, pos.y, g->radius, static_cast<Uint32>(g->rgb));
    else circleColor(renderer, pos.x, pos.y, g->radius, static_cast<Uint32>(g->rgb));
}

void H2DE_Engine::renderText(H2DE_GraphicObject* g) {
    std::unordered_map<std::string, TTF_Font*>::iterator fontIterator = fonts.find(g->font);
    if (fontIterator != fonts.end()) {

        H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
        pos = H2DE_Calculator::getRescaledPos(pos, g->size, g->scaleOrigin, g->scale);
        H2DE_Size size = H2DE_Calculator::getRescaledSize(g->size, g->scale);
        SDL_Rect destRect = { pos.x, pos.y, size.w, size.h };

        H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
        SDL_Point convertedRotationOrigin = static_cast<SDL_Point>(rotationOrigin);

        TTF_Font* font = fontIterator->second;
        SDL_Surface* surface = TTF_RenderText_Solid(font, g->text, g->rgb);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_FreeSurface(surface);
        SDL_SetTextureAlphaMod(texture, g->rgb.a);
        SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
        SDL_RenderCopyEx(renderer, texture, NULL, &destRect, g->rotation, &convertedRotationOrigin, g->flip);
    }
}

// SOUND
void H2DE_SetSoundVolume(H2DE_Engine* engine, int channel, int volume) {
    volume = std::clamp(volume, 0, 100);
    Mix_Volume(channel, static_cast<int>((volume / 100.0f) * 128));
}

void H2DE_PlaySound(H2DE_Engine* engine, int channel, std::string sound, int loop) {
    std::unordered_map<std::string, Mix_Chunk*> sounds = engine->sounds;

    if (sounds.find(sound) != sounds.end()) {
        if ( Mix_PlayChannel(channel, sounds[sound], loop) == -1) std::cerr << "ENGINE => Mix_PlayChannel failed: " << Mix_GetError() << std::endl;
    } else std::cerr << "ENGINE => Sound '" << sound << "' not found" << std::endl;
}

void H2DE_PauseSound(H2DE_Engine* engine, int channel) {
    Mix_Pause(channel);
}

void H2DE_ResumeSound(H2DE_Engine* engine, int channel) {
    Mix_Resume(channel);
}

// GETTER
H2DE_Size H2DE_GetEngineSize(H2DE_Engine* engine) {
    return engine->size;
}

H2DE_Size H2DE_GetEngineMaximumSize(H2DE_Engine* engine) {
    return engine->maxSize;
}

int H2DE_GetEngineFPS(H2DE_Engine* engine) {
    return engine->fps;
}

// SETTER 
void H2DE_SetEngineSize(H2DE_Engine* engine, int w, int h) {
    engine->size.w = std::clamp(w, 0, (engine->maxSize.w == -1) ? INT_MAX : engine->maxSize.w);
    engine->size.h = std::clamp(h, 0, (engine->maxSize.h == -1) ? INT_MAX : engine->maxSize.h);
}

void H2DE_SetEngineMaximumSize(H2DE_Engine* engine, int w, int h) {
    engine->maxSize.w = std::clamp(w, 0, INT_MAX);
    engine->maxSize.h = std::clamp(h, 0, INT_MAX);
    H2DE_Size currentSize = H2DE_GetEngineSize(engine);
    H2DE_SetEngineSize(engine, currentSize.w, currentSize.h);
}

H2DE_Timeline* H2DE_CreateTimeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loop) {
    H2DE_Timeline* timeline = new H2DE_Timeline(engine, duration, effect, update, completed, loop);
    return timeline;
}
