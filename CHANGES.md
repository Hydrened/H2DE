## H2DE-3.0.0
#### ADDED
- delays
- bar object rendering
- basic object rendering
- button object rendering
- textures
- sprites

## H2DE-b-3.0.4
#### ADDED
- camera contains object (auto absolute detection)
- camera contains hitbox (absolute or not)
- objects
- render object hitboxes

## H2DE-b-3.0.3
#### ADDED
- rect collision detection
- rect face collision detection
- camera
- settings get key string 
- settings get key bool 
- get fps
#### FIXED
- set fps

## H2DE-b-3.0.2
#### ADDED
- vector 2d +, +=, -, -=, *, *=, /, /= operators
- vector 2d ==, !=, >, >=, <, <= operators
- vector 2d << operator
- rect +, +=, -, -=, *, *=, /, /= operators
- rect ==, !=, >, >=, <, <= operators
- rect << operator
- rgb color
- rgb color ==, != operators
- rgb color << operator
- hsv color
- hsv color ==, != operators
- hsv color << operator
- random int in range
- random float in range
- random bool
- lerp
#### FIXED
- settings key finding
- window pos y and size saving

## H2DE-b-3.0.1
#### ADDED
- window resize
- window fullscreen
- settings
- get window pos
- get window size
- get window fullscreen
- get window resizable
- save window state

## H2DE-b-3.0.0
#### REMAKE FROM SCRATCH
#### ADDED
- window
- game loop
- asset loader





## H2DE-2.1.7
- added show and hode methods on level objects
#### REPLACED
- `H2DE_GetObjectData` per `H2DE_GetLevelObjectData`

## H2DE-2.1.6
#### FIXED
- timeline not resetting correctly

## H2DE-2.1.5
#### ADDED
- show cursor 
- hide cursor 

## H2DE-2.1.4
#### FIXED
- text size updating when changing text

## H2DE-2.1.3
#### ADDED
- special characters
- uppercase characters
- lowercase characters
#### MODIFIED
- font system

## H2DE-2.1.2
#### FIXED
- `H2DE_GetSpriteData` returns a pointer to the data instead of a copy
- `H2DE_GetTextData` returns a pointer to the data instead of a copy
- absolute level objects not rendering
#### MODIFIED
- `H2DE_GetSurfaceData` instead of surface->getData()

## H2DE-2.1.1
#### FIXED
- timeline with loop = 0

## H2DE-2.1.0
#### ADDED
- text system
- text align
- letter spacing

## H2DE-2.0.23
#### ADDED
- name for hitboxes

## H2DE-2.0.22
#### ADDED
- += operators for `H2DE_LevelPos`, `H2DE_LevelSize`, `H2DE_LevelVelocity` and `H2DE_LevelRect`
- -= operators for `H2DE_LevelPos`, `H2DE_LevelSize`, `H2DE_LevelVelocity` and `H2DE_LevelRect`
- *= operators for `H2DE_LevelPos`, `H2DE_LevelSize` and `H2DE_LevelVelocity`
- /= operators for `H2DE_LevelPos`, `H2DE_LevelSize` and `H2DE_LevelVelocity`

## H2DE-2.0.21
#### ADDED
- sprite can update or not when game is paused

## H2DE-2.0.20
#### ADDED
- `H2DE_StopEngine` method
#### FIXED
- set fps

## H2DE-2.0.19
#### FIXED
- get surface data give a pointer to the data instead

## H2DE-2.0.18
#### ADDED
- on collides now gives the level object that collided
#### FIXED
- object not updating if no gravity

## H2DE-2.0.17
#### ADDED
- `H2DE_RandomFloatInRange` method
- `H2DE_RandomIntInRange` method
- `H2DE_RandomBool` method

## H2DE-2.0.16
#### ADDED
- `H2DE_ConvertToLevelPos` method

## H2DE-2.0.15
#### FIXED
- game events not working

