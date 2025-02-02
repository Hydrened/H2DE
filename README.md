# H2DE
H2DE is a simple and open-source 2D game engine built with C++. It uses SDL2 for handling graphics and inputs. It's designed to help you create 2D games with ease while keeping things efficient and flexible.


## Getting started

### Prerequisites
- g++: Version 13.2.0 or higher
- SDL2: Version 2.30.1 or higher
- SDL_mixer: Version 1.2.7 or higher
- SDL2_gfx: Version 1.0.1
- SDL2_image: Version 2.8.2 or higher
- Base64: Version 2.rc.09 or higher
- Inih


### Include
```cpp
#include <H2DE/H2DE.h>
```


### Create an engine

1. Create the engine's data 
```cpp
H2DE_EngineData data = H2DE_EngineData();
data.fps = 60;
data.camera.width = 20.0f;
data.camera.smoothing = 0.1f;
data.window.title = "Window title";
data.window.pos = { 100, 100 };
data.window.size = { 1280, 720 };
data.window.fullscreen = false;
data.window.resizable = false;
data.window.saveState = true;
```
2. Create the engine
```cpp
H2DE_Engine* engine = H2DE_CreateEngine(data);
```

3. Load assets
```cpp
H2DE_LoadAssets(engine, "assets");
```

4. Set the functions called each frame for handling events and updating
```cpp
H2DE_SetGameHandleEventCall(engine, [](SDL_Event event) {
    // Handling events code
});
H2DE_SetGameUpdateCall(engine, []() {
    // Updating code
});
```

5. Run the engine
```cpp
H2DE_RunEngine(engine);
```
