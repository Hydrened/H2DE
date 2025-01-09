#include <H2DE/H2DE_engine.h>

// INIT
H2DE_Engine::H2DE_Engine(SDL_Renderer* r, int w, int h, int f) : renderer(r), size({ w, h }), fps(f) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error("SDL_Init  failed: " + std::string(SDL_GetError()));
    else if (SDL_Init(SDL_INIT_AUDIO) != 0) throw std::runtime_error("ENGINE => SDL_Init failed: " + std::string(SDL_GetError()));
    else if (!IMG_Init(IMG_INIT_PNG) && IMG_INIT_PNG) throw std::runtime_error("IMG_Init  failed: " + std::string(IMG_GetError()));
    else if (!(Mix_Init(MIX_INIT_MP3) & MIX_INIT_MP3)) throw std::runtime_error("Mix_Init  failed: " + std::string(Mix_GetError()));
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0) throw std::runtime_error("Mix_OpenAudio  failed: " + std::string(Mix_GetError()));
    else if (TTF_Init() == -1) throw std::runtime_error("TTF_Init  failed: " + std::string(TTF_GetError()));
}

H2DE_Engine* H2DE_CreateEngine(SDL_Renderer* renderer, int w, int h, int fps) {
    return new H2DE_Engine(renderer, w, h, fps);
};

// CLEANUP
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

void H2DE_DestroyEngine(H2DE_Engine* engine) {
    delete engine;
}

// DEBUG
void H2DE_DebugGraphicObjects(H2DE_Engine* engine, bool active) {
    engine->debug->graphicObjects = active;
}

void H2DE_DebugScaleOrigins(H2DE_Engine* engine, bool active) {
    engine->debug->scaleOrigins = active;
}

void H2DE_DebugRotationOrigins(H2DE_Engine* engine, bool active) {
    engine->debug->rotationOrigins = active;
}

// ASSETS
void H2DE_LoadAssets(H2DE_Engine* engine, const fs::path& dir) {
    if (fs::exists(dir)) {
        engine->loadedData = 0;
        if (fs::is_directory(dir)) {
            engine->dataToLoad = H2DE_Engine::countFiles(dir);
            engine->importFiles(dir);
            std::cout << "ENGINE => Loading complete" << std::endl;
        } else std::cerr << "ENGINE => ERROR: Path isn't a directory" << std::endl;
    } else std::cerr << "ENGINE => ERROR: Asset directory not found" << std::endl;
}

void H2DE_LoadAsset(H2DE_Engine* engine, const fs::path& file) {
    if (fs::exists(file)) {
        engine->loadedData = 0;
        engine->dataToLoad = 1;
        engine->importFile(file);
        std::cout << "ENGINE => Loading complete" << std::endl;
    } else std::cerr << "ENGINE => ERROR: file not found" << std::endl;
}

void H2DE_RemoveAssets(H2DE_Engine* engine) {
    for (const auto& [name, texture] : engine->textures) H2DE_RemoveAsset(engine, name);
    for (const auto& [name, sound] : engine->sounds) H2DE_RemoveAsset(engine, name);
    for (const auto& [name, font] : engine->fonts) H2DE_RemoveAsset(engine, name);
}

void H2DE_RemoveAsset(H2DE_Engine* engine, const fs::path& name) {
    fs::path extension = name.extension();
    if (extension == ".png") {
        if (engine->textures.erase(name.string())) {
            std::cout << "ENGINE => Removed texture " << name << " from engine" << std::endl;
        }
    } else if (extension == ".mp3" || extension == ".ogg") {
        if (engine->sounds.erase(name.string())) {
            std::cout << "ENGINE => Removed sound " << name << " from engine" << std::endl;
        }
    } else if (extension == ".ttf") {
        if (engine->fonts.erase(name.string())) {
            std::cout << "ENGINE => Removed font " << name << " from engine" << std::endl;
        }
    }
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

void H2DE_Engine::importFiles(const fs::path& dir) {
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (fs::is_directory(entry.status())) importFiles(entry.path());
        else if (fs::is_regular_file(entry.status())) importFile(entry.path());
    }
}

void H2DE_Engine::importFile(const fs::path& file) {
    fs::path extension = file.extension();
    if (extension == ".png" || extension == ".jpg") importTexture(file);
    else if (extension == ".mp3" || extension == ".ogg") importSound(file);
    else if (extension == ".ttf") importFont(file);
}

