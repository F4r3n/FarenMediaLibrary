![Build Status](https://travis-ci.org/F4r3n/FarenMediaLibrary.svg?branch=master)


# FarenEngine

### Introduction
This library enables to create 2D games with lua scripts. The engine uses the ECS system to work.
The image below show the possibilities of the library.

![Link image](http://i.imgur.com/sL3IehG.png)

For the moment, the FarenEngine can :
+ Camera system 
+ Display simple shapes
+ Light effects
+ Sound
+ Text rendering
+ Physics
+ Lua scripting

### Installation

#### Linux system

To install the project under linux just go in the main directory and execute the `Setup.sh` which will install all the dependencies.
Then go inside a `build` directory created and execute the cmake and makefile generated.

#### Windows System

Under windows, no script have been made.

### List of dependencies

The engine needs different libraries to work
+ sndfile
+ Box2D
+ glm
+ glew
+ openal
+ glfw

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

