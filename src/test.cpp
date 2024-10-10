#include <H2DE/H2DE.h>

void test(float a) {
    std::cout << a << std::endl;
}

class Game {
private:
    H2DE_Engine* engine;
    int fps;
    bool isRunning = true;
    H2DE_TimelineManager* tm = H2DE_CreateTimelineManager();
    H2DE_RGB rgb = { 255, 0, 0, 255 };
    float m = 0.0f;

public:
    Game(H2DE_Engine* e, int f) : engine(e), fps(f) {
        H2DE_Timeline* t = H2DE_CreateTimeline(engine, 2500, LINEAR, [this](float blend) {
            m = blend * 2;
        }, NULL, 1);

        H2DE_AddTimelineToManager(tm, t);
        H2DE_SetSFXVolume(engine, 10, -1);
        H2DE_SetSongVolume(engine, 10);
        H2DE_DebugEngineRendering(engine, true);
    }

    ~Game() {
        H2DE_DestroyTimelineManager(tm);
    }

    void run() {
        const int timePerFrame = 1000 / fps;
        Uint32 now, fpsTimer = SDL_GetTicks();
        int frameTime, frameCounter = 0, currentFPS = 0;
        SDL_Event event;
        H2DE_Pos pos = { 0, 0 };


        while (isRunning) {
            now = SDL_GetTicks();
            handleEvents(event);

            H2DE_TickTimelineManager(tm);

            int s = m * 200;

            H2DE_GraphicObject* derriere = H2DE_CreateGraphicObject();
            derriere->type = POLYGON;
            derriere->pos = { 0, 0 };
            derriere->points = {
                { 0, 0 },
                { s, 0 },
                { s, s },
                { 0, s },
            };
            derriere->rgb = rgb;
            derriere->rotation = 20.0f;
            derriere->rotationOrigin = { 100, 100 };
            derriere->filled = true;
            derriere->index = 1;
            derriere->onclick = [this]() {
                H2DE_ResetTimelineManager(tm);
            };
            H2DE_AddGraphicObject(engine, derriere);



            H2DE_RenderEngine(engine);

            frameCounter++;
            frameTime = SDL_GetTicks() - now;
            if (timePerFrame >= frameTime) SDL_Delay(timePerFrame - frameTime);
            if (SDL_GetTicks() - fpsTimer >= 1000) {
                currentFPS = frameCounter / ((SDL_GetTicks() - fpsTimer) / 1000.0f);
                frameCounter = 0;
                fpsTimer = SDL_GetTicks();
            }
        }
    }

    void close() {
        isRunning = false;
    }

    void handleEvents(SDL_Event event) {
        while (SDL_PollEvent(&event)) switch (event.type) {
            case SDL_QUIT: close(); break;
            case SDL_MOUSEBUTTONDOWN: if (event.button.button == SDL_BUTTON_LEFT) H2DE_Click(engine, event.button.x, event.button.y); break;
            case SDL_KEYDOWN: switch (event.key.keysym.sym) {
                case SDLK_k: H2DE_PlaySong(engine, "song.mp3", 0); break;
                case SDLK_l: H2DE_PlaySFX(engine, "test.ogg", 0); break;
            }
        }
    }
};

int main(int argc, char** argv) {
    const int FPS = 60;
    const int winWidth = 1280;
    const int winHeight = 720;

    SDL_Window* window = SDL_CreateWindow("H2DE-1.0.0", 100, 100, winWidth, winHeight, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    
    H2DE_Engine* engine = H2DE_CreateEngine(renderer, winWidth, winHeight, FPS);
    if (!engine) return 1;
    H2DE_LoadAssets(engine, "assets");

    H2DE_SetEngineSize(engine, 1200, 500);

    
    Game game = Game(engine, FPS);
    game.run();

    H2DE_DestroyEngine(engine);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