void H2DE_Engine::importTexture(const fs::path& img) {
    size_t startNameIndex = img.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = img.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    SDL_Texture* texture = H2DE_Loader::loadTexture(renderer, img.string().c_str());
    if (texture != nullptr) {
        if (textures.find(file) != textures.end()) std::cerr << "ENGINE => WARNING: Asset " << '"' << file << '"' << " has been replaced" << std::endl;
        textures[file] = texture;
    } else std::cerr << "ENGINE => IMG_Load failed: " << SDL_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::importSound(const fs::path& song) {
    size_t startNameIndex = song.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = song.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    Mix_Chunk* music = H2DE_Loader::loadSound(song.string().c_str());
    if (music != nullptr) {
        if (sounds.find(file) != sounds.end()) std::cerr << "ENGINE => WARNING: Asset " << '"' << file << '"' << " has been replaced" << std::endl;
        sounds[file] = music;
    } else std::cerr << "ENGINE => Mix_LoadMUS failed: " << Mix_GetError() << std::endl;
    assetImported();
}

void H2DE_Engine::importFont(const fs::path& font) {
    size_t startNameIndex = font.string().rfind('\\');
    startNameIndex = (startNameIndex != std::string::npos) ? startNameIndex + 1 : 0;
    std::string file = font.string().substr(startNameIndex);
    std::replace(file.begin(), file.end(), '\\', '/');
    TTF_Font* ttf = H2DE_Loader::loadFont(font.string().c_str());
    if (ttf != nullptr) {
        if (fonts.find(file) != fonts.end()) std::cerr << "ENGINE => WARNING: Asset " << '"' << file << '"' << " has been replaced" << std::endl;
        fonts[file] = ttf;
    } else std::cerr << "ENGINE => TTF_OpenFont failed: " << TTF_GetError() << std::endl;
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
        H2DE_Pos pos = g->pos;
        H2DE_Size size = (g->type == CIRCLE) ? H2DE_Calculator::getCircleSize(g->radius) : (g->type == POLYGON) ? H2DE_Calculator::getPolygonSize(g->points) : g->size;

        int i = 0;
        while (pos.x + size.w * g->scale.x < size.w && i < 10) {

            // pos.x += 

            i++;
        }
        


    }

    return res;

    // std::vector<H2DE_GraphicObject*> res;
    // for (H2DE_GraphicObject* g : graphicObjects) if (g->repeatX) {
    //     int leftNb = ceil(static_cast<float>(g->pos.x) / (g->size.w * g->scale.x));
    //     for (int i = 0; i < leftNb; i++) {
    //         int posX = g->pos.x - g->size.w * g->scale.x * (i + 1);
    //         if (posX - g->size.w >= size.w) continue;
    //         H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
    //         copy->pos.x = posX;
    //         copy->repeatX = false;
    //         res.push_back(copy);
    //     }
    //     int rightNb = ceil(static_cast<float>(size.w - g->pos.x - g->size.w * g->scale.x) / (g->size.w * g->scale.x)) + 1;
    //     for (int i = 0; i < rightNb; i++) {
    //         int posX = g->pos.x + g->size.w * g->scale.x * (i + 1);
    //         if (posX + g->size.w < 0) continue;
    //         H2DE_GraphicObject* copy = new H2DE_GraphicObject(*g);
    //         copy->pos.x = posX;
    //         copy->repeatX = false;
    //         res.push_back(copy);
    //     }
    // }
    // return res;
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
    if (engine->debug->graphicObjects) std::cout << "ENGINE => rendering " << graphicObjects.size() << " object(s)" << std::endl; 

    H2DE_GraphicObject* clickedElement = nullptr;
    for (H2DE_GraphicObject* g : graphicObjects) {
        switch (g->type) {
            case IMAGE: engine->renderImage(g); break;
            case POLYGON: engine->renderPolygon(g); break;
            case CIRCLE: engine->renderCircle(g); break;
            case TEXT: engine->renderText(g); break;
            default: break;
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
    SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, 255);
    SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

void H2DE_Engine::renderImage(H2DE_GraphicObject* g) {
    std::unordered_map<std::string, SDL_Texture*>::iterator textureIterator = textures.find(g->texture);
    if (textureIterator == textures.end()) return;
    SDL_Texture* texture = textureIterator->second;
    H2DE_Size size = g->size;
    SDL_RendererFlip flip = g->flip;
    float rotation = g->rotation;
    H2DE_GraphicObject* parent = nullptr;

    // TRANSLATE
    H2DE_Pos translatedPos = H2DE_Calculator::getPosFromParents(g);

    // FLIP
    H2DE_Pos flipedPos = translatedPos;
    // parent = g->parent;
    // while (parent) {
    //     H2DE_Pos parentPos = H2DE_Calculator::getPosFromParents(parent);
    //     H2DE_Size parentSize = (parent->type == POLYGON) ? H2DE_Calculator::getPolygonSize(parent->points) : parent->size;
    //     H2DE_Pos parentCenter = H2DE_Calculator::getCenter(parentPos, parentSize);

    //     switch (parent->flip) {
    //         case SDL_FLIP_VERTICAL: flipedPos.y -= static_cast<int>((flipedPos.y - parentCenter.y) * 2 + size.h); break;
    //         case SDL_FLIP_HORIZONTAL: flipedPos.x -= static_cast<int>((flipedPos.x - parentCenter.x) * 2 + size.w); break;
    //         default: break;
    //     }

    //     if (flip != parent->flip && flip != SDL_FLIP_NONE && parent->flip != SDL_FLIP_NONE) rotation += 180;
    //     flip = H2DE_Calculator::addFlip(flip, parent->flip);

    //     parent = parent->parent;
    // }
    // parent = nullptr;

    // ROTATE
    H2DE_Pos rotatedPos = flipedPos;




    SDL_Rect destRect = { rotatedPos.x, rotatedPos.y, size.w, size.h };
    SDL_Point rotationOrigin = { g->rotationOrigin.x, g->rotationOrigin.y };

    SDL_Point pivot = static_cast<SDL_Point>(rotationOrigin);

    SDL_SetTextureColorMod(texture, g->rgb.r, g->rgb.g, g->rgb.b);
    SDL_SetTextureAlphaMod(texture, g->rgb.a);
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);

    if (g->srcRect.has_value()) {
        SDL_Rect srcRect = (SDL_Rect)g->srcRect.value();
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, rotation, &pivot, flip);
    } else SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &pivot, flip);
}

