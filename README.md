![Build Status](https://travis-ci.org/F4r3n/FarenMediaLibrary.svg?branch=master)
![Build Status Windows](https://ci.appveyor.com/api/projects/status/github/F4r3n/FarenMediaLibrary?branch=master&svg=true)

# FarenEngine

### Introduction
This library enables to create 2D or 3D games with lua scripts. The engine is linked to an entity system component.
The image below show the possibilities of the library.

![Link image](https://imgur.com/OpzrxbM.png)

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

* Clone the project with
git clone https://github.com/F4r3n/FarenMediaLibrary.git


```
mkdir build
cd build
cmake ..
make
```
It will create a static lib and the exe FarenEngine.

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
fm::Config config;
	config.name = "FML Engine";//Name of the window
	config.fpsWanted = 60;//Fps wanted
	config.width = 800;
	config.height = 600;
	fm::Application app(config);
	app.Init();//Init then engine


	fm::Window *window = app.GetWindow();
	fm::Engine *engine = app.GetEngine();

	while (!window->isClosed())
	{
		app.Update();
		window->swapBuffers();
	}
	app.DeInit();
```

### TODO

- [x] Load mesh
- [ ] 3D physics
- [ ] Culling Perspective
- [ ] 3D QuadTree
- [x] GUI/List entities
- [ ] GUI/Edit Shader
- [ ] GUI/Add texture
- [x] C++ Scripts //To improve
- [ ] Command buffer
- [x] Filesystem
...
