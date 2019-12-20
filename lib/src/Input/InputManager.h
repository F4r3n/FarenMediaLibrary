#pragma once
#include "Core/Math/Vector2.h"
#include "NonCopyable.h"
#include <SDL_events.h>
namespace fm
{

	class Window;

enum FM_KEY {
    FM_KEY_UNKNOWN = SDL_SCANCODE_UNKNOWN,
    FM_KEY_SPACE = SDL_SCANCODE_SPACE,
    FM_KEY_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    FM_KEY_COMMA = SDL_SCANCODE_COMMA,
    FM_KEY_MINUS = SDL_SCANCODE_MINUS,
    FM_KEY_PERIOD = SDL_SCANCODE_PERIOD,
    FM_KEY_SLASH = SDL_SCANCODE_SLASH,
    FM_KEY_0 = SDL_SCANCODE_0,
    FM_KEY_1 = SDL_SCANCODE_1,
    FM_KEY_2 = SDL_SCANCODE_2,
    FM_KEY_3 = SDL_SCANCODE_3,
    FM_KEY_4 = SDL_SCANCODE_4,
    FM_KEY_5 = SDL_SCANCODE_5,
    FM_KEY_6 = SDL_SCANCODE_6,
    FM_KEY_7 = SDL_SCANCODE_7,
    FM_KEY_8 = SDL_SCANCODE_8,
    FM_KEY_9 = SDL_SCANCODE_9,
    FM_KEY_SEMICOLON = SDL_SCANCODE_SEMICOLON,
    FM_KEY_EQUALS = SDL_SCANCODE_EQUALS,
    FM_KEY_A = SDL_SCANCODE_A,
    FM_KEY_B = SDL_SCANCODE_B,
    FM_KEY_C = SDL_SCANCODE_C,
    FM_KEY_D = SDL_SCANCODE_D,
    FM_KEY_E = SDL_SCANCODE_E,
    FM_KEY_F = SDL_SCANCODE_F,
    FM_KEY_G = SDL_SCANCODE_G,
    FM_KEY_H = SDL_SCANCODE_H,
    FM_KEY_I = SDL_SCANCODE_I,
    FM_KEY_J = SDL_SCANCODE_J,
    FM_KEY_K = SDL_SCANCODE_K,
    FM_KEY_L = SDL_SCANCODE_L,
    FM_KEY_M = SDL_SCANCODE_M,
    FM_KEY_N = SDL_SCANCODE_N,
    FM_KEY_O = SDL_SCANCODE_O,
    FM_KEY_P = SDL_SCANCODE_P,
    FM_KEY_Q = SDL_SCANCODE_Q,
    FM_KEY_R = SDL_SCANCODE_R,
    FM_KEY_S = SDL_SCANCODE_S,
    FM_KEY_T = SDL_SCANCODE_T,
    FM_KEY_U = SDL_SCANCODE_U,
    FM_KEY_V = SDL_SCANCODE_V,
    FM_KEY_W = SDL_SCANCODE_W,
    FM_KEY_X = SDL_SCANCODE_X,
    FM_KEY_Y = SDL_SCANCODE_Y,
    FM_KEY_Z = SDL_SCANCODE_Z,
    FM_KEY_LEFT_BRACKET = SDL_SCANCODE_LEFTBRACKET,
    FM_KEY_BACKSLASH = SDL_SCANCODE_BACKSLASH,
    FM_KEY_RIGHT_BRACKET = SDL_SCANCODE_RIGHTBRACKET,
    FM_KEY_GRAVE_ACCENT = SDL_SCANCODE_GRAVE,
    //FM_KEY_WORLD_1 = SDL_SCANCODE_WORLD_1,
    //FM_KEY_WORLD_2 = SDL_SCANCODE_WORLD_2,