## H2DE-2.0.14
#### ADDED
- hitboxes debug
#### MODIFIED
- object rendered number debug
#### FIXED
- level objects and hitboxes rendering outside of screen
#### REMOVED
- level object data update parameter

## H2DE-2.0.13
#### ADDED
- sprites
#### FIXED
- camera smoothing

## H2DE-2.0.12
#### FIXED
- compiled with test

## H2DE-2.0.11
#### ADDED
- pause
- resume

## H2DE-2.0.10
#### ADDED
- buttons
#### REMOVED
- setters for level object (get data and modify it intead)

## H2DE-2.0.9
#### ADDED
- timelines
- engine debug

## H2DE-2.0.8
#### ADDED
- settings initializer
- settings reader
- modify settings
- get settings

## H2DE-2.0.7
#### ADDED
- parent position offset
- rendering objects in index order

## H2DE-2.0.6
#### ADDED
- methods comments
- camera lock to level object
- camera padding from a level object
- camera smooth movements
#### FIXED
- get camera size
#### REMOVED
- set engine's render function

## H2DE-2.0.5
#### ADDED
- level object default gravity
- level object default max gravity
- level object collision
- level object snap
- level object multiple hitboxes
- auto save / load window data
- game data
#### MODIFIED
- level object functioning

## H2DE-2.0.4
#### ADDED
- level object texture rendering
- level object hitbox rendering 
#### MODIFIED
- graphic object now becomes level objects
#### REMOVED
- camera zoom

## H2DE-2.0.3
#### ADDED
- camera contains
- types cout
- image rendering
#### FIXED
- camera's default position

## H2DE-2.0.2
#### ADDED
- set window grab
- set window fullscreen
- set window resizable
- camera
- set camera pos
- set camera zoom
- get camera pos
- get camera size
- get camera zoom
- level position system
- level size system
- level hitbox system
- collision detection
- rgb color
- hsv color
- add hue
- add saturation
- add value

## H2DE-2.0.1
#### ADDED
- load assets
- load specific asset
- remove assets
- remove specific asset
- delay
- get window size
- get window minimum size
- get window maximum size
- get fps
- get curernt fps
- set sound volume
- play sound
- pause sound
- resume sound
#### FIXED
- init errors
- clearing memory

## H2DE-2.0.0
#### REMAKE FROM SCRATCH
#### ADDED
- auto window / renderer creation
- game loop
- set window title
- set window size
- set window min size
- set window max size
- set window fps





## H2DE-1.3.16
#### ADDED
- auto encode / decode json files

## H2DE-1.3.15
#### FIXED
- graphic object's index sorting
#### REMOVED
- text rendering

## H2DE-1.3.14
#### MODIFIED
- indexes are now signed

## H2DE-1.3.13
#### MODIFIED
- color name in `H2DE_GraphicObject` structure

## H2DE-1.3.12
#### ADDED
- jpg loading

## H2DE-1.3.11
#### ADDED
- remove assets from engine
- remove specific asset from engine

## H2DE-1.3.10
#### ADDED
- `H2DE_LoadAsset` method
- warnings when loading assets with same name
#### FIXED
- loading percentage not reseting

## H2DE-1.3.9
#### ADDED
- `H2DE_SetEngineMinimumSize` method
- `H2DE_GetEngineMinimumSize` method

## H2DE-1.3.8
#### REMOVED
- rotate from parent for polygons
- flip from parent for images

## H2DE-1.3.7
#### ADDED
- translate, flip for images
- translate, flip for circles
#### FIXED
- swaped horizontal and vertical flips
- polygons scales if x or y scale == 1.0f

## H2DE-1.3.6
#### ADDED
- translate, flip, rotation, and scale (polygons only)

## H2DE-1.3.5
#### FIXED
- flips (polygons only)

## H2DE-1.3.4
#### ADDED
- debug for number of graphic objects
- debug for scale origins
- debug for rotation origins
#### FIXED
- child rotation when parent is rotated