void H2DE_Engine::renderPolygon(H2DE_GraphicObject* g) {
    size_t nbPoints = g->points.size();
    std::vector<Sint16> vx(nbPoints);
    std::vector<Sint16> vy(nbPoints);

    H2DE_GraphicObject* parent = nullptr;
    H2DE_Size size = H2DE_Calculator::getPolygonSize(g->points);
    for (int i = 0; i < nbPoints; i++) {
        vx[i] = g->points[i].x;
        vy[i] = g->points[i].y;
    }

    // TRANSLATE
    H2DE_Pos translatedPos = H2DE_Calculator::getPosFromParents(g);
    for (int i = 0; i < nbPoints; i++) {
        vx[i] += translatedPos.x;
        vy[i] += translatedPos.y;
    }

    // FLIP
    H2DE_Pos flipedPos = translatedPos;
    parent = g->parent;
    while (parent) {
        H2DE_Pos parentPos = H2DE_Calculator::getPosFromParents(parent);
        H2DE_Size parentSize = (parent->type == POLYGON) ? H2DE_Calculator::getPolygonSize(parent->points) : parent->size;
        H2DE_Pos parentCenter = H2DE_Calculator::getCenter(parentPos, parentSize);

        switch (parent->flip) {
            case SDL_FLIP_VERTICAL: flipedPos.y -= static_cast<int>((flipedPos.y - parentCenter.y) * 2 + size.h); break;
            case SDL_FLIP_HORIZONTAL: flipedPos.x -= static_cast<int>((flipedPos.x - parentCenter.x) * 2 + size.w); break;
            default: break;
        }

        for (int i = 0; i < nbPoints; i++) {
            H2DE_Pos point = { vx[i], vy[i] };
            H2DE_Pos flipedPoint = H2DE_Calculator::getFlipedPos(point, parentCenter, parent->flip);
            vx[i] = flipedPoint.x;
            vy[i] = flipedPoint.y;
        }

        parent = parent->parent;
    }
    parent = nullptr;

    H2DE_Pos center = H2DE_Calculator::getCenter(flipedPos, size);
    if (g->flip != SDL_FLIP_NONE) for (int i = 0; i < nbPoints; i++) {
        H2DE_Pos point = { vx[i], vy[i] };
        H2DE_Pos flipedPoint = H2DE_Calculator::getFlipedPos(point, center, g->flip);
        vx[i] = flipedPoint.x;
        vy[i] = flipedPoint.y;
    }

    // ROTATE
    // parent = g->parent;
    // while (parent) {
    //     H2DE_Pos parentPos = H2DE_Calculator::getPosFromParents(parent);
    //     H2DE_Pos parentRotationOrigin = H2DE_Calculator::getRotationOrigin(parentPos, parent->rotationOrigin);

    //     for (int i = 0; i < nbPoints; i++) {
    //         H2DE_Pos point = { vx[i], vy[i] };
    //         H2DE_Pos rotatedPoint = H2DE_Calculator::getRotatedPos(point, parentRotationOrigin, parent->rotation);
    //         vx[i] = rotatedPoint.x;
    //         vy[i] = rotatedPoint.y;
    //     }

    //     parent = parent->parent;
    // }
    // parent = nullptr;

    if (g->rotation != 0.0f) {
        H2DE_Pos rotationOrigin = H2DE_Calculator::getRotationOrigin(flipedPos, g->rotationOrigin);
        if (debug->rotationOrigins) renderPixel(rotationOrigin, { 255, 0, 0, 255 });

        for (int i = 0; i < nbPoints; i++) {
            H2DE_Pos point = { vx[i], vy[i] };
            H2DE_Pos rotatedPoint = H2DE_Calculator::getRotatedPos(point, rotationOrigin, g->rotation);
            vx[i] = rotatedPoint.x;
            vy[i] = rotatedPoint.y;
        }
    }

    // SCALE
    H2DE_Pos scaledPos = flipedPos;
    H2DE_Scale scale = { 1.0f, 1.0f };
    parent = g->parent;
    while (parent) {
        if (parent->scale.x != 1.0f || parent->scale.y != 1.0f) {
            scale.x *= parent->scale.x;
            scale.y *= parent->scale.y;

            H2DE_Pos parentPos = H2DE_Calculator::getPosFromParents(parent);
            H2DE_Pos parentScaleOrigin = H2DE_Calculator::getScaleOrigin(parentPos, parent->scaleOrigin);
            H2DE_Pos futurePos = H2DE_Calculator::getRescaledPos(scaledPos, size, parentScaleOrigin, parent->scale);
            H2DE_Pos offset = { scaledPos.x - futurePos.x, scaledPos.y - futurePos.y };

            for (int i = 0; i < nbPoints; i++) {
                H2DE_Pos point = { vx[i], vy[i] };
                H2DE_Pos t = { static_cast<int>((point.x - scaledPos.x) * parent->scale.x), static_cast<int>((point.y - scaledPos.y) * parent->scale.y) };
                vx[i] = scaledPos.x + t.x - offset.x;
                vy[i] = scaledPos.y + t.y - offset.y;
            }

            scaledPos = futurePos;
        }

        parent = parent->parent;
    }
    parent = nullptr;

    if (g->scale.x != 1.0f || g->scale.y != 1.0f) {

        H2DE_Pos scaleOrigin = H2DE_Calculator::getScaleOrigin(scaledPos, { static_cast<int>(g->scaleOrigin.x * scale.x), static_cast<int>(g->scaleOrigin.y * scale.y) });
        if (debug->scaleOrigins) renderPixel(scaleOrigin, { 0, 255, 0, 255 });
        H2DE_Pos futurePos = H2DE_Calculator::getRescaledPos(scaledPos, size, scaleOrigin, g->scale);
        H2DE_Pos offset = { scaledPos.x - futurePos.x, scaledPos.y - futurePos.y };

        for (int i = 0; i < nbPoints; i++) {
            H2DE_Pos point = { vx[i], vy[i] };
            H2DE_Pos t = { static_cast<int>((point.x - scaledPos.x) * g->scale.x), static_cast<int>((point.y - scaledPos.y) * g->scale.y) };
            vx[i] = scaledPos.x + t.x - offset.x;
            vy[i] = scaledPos.y + t.y - offset.y;
        }

        scaledPos = futurePos;
    }

    // RENDER
    if (g->filled) filledPolygonColor(renderer, vx.data(), vy.data(), nbPoints, static_cast<Uint32>(g->rgb));
    else polygonColor(renderer, vx.data(), vy.data(), nbPoints, static_cast<Uint32>(g->rgb));
}

