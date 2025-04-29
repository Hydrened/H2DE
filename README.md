```cpp
 __ __  _____  ____    _____
|  |  ||___  ||  _  \ |  ___|
|     ||  ___|| |_|  ||  ___|
|__|__||_____||_____/ |_____|

Hydren 2D Engine
```

# H2DE
H2DE is a simple and open-source 2D game engine built with C++. It uses SDL2 for handling graphics and inputs. It is designed to help you create 2D games easily while maintaining efficiency and flexibility.


## Getting started

### Prerequisites
- g++: Version 13.2.0 or higher
- SDL2: Version 2.30.1 or higher
- SDL2_image: Version 2.8.2 or higher
- SDL_mixer: Version 2.8.0 or higher
- SDL2_gfx: Version 1.0.1
- Base64: Version 2.rc.09 or higher
- Inih





### Usage
```cpp
#include <H2DE/H2DE.h>
```





### Create an engine

1. Set up the engine data
```cpp
H2DE_EngineData data = H2DE_EngineData();

data.window.title = "Title";                                        // title of the window
data.window.fps = 60;                                               // fps of the window
data.window.pos = { 100, 100 };                                     // position of the window
data.window.size = { 1280, 720 };                                   // size of the window
data.window.fullscreen = false;                                     // whether the window should be fullscreen
data.window.resizable = true;                                       // for the window to be resizable
data.window.saveState = true;                                       // if you wanna save the position and size of the window when closing the app
data.window.ratio = H2DE_WINDOW_RATIO_NO_RATIO;                     // ratio of the window (16/9, 3/4, custom, and much more)

data.camera.width = 20.0f;                                          // width of the camera
data.camera.defaultPos = { 0.0f, 0.0f };                            // default position of the camera
data.camera.padding = { 1.0f, 1.0f, 1.0f, 1.0f };                   // padding from the reference (will automatically move the camera away from the reference)
data.camera.smoothing = 0.5f;                                       // smoothing of the camera: 0 = no smoothing, 1 = max smoothing
```

2. Create the engine
```cpp
H2DE_Engine* engine = H2DE_CreateEngine(data);
```

3. Load assets
```cpp
H2DE_LoadAssets(engine, "assets");
```

4. Set up the functions that are called every frame to handle events and update the game
```cpp
H2DE_SetGameHandleEventCall(engine, [](SDL_Event event) {
    switch (event.type) {
        // Handling events code
    }
});

H2DE_SetGameUpdateCall(engine, []() {
    // Updating code
});
```

5. Run the engine
```cpp
H2DE_RunEngine(engine);
```





### Create an object

1. Set up the object's data
```cpp
H2DE_ObjectData object_data = H2DE_ObjectData();
object_data.pos = { 1.0f, 1.0f };                                   // position of the object
object_data.size = { 5.0f, 5.0f };                                  // size of the object
object_data.pivot = object_data.size.getCenter()                    // pivot for rotation
object_data.rotation = 45.0f;                                       // rotation of the object
object_data.flip = H2DE_FLIP_XY;                                    // flip of the object
object_data.absolute = false;                                       // whether the object is absolute (ex. for hud)
object_data.index = 1;                                              // rendering index (higher = on top)

// You can create as many hitboxes as you want
H2DE_Hitbox obj1_hitbox = H2DE_Hitbox();
obj1_hitbox.color = { 255, 127, 127, 255 };                         // color of the hitbox (if rendered)
obj1_hitbox.rect = { 1.0f, 1.0f, 3.0f, 3.0f };                      // rectangle of the hitbox
obj1_hitbox.collisionIndex = 1;                                     // collision index (hitboxes can only collide with same index)
obj1_hitbox.onCollide = [](H2DE_Object* object, H2DE_Face face) {   // function that is called when a collision is detected

};
object_data.hitboxes["collision"] = obj1_hitbox;                    // set the name 'collision' for this hitbox
```

2. Set up basic object-specific data

```cpp
H2DE_BasicObjectData basic_object_data = H2DE_BasicObjectData();

// Set up surface data
H2DE_SurfaceData surface_data = H2DE_SurfaceData();
surface_data.textureName = "image.png";                             // name of the texture
surface_data.scaleMode = H2DE_SCALE_MODE_LINEAR;                    // rendering mode
surface_data.color = { 255, 255, 255, 255 };                        // color of the texture

// Create texture data for the surface
H2DE_TextureData obj1_texture_data = H2DE_TextureData();
obj1_texture_data.srcRect = { 0, 0, 10, 10 };                       // src rect if needed

// Create texture
basic_object_data.surface = H2DE_CreateTexture(engine, surface_data, obj1_texture_data);
```

Note: You can also create a sprite instead of a texture

3. Create object
```cpp
H2DE_BasicObject* object = H2DE_CreateBasicObject(engine, object_data, basic_object_data);
```

Note: You can also create other types of objects such as:
- H2DE_BasicObject: basically just a texture and hitboxes
- H2DE_BarObject: background texture, front texture and a function for setting the bar value
- H2DE_ButtonObject: a texture and hover, out, click events
- H2DE_TextObject: for text rendering
