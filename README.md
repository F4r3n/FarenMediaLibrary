# FarenMediaLibrary
[![Build Status](https://travis-ci.org/F4r3n/FarenMediaLibrary.svg?branch=master)](https://travis-ci.org/F4r3n/FarenMediaLibrary)
It's a library to create 2D games with lua scripts.
## Dependencies
You need to compile lua lib https://www.lua.org/download.html (64Bits need to add -fPIC in the Makefile).
```
- tar zxf lua-5.3.3.tar.gz
- cd lua-5.3.3
- sudo make linux install MYCFLAGS='-fPIC'

```
To bind with lua, FML use sol2 https://github.com/ThePhD/sol2
### Ubuntu
GLFW 3.3 is required
```
 sudo apt-get install libglew-dev
 cd extlib/glfw
 cmake .
 sudo make install
 cd ../../
 mkdir build
 cd build
 cmake ..
 make
```
## How to use ?
After compiling the library, you can use it as any library.

## Sample

### To draw a simple white rect
```
int main() {
	fm::Window window(800, 600, "test");
	fm::InputManager::getInstance().init(window);
	fm::Camera camera(800, 600);

	fm::Rectangle rect(255,255,255);
	rect.scale(100,100);


	while(!window.isClosed()) {
		window.update(60);

		camera.view();
		camera.draw(rect);

		window.swapBuffers();

	}

	return 0;
}
```
### Manage particles
To create a funtain like particle
```
fm::ParticleGenerator particle(100,100, 100, texture);
	particle.setGravity(0,10);
	particle.setVelocity(fm::Vector2<float>(10,10), fm::Vector2<float>(100,100));
	particle.setShape(fm::pa::SHAPE::CIRCLE);
	particle.setLife(fm::Vector2f(0.1,5));
	particle.initParticles();
```
Then in the main loop
```
	particle.update(fm::Time::dt);
	camera.draw(particle);

```

###Manage lua scripts
```
	fm::ScriptManager scriptManager;
	scriptManager.registerScript("../assets/test.lua");
	scriptManager.init();
	scriptManager.start();
```
In the main loop
```
	scriptManager.update(fm::Time::dt);
	sprite.move(scriptManager.getScript("../assets/test.lua")->getTransform().position); 
```