void H2DE_Engine::renderCircle(H2DE_GraphicObject* g) {
    H2DE_GraphicObject* parent = nullptr;
    H2DE_Size size = H2DE_Calculator::getCircleSize(g->radius);

    // TRANSLATE
    H2DE_Pos translatedPos = H2DE_Calculator::getPosFromParents(g);

    // FLIP => ellipse
    H2DE_Pos flipedPos = translatedPos;

    // ROTATE
    H2DE_Pos rotatedPos = flipedPos;
    // parent = g->parent;
    // while (parent) {
    //     H2DE_Pos parentPos = H2DE_Calculator::getPosFromParents(parent);
    //     H2DE_Pos parentRotationOrigin = H2DE_Calculator::getRotationOrigin(parentPos, parent->rotationOrigin);

    //     rotatedPos = H2DE_Calculator::getRotatedPos(rotatedPos, parentRotationOrigin, parent->rotation);

    //     parent = parent->parent;
    // }
    // parent = nullptr;

    if (g->rotation != 0.0f) {
        H2DE_Pos rotationOrigin = H2DE_Calculator::getRotationOrigin(rotatedPos, g->rotationOrigin);
        if (debug->rotationOrigins) renderPixel(rotationOrigin, { 255, 0, 0, 255 });

        rotatedPos = H2DE_Calculator::getRotatedPos(rotatedPos, rotationOrigin, g->rotation);
    }
    


    // H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
    // pos = H2DE_Calculator::getRescaledPos(pos, { static_cast<int>(g->radius) * 2, static_cast<int>(g->radius) * 2 }, g->scaleOrigin, g->scale);
    
    // H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
    // rotationOrigin = { rotationOrigin.x + pos.x, rotationOrigin.y + pos.y };

    // pos = H2DE_Calculator::getRotatedPos(pos, rotationOrigin, g->rotation);

    if (g->filled) filledCircleColor(renderer, rotatedPos.x, rotatedPos.y, g->radius, static_cast<Uint32>(g->rgb));
    else circleColor(renderer, rotatedPos.x, rotatedPos.y, g->radius, static_cast<Uint32>(g->rgb));
}

