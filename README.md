# FarenMediaLibrary
![Build Status](https://travis-ci.org/F4r3n/FarenMediaLibrary.svg?branch=master)

It's a library to create 2D games with lua scripts.


To bind with lua, FML use sol2 https://github.com/ThePhD/sol2

## Ubuntu
GLFW 3.3, GLEW, GLM, SOL2 are required.
```
 sudo apt-get install libglew-dev
 
```
Most of the dependencies are in submodules, you just have to create the shared files.

#### Lua
You need to compile lua lib https://www.lua.org/download.html (64Bits need to add -fPIC in the Makefile).

```
tar zxf lua-5.3.3.tar.gz
cd lua-5.3.3
sudo make linux install MYCFLAGS='-fPIC'

```


## How to use ?
After compiling the library, you can use it as any library.

## Sample

### To draw a simple white rect
```C++
int main() {
	//Create a window of width 800 and height 600 with the name "Sample"
	fm::Window window(800, 600, "Sample");

	//Create a camera which takes the whole screen, it's possible to create a mini map easily
	fm::Camera camera(800, 600);

	//Create a white rectangle
	fm::Rectangle rect(255,255,255);
	rect.scale(100,100);


	while(!window.isClosed()) {
		//Update the inputs of the window and limmit the frequency to 60FPS
		window.update(60);

		//Apply a camera, all objects drawn after take that view
		camera.view();
		camera.draw(rect);

		//Clear the screen, apply post effect and swap buffers
		window.swapBuffers();

	}

	return 0;
}
```

### Glow
![Bolt](http://i.imgur.com/60LuH4z.png)

Possibility to make an object glow with one line, the image above shows a lightning effect and a rectangle with a glow effect.

```C++
setBloom(true)
```


### Create post-Effects
You can easily create your own post effect

```C++
fm::PostProcessor postProcess;
postProcess.loadShader("name", "path");
postProcess.activate("name");

```

### Manage particles
To create a funtain like particle

```C++
fm::ParticleGenerator particle(100,100, 100, texture);
	particle.setGravity(0,10);
	particle.setVelocity(fm::Vector2<float>(10,10), fm::Vector2<float>(100,100));
	particle.setShape(fm::pa::SHAPE::CIRCLE);
	particle.setLife(fm::Vector2f(0.1,5));
	particle.initParticles();
```
Then in the main loop

```C++
	particle.update(fm::Time::dt);
	camera.draw(particle);

```

### Manage lua scripts

```C++
	fm::ScriptManager scriptManager;
	scriptManager.registerScript("../assets/test.lua");
	scriptManager.init();
	scriptManager.start();
```
In the main loop

```C++
	scriptManager.update(fm::Time::dt);
	sprite.move(scriptManager.getScript("../assets/test.lua")->getTransform().position); 
```