    FM_KEY_ESCAPE = SDL_SCANCODE_ESCAPE,
    FM_KEY_ENTER = SDL_SCANCODE_RETURN,
    FM_KEY_TAB = SDL_SCANCODE_TAB,
    FM_KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
    FM_KEY_INSERT = SDL_SCANCODE_INSERT,
    FM_KEY_DELETE = SDL_SCANCODE_DELETE,
    FM_KEY_RIGHT = SDL_SCANCODE_RIGHT,
    FM_KEY_LEFT = SDL_SCANCODE_LEFT,
    FM_KEY_DOWN = SDL_SCANCODE_DOWN,
    FM_KEY_UP = SDL_SCANCODE_UP,
    FM_KEY_PAGE_UP = SDL_SCANCODE_PAGEUP,
    FM_KEY_PAGE_DOWN = SDL_SCANCODE_PAGEDOWN,
    FM_KEY_HOME = SDL_SCANCODE_HOME,
    FM_KEY_END = SDL_SCANCODE_END,
    FM_KEY_CAPS_LOCK = SDL_SCANCODE_CAPSLOCK,
    FM_KEY_SCROLL_LOCK = SDL_SCANCODE_SCROLLLOCK,
    FM_KEY_NUM_LOCK = SDL_SCANCODE_NUMLOCKCLEAR,
    FM_KEY_PRINT_SCREEN = SDL_SCANCODE_PRINTSCREEN,
    FM_KEY_PAUSE = SDL_SCANCODE_PAUSE,
    FM_KEY_F1 = SDL_SCANCODE_F1,
    FM_KEY_F2 = SDL_SCANCODE_F2,
    FM_KEY_F3 = SDL_SCANCODE_F3,
    FM_KEY_F4 = SDL_SCANCODE_F4,
    FM_KEY_F5 = SDL_SCANCODE_F5,
    FM_KEY_F6 = SDL_SCANCODE_F6,
    FM_KEY_F7 = SDL_SCANCODE_F7,
    FM_KEY_F8 = SDL_SCANCODE_F8,
    FM_KEY_F9 = SDL_SCANCODE_F9,
    FM_KEY_F10 = SDL_SCANCODE_F10,
    FM_KEY_F11 = SDL_SCANCODE_F11,
    FM_KEY_F12 = SDL_SCANCODE_F12,
    FM_KEY_F13 = SDL_SCANCODE_F13,
    FM_KEY_F14 = SDL_SCANCODE_F14,
    FM_KEY_F15 = SDL_SCANCODE_F15,
    FM_KEY_F16 = SDL_SCANCODE_F16,
    FM_KEY_F17 = SDL_SCANCODE_F17,
    FM_KEY_F18 = SDL_SCANCODE_F18,
    FM_KEY_F19 = SDL_SCANCODE_F19,
    FM_KEY_F20 = SDL_SCANCODE_F20,
    FM_KEY_F21 = SDL_SCANCODE_F21,
    FM_KEY_F22 = SDL_SCANCODE_F22,
    FM_KEY_F23 = SDL_SCANCODE_F23,
    FM_KEY_F24 = SDL_SCANCODE_F24,
    FM_KEY_KP_0 = SDL_SCANCODE_KP_0,
    FM_KEY_KP_1 = SDL_SCANCODE_KP_1,
    FM_KEY_KP_2 = SDL_SCANCODE_KP_2,
    FM_KEY_KP_3 = SDL_SCANCODE_KP_3,
    FM_KEY_KP_4 = SDL_SCANCODE_KP_4,
    FM_KEY_KP_5 = SDL_SCANCODE_KP_5,
    FM_KEY_KP_6 = SDL_SCANCODE_KP_6,
    FM_KEY_KP_7 = SDL_SCANCODE_KP_7,
    FM_KEY_KP_8 = SDL_SCANCODE_KP_8,
    FM_KEY_KP_9 = SDL_SCANCODE_KP_9,
    FM_KEY_KP_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
    FM_KEY_KP_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
    FM_KEY_KP_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
    FM_KEY_KP_SUBTRACT = SDL_SCANCODE_KP_MINUS,
    FM_KEY_KP_ADD = SDL_SCANCODE_KP_PLUS,
    FM_KEY_KP_ENTER = SDL_SCANCODE_KP_ENTER,
    FM_KEY_KP_EQUAL = SDL_SCANCODE_KP_EQUALS,
    FM_KEY_LEFT_SHIFT = SDL_SCANCODE_LSHIFT,
    FM_KEY_LEFT_CONTROL = SDL_SCANCODE_LCTRL,
    FM_KEY_LEFT_ALT = SDL_SCANCODE_LALT,
   // FM_KEY_LEFT_SUPER = SDL_SCANCODE_LEFT_SUPER,
    FM_KEY_RIGHT_SHIFT = SDL_SCANCODE_RSHIFT,
    FM_KEY_RIGHT_CONTROL = SDL_SCANCODE_RCTRL,
    FM_KEY_RIGHT_ALT = SDL_SCANCODE_RALT,
    //FM_KEY_RIGHT_SUPER = SDL_SCANCODE_RIGHT_SUPER,
    FM_KEY_MENU = SDL_SCANCODE_MENU

};


enum FM_MOUSE_KEY
{
	FM_MOUSE_LEFT = 0,
	FM_MOUSE_MIDDLE = 1,
	FM_MOUSE_RIGHT = 2,
	FM_MOUSE_LAST

};

class InputManager : protected fm_system::NonCopyable
{

public:
    
    void pollEvents();
    void processEvents();
    ~InputManager();
    bool IsKeyPressed(FM_KEY key);
	bool IsMouseButtonPressed(FM_MOUSE_KEY id);

    void GetMousePosition(math::Vector2f & outPos);

    static InputManager& Get();
    void init(Window& window);

    SDL_Event& getLastEvent() {return _event;}
    bool isClosed() {return _closed;}
	void CaptureMousePosition();
private:
	InputManager();
	InputManager(Window& window);

    bool _keys[SDL_NUM_SCANCODES];
	bool _keyCtrl;
	bool _keyAlt;
	bool _keySuper;
	bool _keyShift;
	bool _mouseKeys[FM_MOUSE_LAST];
	fm::math::vec2 _mousePos;
	fm::math::vec2 _scrollWheel;

    SDL_Event _event;

    static InputManager _instance;

    
    bool _closed = false;

};
}