## H2DE-1.3.3
#### ADDED
- parent rotations (polygons only)
#### FIXED
- rotation (polygons only)

## H2DE-1.3.2
#### FIXED
- scaled parent in scaled parent (polygons only)

## H2DE-1.3.1
#### ADDED
- scale for parents and child (polygons only) (but not scaled parent in scaled parent)
#### REMOVED
- polygon rotations (temporary)

## H2DE-1.3.0
#### ADDED
- parent system (translates only)

## H2DE-1.2.4
#### ADDED
- src rect for images

## H2DE-1.2.3
#### ADDED
- debug: number of graphic object rendered

## H2DE-1.2.2
#### REPLACED
- songs and sfxs per sound

## H2DE-1.2.1
#### ADDED
- timeline reset
- timeline manager reset

## H2DE-1.2.0
#### ADDED
- rgb and hsv colors
- added add, substract, multiply and divide functions for rgb colors

## H2DE-1.1.5
#### ADDED
- dump parameter on write json method
- debug for engine rendering (only rotation origins)

## H2DE-1.1.4
#### ADDED
- `H2DE_CreateTimelineManager` function
- `H2DE_DestroyTimelineManager` function
- `H2DE_CreateGraphicObject` function

## H2DE-1.1.3
#### FIXED
- click only call the onclick function on the element with the highest index

## H2DE-1.1.2
#### FIXED
- sfx are now ogg files
- timeline with null duration call update fonction with blend at 1.0f

## H2DE-1.1.1
#### FIXED
- timeline with null duration instantly call completed function parameter

## H2DE-1.1.0
#### ADDED
- onclick function on graphic objects

## H2DE-1.0.19
#### ADDED
- timeline loops
#### FIXED
- durations are now unsigned ints

## H2DE-1.0.18
#### FIXED
- timeline memory usage

## H2DE-1.0.17
#### ADDED
- remove timeline from timeline manager

## H2DE-1.0.16
#### FIXED
- volume does not work if diffrent from 100

## H2DE-1.0.15
#### ADDED
- clear timeline manager method

## H2DE-1.0.14
#### ADDED
- timeline manager

## H2DE-1.0.13
#### ADDED
- `H2DE_Timeline::tick()` must now be called manually

## H2DE-1.0.12
#### FIXED
- removed start & end from timeline parameters
- timelines calling completed paramter many times

## H2DE-1.0.11
#### FIXED
- double deleting pointer
- `H2DE_Calculator` class is now friend of `H2DE_Engine`

## H2DE-1.0.10 (BUGGED)
#### FIXED
- updated public engine methods to friend

## H2DE-1.0.9
#### ADDED
- timelines
- getter for engine's fps
- added ease in and ease out timeline effects
#### FIXED
- effects are now included in timelines

## H2DE-1.0.8
#### FIXED
- json read method now returns a json pointer
- json write method now needs a json pointer

## H2DE-1.0.7
#### FIXED
- repeat x and y images disappearing

## H2DE-1.0.6
#### FIXED
- errors for reading and writing jsons

## H2DE-1.0.5
#### ADDED
- engine max size
- getter and setter for engine's max size
#### FIXED
- getter and setter functions names for engine's size

## H2DE-1.0.4
#### ADDED
- getter and setter for engine's size

## H2DE-1.0.3
#### REMOVED
- linear color effect

## H2DE-1.0.2
#### FIXED
- engine creation errors

## H2DE-1.0.1
#### FIXED
- engine loading assets errors

## H2DE-1.0.0
#### ADDED
- load png, mp3, wav and ttf files
- render images
- render polygons
- render circles
- render texts
- play / pause / resume songs
- play / pause / resume sfxs
- read / write json 
- linear effect
- ease in out effect
- graphic object rotation
- graphic object scale
- image / text flip
- repeat x / y graphic 
