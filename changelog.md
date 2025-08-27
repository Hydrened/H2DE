## H2DE-4.3.2
#### ADDED
- frame-synced events

## H2DE-4.3.1
#### ADDED
- up / down arrow controls for input object
#### FIXED
- input blur with any mouse button
- possible crash when updating cursor on input object

## H2DE-4.3.0
#### ADDED
- input object
- button text getter and setter
- timer text getter
- isVisible getters for objects and surfaces
- color macros
- null object and surface macros
- padding == operator
- time == operator
#### MODIFIED
- get game / interface mouse pos to get mouse game / interface translate
#### REMOVED
- button pause sensitive (always true now)
- button event timeline

## H2DE-4.2.8
#### ADDED
- debug hitboxes with collision indexes as parameter

## H2DE-4.2.7
#### ADDED
- version macros
- camera game interface world rect
- button hover cursor
#### MODIFIED
- camera get world rect is now get game world rect
- renomed private members with "_" prefix

## H2DE-4.2.6
#### ADDED
- printl
- font's characters can have a unique size
- font ascent
- font descent
#### MODIFIED
- renomed text container to bounds
- font size is now a float representing the characters' height

## H2DE-4.2.5
#### ADDED
- line break for text object
- set cursor
#### FIXED
- json read encoded files crash
#### MODIFIED
- Json namespace is now in H2DE namespace

## H2DE-4.2.4
#### FIXED
- setting higher value than maximum for bar object
- bar background surfaces not working
#### MODIFIED
- bar front surfaces are now fill surfaces

## H2DE-4.2.3
#### ADDED
- loadings on silent mode
#### FIXED
- async load double warning when overriding an asset

## H2DE-4.2.2
#### FIXED
- createObject template not always working

## H2DE-4.2.1
#### FIXED
- bar object front surface not working

## H2DE-4.2.0
#### ADDED
- load assets async
#### FIXED
- divide functions not noexcept
#### MODIFIED
- create and destroy engine are now is H2DE namespace

## H2DE-4.1.14
#### ADDED
- min and max with more than 2 parameters
#### MODIFIED
- lerp functions easing is now H2DE_EASING_LINEAR by default
- reorganized utils header

## H2DE-4.1.13
#### ADDED
- encode / decode with custom key for json encoding / decoding

## H2DE-4.1.12
#### ADDED
- json file exists
- convert value to json
#### MODIFIED
- lerp functions completed call is now nullptr by default

## H2DE-4.1.11
#### ADDED
- mouse button specification
#### FIXED
- destroying object when ending a object lerp effect crash 

## H2DE-4.1.10
#### ADDED
- padding cout operator
- transform cout operator
- print

## H2DE-4.1.9
#### ADDED
- border placement

## H2DE-4.1.8
#### FIXED
- objects hitboxes default scale 

## H2DE-4.1.7
#### FIXED
- timeline memory leak
- timeline easing not working
- buttons not working

## H2DE-4.1.6
#### ADDED
- border surface
- border thickness
- chronos
- constexpr noexcept min, max and clamp
- cout operator for H2DE_Time
#### MODIFIED
- button events parameters
- timelines now gives a pointer to a timeline instead of a timeline ID (same for delays)
- timer object based on chronos

## H2DE-4.1.5
#### FIXED
- text fliped letters when changing camera origins
- mouse pos not accurate when changing camera origins

## H2DE-4.1.4
#### ADDED
- random float function [0.0f, 1.0f]
- random value from vector function
- random index from vector function
#### FIXED
- subpixel precision

## H2DE-4.1.3
#### ADDED
- warning if audio played was not found
- auto volume saving
#### FIXED
- sfx using channel 0 (reserved for song only)
- volume lerping not working
- audio not stopping in debug mode
#### MODIFIED
- renomed volume to audio

## H2DE-4.1.2
#### ADDED
- get fixed delta time engine function
#### FIXED
- volume pause sensitive not working

## H2DE-4.1.1
#### ADDED
- getter and setter for text container
- getter and setter for timer pause sensitive
- increase / decrease for timer
- onReach function for timer
- missing dll files: "libgcc_s_seh-1.dll", "libstdc++-6.dll" and "libwinpthread-1.dll"
#### FIXED
- H2DE_Time constructor ms type
- timer member overflow

## H2DE-4.1.0
#### ADDED
- scale object resize surfaces and hitboxes
- specific member operation for H2DE_Vector2D and H2DE_Rect
- "has hitbox" and "has surface" object methods

## H2DE-4.0.18
#### ADDED
- doxygen documentation
#### FIXED
- crash when removing an object while it's lerp animated 