void H2DE_Engine::renderText(H2DE_GraphicObject* g) {
    // std::unordered_map<std::string, TTF_Font*>::iterator fontIterator = fonts.find(g->font);
    // if (fontIterator != fonts.end()) {

    //     H2DE_Pos pos = H2DE_Calculator::getPosFromParents(g);
    //     pos = H2DE_Calculator::getRescaledPos(pos, g->size, g->scaleOrigin, g->scale);
    //     H2DE_Size size = H2DE_Calculator::getRescaledSize(g->size, g->scale);
    //     SDL_Rect destRect = { pos.x, pos.y, size.w, size.h };

    //     H2DE_Pos rotationOrigin = H2DE_Calculator::getRescaledRotationOrigin(g->rotationOrigin, g->scale);
    //     SDL_Point convertedRotationOrigin = static_cast<SDL_Point>(rotationOrigin);

    //     TTF_Font* font = fontIterator->second;
    //     SDL_Surface* surface = TTF_RenderText_Solid(font, g->text, g->rgb);
    //     SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    //     SDL_FreeSurface(surface);
    //     SDL_SetTextureAlphaMod(texture, g->rgb.a);
    //     SDL_SetTextureScaleMode(texture, SDL_ScaleModeLinear);
    //     SDL_RenderCopyEx(renderer, texture, NULL, &destRect, g->rotation, &convertedRotationOrigin, g->flip);
    // }
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

H2DE_Size H2DE_GetEngineMinimumSize(H2DE_Engine* engine) {
    return engine->minSize;
}

int H2DE_GetEngineFPS(H2DE_Engine* engine) {
    return engine->fps;
}

// SETTER 
void H2DE_SetEngineSize(H2DE_Engine* engine, int w, int h) {
    engine->size.w = std::clamp(w, (engine->minSize.w == -1) ? 0 : engine->minSize.w, (engine->maxSize.w == -1) ? INT_MAX : engine->maxSize.w);
    engine->size.h = std::clamp(h, (engine->minSize.h == -1) ? 0 : engine->minSize.h, (engine->maxSize.h == -1) ? INT_MAX : engine->maxSize.h);
}

void H2DE_SetEngineMaximumSize(H2DE_Engine* engine, int w, int h) {
    engine->maxSize.w = std::clamp(w, 0, INT_MAX);
    engine->maxSize.h = std::clamp(h, 0, INT_MAX);
    H2DE_Size currentSize = H2DE_GetEngineSize(engine);
    H2DE_SetEngineSize(engine, currentSize.w, currentSize.h);
}

void H2DE_SetEngineMinimumSize(H2DE_Engine* engine, int w, int h) {
    engine->minSize.w = std::clamp(w, 0, INT_MAX);
    engine->minSize.h = std::clamp(h, 0, INT_MAX);
    H2DE_Size currentSize = H2DE_GetEngineSize(engine);
    H2DE_SetEngineSize(engine, currentSize.w, currentSize.h);
}

H2DE_Timeline* H2DE_CreateTimeline(H2DE_Engine* engine, unsigned int duration, H2DE_TimelineEffect effect, std::function<void(float)> update, std::function<void()> completed, int loop) {
    H2DE_Timeline* timeline = new H2DE_Timeline(engine, duration, effect, update, completed, loop);
    return timeline;
}
