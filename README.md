# FarenMediaLibrary
It's a library to create 2D games.
## Dependencies

### Ubuntu

 sudo apt-get install libsoil-dev libglew-dev libglfw-dev

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