## H2DE-4.0.17
#### ADDED
- constexpr round, ceil and floor
- H2DE namespace for abs, round, ceil, floor, pow, randoms and lerp
#### FIXED
- getter for hitbox world rect with negative size
- crash after destroying button

## H2DE-4.0.16
#### ADDED
- get hitbox world rect

## H2DE-4.0.15
#### ADDED
- get texture pixel size

## H2DE-4.0.14
#### ADDED
- disable button
#### FIXED
- subpixel precision

## H2DE-4.0.13
#### ADDED
- button stop timeline function
#### FIXED
- button timeline not stopping when button removed 
- button mouse up called when mouse isn't on button's hitboxes
#### MODIFIED
- remove hitbox and surfaces from object now returns boolean instead of void

## H2DE-4.0.12
#### ADDED
- button callbacks gives timelineID to manage button animations
#### FIXED
- object memory management
- destroy inner text when destroying timer or button object

## H2DE-4.0.11
#### FIXED
- button on mouse up dot working if on mouse down is null
#### MODIFIED
- object data type as public

## H2DE-4.0.10
#### FIXED
- camera grid renndering

## H2DE-4.0.9
#### ADDED
- surface from objects getters with templates
#### FIXED
- axis origin not working for surfaces and hitboxes
- camera translate getter
- camera not working correctly with axis origins
- absolute object not rendering on top of non absolute ones

## H2DE-4.0.8
#### ADDED
- stop engine method
#### FIXED
- color and time lerp not accurate
- object opacity not working
- timer hours not rendering
- timelines not accurate

## H2DE-4.0.7
#### ADDED
- create, read, write json file
- type getters from json data

## H2DE-4.0.6
#### ADDED
- timer object
- pause, resume, toggle pause, is paused methods for timelines and delays
#### FIXED
- timeline with infinite loop offset
- timeline crash when duration < delta time
- button object crash when closing app

## H2DE-4.0.5
#### FIXED
- crash when closing app when using sprite
- sprite rendering
#### MODIFIED
- delays and timelines now returns H2DE_TimelineID (uint32tt) instead of unsigned int
- channel ids for volume are now H2DE_ChannelID (uint8_t) instead of int

## H2DE-4.0.4
#### ADDED
- rendered objects, surfaces and hitboxes getters
- button's text object getter
#### FIXED
- camera translate when changing origins
- object not rendering sometimes when changing origins
#### MODIFIED
- all pause sensitive parameters are true by default

## H2DE-4.0.3
#### ADDED
- camera grid on top
#### FIXED
- button's text memory leak 
- camera origins flips
- camera grid resizing on window resize
#### REMOVED
- hitbox snap

## H2DE-4.0.2
#### ADDED
- text surface rendering
- camera crosshair absolute size
- button text
#### FIXED
- color surface 1px overflow
- get current fps engine method is more accurate
- surface rotation rendering
- camera grid resizing
- button calls give pointer to button instead of object

## H2DE-4.0.1
#### ADDED
- camera grid crosshair
- camera grid world center axis distinct color
- color surface rendering
#### FIXED
- surface / hitbox rendering
- grid rendering
- optimized camera grid

## H2DE-4.0.0
#### ADDED
- color surface
- surface rendering

## H2DE-b-4.0.3
#### ADDED
- button events
- get mouse game pos
- get mouse interface pos
#### FIXED
- absolute object translation rendering
- absolute objecs not rendering

## H2DE-b-4.0.2
#### ADDED
- camera with for game objects
- camera with for interface objects
- camera grid
#### FIXED
- block size not accurate
- camera translate
- object collision detection
- min max rect getters
- delta time
- timeline and delay precision

## H2DE-b-4.0.1
#### ADDED
- delta time
- hitbox display
- debug mode (next frame, previous frame)
#### FIXED
- hitbox collisions
- hitbox snap
- camera object detection

## H2DE-b-4.0.0
#### REMAKE FROM SCRATCH
#### MODIFIED
- positions are center instead of top left
- classes methods are not friend anymore





## H2DE-3.2.15
#### ADDED
- easing functions take completed function as parameter

## H2DE-3.2.14
#### ADDED
- easing functions returns created timeline id
#### FIXED
- surface flip rendering

## H2DE-3.2.13
#### ADDED
- snap rect method
#### FIXED
- hide object

## H2DE-3.2.12
#### ADDED
- json typed value getters
- toggle pause
- get specific hitbox from object
- get specific hitbox world rect from object 
- hitbox rotation (snap every 90 deg)
#### FIXED
- renderer vsync
- game loop is 100% accurate to given fps

## H2DE-3.2.11
#### ADDED
- debug mode (frame per frame)

## H2DE-3.2.10
#### FIXED
- rotation on objects not working
- bar object not working

