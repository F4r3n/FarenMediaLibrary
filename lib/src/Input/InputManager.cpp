
#include "Input/InputManager.h"
#include <stdarg.h>
#include <cassert>
using namespace fm;

InputManager InputManager::_instance;


InputManager::InputManager(Window& window) {
    init(window);
}

void InputManager::init(Window&) {

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

void InputManager::_ProcessEvents() 
{

	switch (_event.type)
	{
	case SDL_EVENT_MOUSE_WHEEL:
	{
		_scrollWheel.x += _event.wheel.x;
		_scrollWheel.y += _event.wheel.y;
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_UP:
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	{
		_mouseKeys[SDL_BUTTON_LEFT - 1] = _event.button.button == SDL_BUTTON_LEFT && (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
		_mouseKeys[SDL_BUTTON_RIGHT - 1] = _event.button.button == SDL_BUTTON_RIGHT && (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
		_mouseKeys[SDL_BUTTON_MIDDLE - 1] = _event.button.button == SDL_BUTTON_MIDDLE && (_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);

		break;
	}
	case SDL_EVENT_TEXT_INPUT:
	{
		//return true;
		break;
	}
	case SDL_EVENT_KEY_DOWN:
	case SDL_EVENT_KEY_UP:
	{
		int key = _event.key.keysym.scancode;
		assert(key < 512);
		_keys[key] = (_event.type == SDL_EVENT_KEY_DOWN);
		_keyShift = ((SDL_GetModState() & SDL_KMOD_SHIFT) != 0);
		_keyCtrl = ((SDL_GetModState() & SDL_KMOD_CTRL) != 0);
		_keyAlt = ((SDL_GetModState() & SDL_KMOD_ALT) != 0);
		_keySuper = ((SDL_GetModState() & SDL_KMOD_GUI) != 0);
		break;
	}
	// Multi-viewport support
	case SDL_EVENT_WINDOW_MOVED:
	case SDL_EVENT_WINDOW_RESIZED:
		break;
	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		_closed = true;
		break;
	}


}

void InputManager::CaptureMousePosition()
{
	SDL_GetMouseState(&_mousePos.x, &_mousePos.y);
}

void InputManager::PollEvents(std::function<void(const SDL_Event&)>&& inCallBack)
{
	//memset(_mouseKeys, 0, sizeof(_mouseKeys));
	//memset(_keys, 0, sizeof(_keys));

	while (SDL_PollEvent(&_event))
	{
		_ProcessEvents();
		if (inCallBack != nullptr)
		{
			inCallBack(_event);
		}
	}
	
	CaptureMousePosition();
}



void InputManager::_PollEvents() {
    _closed = false;
    while(SDL_PollEvent(&_event)) 
	{
        _ProcessEvents();
    }
	CaptureMousePosition();
}

InputManager::~InputManager() {
}
