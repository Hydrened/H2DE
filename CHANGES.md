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
