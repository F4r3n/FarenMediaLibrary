# FarenMediaLibrary
It's a library to create 2D games with lua scripts
## Dependencies
You need to compile lua lib https://www.lua.org/download.html (64Bits need to add -fPIC in the Makefile)
To bind with lua https://github.com/ThePhD/sol2
### Ubuntu
```
 sudo apt-get install libsoil-dev libglew-dev libglfw-dev
```
## How to use ?
After compiling the library, you can use it as any library.

## Sample
```
int main() {
	fm::Window window(800, 600, "test");
	fm::InputManager manager(window);
	fm::Camera camera(800, 600);

	fm::Rectangle rect(255,255,255);
	rect.scale(100,100);


	while(!window.isClosed()) {
		manager.pollEvents(window);
		window.clear();

		camera.view();
		camera.draw(rect);

		window.swapBuffers();

	}

	return 0;
}
```