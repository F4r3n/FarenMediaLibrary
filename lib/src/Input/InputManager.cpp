
#include "Input/InputManager.h"
#include <stdarg.h>
#include <iostream>
#include <cassert>
using namespace fm;

InputManager InputManager::_instance;


InputManager::InputManager(Window& window) {
    init(window);
}

void InputManager::init(Window& window) {

}



InputManager::InputManager() 
{
}


InputManager& InputManager::Get() {
    return _instance;
}


void InputManager::GetMousePosition(math::Vector2f& outPos)
{
	outPos = _mousePos;
}

bool InputManager::IsKeyPressed(FM_KEY key)
{
    return _keys[key];
}


bool InputManager::IsMouseButtonPressed(FM_MOUSE_KEY id)
{
	return _mouseKeys[id];
}

void InputManager::processEvents() 
{

	switch (_event.type)
	{
	case SDL_MOUSEWHEEL:
	{
		_scrollWheel.x += _event.wheel.x;
		_scrollWheel.y += _event.wheel.y;
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		_mouseKeys[SDL_BUTTON_LEFT - 1] = _event.button.button == SDL_BUTTON_LEFT;
		_mouseKeys[SDL_BUTTON_RIGHT - 1] = _event.button.button == SDL_BUTTON_RIGHT;
		_mouseKeys[SDL_BUTTON_MIDDLE - 1] = _event.button.button == SDL_BUTTON_MIDDLE;

		break;
	}
	case SDL_TEXTINPUT:
	{
		//return true;
		break;
	}
	case SDL_KEYDOWN:
	case SDL_KEYUP:
	{
		int key = _event.key.keysym.scancode;
		assert(key < 512);
		_keys[key] = (_event.type == SDL_KEYDOWN);
		_keyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
		_keyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
		_keyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
		_keySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
		break;
	}
	// Multi-viewport support
	case SDL_WINDOWEVENT:
		Uint8 window_event = _event.window.event;
		if (window_event == SDL_WINDOWEVENT_CLOSE || window_event == SDL_WINDOWEVENT_MOVED || window_event == SDL_WINDOWEVENT_RESIZED)
		{
			_closed = (_event.window.event == SDL_WINDOWEVENT_CLOSE);
		}
		break;
	}


}

void InputManager::CaptureMousePosition()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	_mousePos.x = (float)mouseX;
	_mousePos.y = (float)mouseY;
}


void InputManager::pollEvents() {
    _closed = false;
    while(SDL_PollEvent(&_event)) 
	{
        processEvents();
    }
	CaptureMousePosition();
}

InputManager::~InputManager() {
}
