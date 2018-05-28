![Build Status](https://travis-ci.org/F4r3n/FarenMediaLibrary.svg?branch=master)


# FarenEngine

### Introduction
This library enables to create 2D or 3D games with lua scripts. The engine is linked to an entity system component.
The image below show the possibilities of the library.

![Link image](http://i.imgur.com/sL3IehG.png)

For the moment, the FarenEngine can :
+ Camera system 
+ Display simple shapes
+ Sound
+ Text rendering
+ Physics
+ Lua scripting
+ Forward/Deferred
+ Light effects
+ Serialization systeam
### Installation


To install the whole project:
```
mkdir build
cd build
cmake .. -DFML_BUILD_SHARED=ON
make
```



### List of dependencies

The engine needs different libraries to work:

+ Box2D
+ glew
+ openal
+ sdl2
+ lua 5.3
+ ECS (Custom)
+ Opengl

The dependencies are all in the folder extlib.

### How to use it ?

#### Simple Window

```c++
//To create a simple window where all the rendering will be made
Window window(800, 600, "Simple Window");

/*Initialize your first entity there*/

//initialize the engine, all the logic is there. 
//The entities previously made will be created.
Engine engine;
engine.init();

//The engine is ready and runs until the window is closed
engine.run(window);
```

### TODO

- [ ] Load mesh
- [ ] 3D physics
- [ ] Culling Perspective
- [ ] 3D QuadTree
- [ ] GUI/List entities
- [ ] GUI/Edit Shader
- [ ] GUI/Add texture
- [ ] C++ Scripts
...