## H2DE-3.2.9
#### FIXED
- bar object and text object rendering
- if settings section does not exist when creating a key, section will be automatically created
- if settings key does not exist when setting its value, key will be automatically created

## H2DE-3.2.8
#### FIXED
- fliped object collision detection
- rotated surfaces not rendering
#### MODIFIED
- object position and size is now a rect

## H2DE-3.2.7
#### FIXED
- flips and rotations

## H2DE-3.2.6
#### FIXED
- transparent texture with black default background 

## H2DE-3.2.5
#### ADDED
- surface index

## H2DE-3.2.4
#### FIXED
- surface not refreshing when setting object rotation

## H2DE-3.2.3
#### ADDED
- hitboxes render on top of everything
- return collided face on collision
#### FIXED
- hitbox size rendering
- round position instead of ceil for pixel rendering 

## H2DE-3.2.2
#### FIXED
- object collision detection

## H2DE-3.2.1
#### ADDED
- null pointer error detection
- add surface to object
- remove surface to object
- optimized objects

## H2DE-3.2.0
#### ADDED
- objects have map of surface intead of single surface

## H2DE-3.1.8
#### FIXED
- path include

## H2DE-3.1.7
#### ADDED
- buttons calls return the button as parameters
#### FIXED
- surface rotation
#### MODIFIED
- restructured include folder

## H2DE-3.1.6
#### ADDED
- set surface rotation with easing
- button pause sensitive
- button mouse up
#### FIXED
- hidden object can't collide with others
- button hover 
#### MODIFIED
- loading assets loading bar

## H2DE-3.1.5
#### ADDED
- is song / sfx playing functions

## H2DE-3.1.4
#### FIXED
- absolute buttons not working if camera moved
- textures not supporting blend mode

## H2DE-3.1.3
#### FIXED
- absolute object not rendering if camera moved

## H2DE-3.1.2
#### FIXED
- crash if H2DE_StopEngine is called two times

## H2DE-3.1.1
#### ADDED
- hitbox flip when object is fliped

## H2DE-3.1.0
#### ADDED
- surface rotation
- surface flip
- 2d vector rotate method
- yx flip
- add flip
- set / get object rotation
- set / get object pivot
- set / get object flip
- objet rotation
- objet flip
- set surface color with easing
#### MODIFIED
- colors default alpha = max

## H2DE-3.0.7
#### ADDED
- set camera pos with easing
- camera contains rect
- get button surface
- create json file
- read json file
- write json file
#### FIXED
- camera contains object (at least one surface or hitbox on screen)

## H2DE-3.0.6
#### ADDED
- hitboxes debug
- easing effect for set object pos
- easing effect for set object size
- easing effect for set bar object value
- set bar min and max
- get basic object surface
- function to call hover, out and click calls for buttons
- set button onhover, onout and onclick
- set text object tets, font, font size, spacing, text align and color
#### FIXED
- first word is too long in text object

## H2DE-3.0.5
#### ADDED
- timeline
- camera object reference
- camera padding
- camera smoothing
- camera locked to reference
- camera padding from reference
#### FIXED
- object not rendering outside of camera
#### REMOVED
- delay loop

## H2DE-3.0.4
#### ADDED
- other lerp easing
- custom window ratio (default size's ratio)
- play song
- play sfx
#### FIXED
- delay not updating on pause

## H2DE-3.0.3
#### ADDED
- window ratio
- lerp with easing
- text rendering

## H2DE-3.0.2
#### ADDED
- set bar percentage
- surface set texture name, color, scale mode
- texture set src rect
- sprite set starting pos, size, spacing, nb frame, delay and pause sensitive
- stop delay
- window set size, min size, max size, set icon, set title, set fullscreen, set resizable, set grab
- wav files supported
- jpeg and bmp files supported
#### FIXED
- delays > 1000ms (division by 0)
- last settings line can't be modified

## H2DE-3.0.1
#### ADDED
- collision detection
- snap when collision
- get mouse pos (absolute or not)
- button click event
- button hover event
- button out event
- sprite starting pos
#### FIXED
- blockSize <= 0 (prevent division by 0)
- object memory clear
- surface memory clear

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
- += operators for `H2DE_LevelPos`, `H2DE_LevelSize`, `H2DE_Velocity` and `H2DE_LevelRect`
- -= operators for `H2DE_LevelPos`, `H2DE_LevelSize`, `H2DE_Velocity` and `H2DE_LevelRect`
- *= operators for `H2DE_LevelPos`, `H2DE_LevelSize` and `H2DE_Velocity`
- /= operators for `H2DE_LevelPos`, `H2DE_LevelSize` and `H2DE_Velocity`

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
