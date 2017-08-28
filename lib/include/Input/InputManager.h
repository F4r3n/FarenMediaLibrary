#pragma once
#include "Window.h"
#include <map>
#include "Core/Math/Vector2.h"
#include "NonCopyable.h"
#include <SDL_events.h>
namespace fm
{

enum FM_KEY {
    FM_KEY_UNKNOWN = SDLK_UNKNOWN,
    FM_KEY_SPACE = SDLK_SPACE,
    FM_KEY_APOSTROPHE = SDLK_QUOTE,
    FM_KEY_COMMA = SDLK_COMMA,
    FM_KEY_MINUS = SDLK_MINUS,
    FM_KEY_PERIOD = SDLK_PERIOD,
    FM_KEY_SLASH = SDLK_SLASH,
    FM_KEY_0 = SDLK_0,
    FM_KEY_1 = SDLK_1,
    FM_KEY_2 = SDLK_2,
    FM_KEY_3 = SDLK_3,
    FM_KEY_4 = SDLK_4,
    FM_KEY_5 = SDLK_5,
    FM_KEY_6 = SDLK_6,
    FM_KEY_7 = SDLK_7,
    FM_KEY_8 = SDLK_8,
    FM_KEY_9 = SDLK_9,
    FM_KEY_SEMICOLON = SDLK_SEMICOLON,
    FM_KEY_EQUALS = SDLK_EQUALS,
    FM_KEY_A = SDLK_a,
    FM_KEY_B = SDLK_b,
    FM_KEY_C = SDLK_c,
    FM_KEY_D = SDLK_d,
    FM_KEY_E = SDLK_e,
    FM_KEY_F = SDLK_f,
    FM_KEY_G = SDLK_g,
    FM_KEY_H = SDLK_h,
    FM_KEY_I = SDLK_i,
    FM_KEY_J = SDLK_j,
    FM_KEY_K = SDLK_k,
    FM_KEY_L = SDLK_l,
    FM_KEY_M = SDLK_m,
    FM_KEY_N = SDLK_n,
    FM_KEY_O = SDLK_o,
    FM_KEY_P = SDLK_p,
    FM_KEY_Q = SDLK_q,
    FM_KEY_R = SDLK_r,
    FM_KEY_S = SDLK_s,
    FM_KEY_T = SDLK_t,
    FM_KEY_U = SDLK_u,
    FM_KEY_V = SDLK_v,
    FM_KEY_W = SDLK_w,
    FM_KEY_X = SDLK_x,
    FM_KEY_Y = SDLK_y,
    FM_KEY_Z = SDLK_z,
    FM_KEY_LEFT_BRACKET = SDLK_LEFTBRACKET,
    FM_KEY_BACKSLASH = SDLK_BACKSLASH,
    FM_KEY_RIGHT_BRACKET = SDLK_RIGHTBRACKET,
    //FM_KEY_GRAVE_ACCENT = SDLK_GRAVEACCENT,
    //FM_KEY_WORLD_1 = SDLK_WORLD_1,
    //FM_KEY_WORLD_2 = SDLK_WORLD_2,

    FM_KEY_ESCAPE = SDLK_ESCAPE,
    FM_KEY_ENTER = SDLK_RETURN,
    FM_KEY_TAB = SDLK_TAB,
    FM_KEY_BACKSPACE = SDLK_BACKSPACE,
    FM_KEY_INSERT = SDLK_INSERT,
    FM_KEY_DELETE = SDLK_DELETE,
    FM_KEY_RIGHT = SDLK_RIGHT,
    FM_KEY_LEFT = SDLK_LEFT,
    FM_KEY_DOWN = SDLK_DOWN,
    FM_KEY_UP = SDLK_UP,
    FM_KEY_PAGE_UP = SDLK_PAGEUP,
    FM_KEY_PAGE_DOWN = SDLK_PAGEDOWN,
    FM_KEY_HOME = SDLK_HOME,
    FM_KEY_END = SDLK_END,
    FM_KEY_CAPS_LOCK = SDLK_CAPSLOCK,
    FM_KEY_SCROLL_LOCK = SDLK_SCROLLLOCK,
    FM_KEY_NUM_LOCK = SDLK_NUMLOCKCLEAR,
    FM_KEY_PRINT_SCREEN = SDLK_PRINTSCREEN,
    FM_KEY_PAUSE = SDLK_PAUSE,
    FM_KEY_F1 = SDLK_F1,
    FM_KEY_F2 = SDLK_F2,
    FM_KEY_F3 = SDLK_F3,
    FM_KEY_F4 = SDLK_F4,
    FM_KEY_F5 = SDLK_F5,
    FM_KEY_F6 = SDLK_F6,
    FM_KEY_F7 = SDLK_F7,
    FM_KEY_F8 = SDLK_F8,
    FM_KEY_F9 = SDLK_F9,
    FM_KEY_F10 = SDLK_F10,
    FM_KEY_F11 = SDLK_F11,
    FM_KEY_F12 = SDLK_F12,
    FM_KEY_F13 = SDLK_F13,
    FM_KEY_F14 = SDLK_F14,
    FM_KEY_F15 = SDLK_F15,
    FM_KEY_F16 = SDLK_F16,
    FM_KEY_F17 = SDLK_F17,
    FM_KEY_F18 = SDLK_F18,
    FM_KEY_F19 = SDLK_F19,
    FM_KEY_F20 = SDLK_F20,
    FM_KEY_F21 = SDLK_F21,
    FM_KEY_F22 = SDLK_F22,
    FM_KEY_F23 = SDLK_F23,
    FM_KEY_F24 = SDLK_F24,
    FM_KEY_KP_0 = SDLK_KP_0,
    FM_KEY_KP_1 = SDLK_KP_1,
    FM_KEY_KP_2 = SDLK_KP_2,
    FM_KEY_KP_3 = SDLK_KP_3,
    FM_KEY_KP_4 = SDLK_KP_4,
    FM_KEY_KP_5 = SDLK_KP_5,
    FM_KEY_KP_6 = SDLK_KP_6,
    FM_KEY_KP_7 = SDLK_KP_7,
    FM_KEY_KP_8 = SDLK_KP_8,
    FM_KEY_KP_9 = SDLK_KP_9,
    FM_KEY_KP_DECIMAL = SDLK_KP_DECIMAL,
    FM_KEY_KP_DIVIDE = SDLK_KP_DIVIDE,
    FM_KEY_KP_MULTIPLY = SDLK_KP_MULTIPLY,
    FM_KEY_KP_SUBTRACT = SDLK_KP_MINUS,
    FM_KEY_KP_ADD = SDLK_KP_PLUS,
    FM_KEY_KP_ENTER = SDLK_KP_ENTER,
    FM_KEY_KP_EQUAL = SDLK_KP_EQUALS,
    FM_KEY_LEFT_SHIFT = SDLK_LSHIFT,
    FM_KEY_LEFT_CONTROL = SDLK_LCTRL,
    FM_KEY_LEFT_ALT = SDLK_LALT,
   // FM_KEY_LEFT_SUPER = SDLK_LEFT_SUPER,
    FM_KEY_RIGHT_SHIFT = SDLK_RSHIFT,
    FM_KEY_RIGHT_CONTROL = SDLK_RCTRL,
    FM_KEY_RIGHT_ALT = SDLK_RALT,
    //FM_KEY_RIGHT_SUPER = SDLK_RIGHT_SUPER,
    FM_KEY_MENU = SDLK_MENU

};

class InputManager : protected fm_system::NonCopyable
{

public:
    InputManager(Window& window);
    InputManager();
    void pollEvents();

    ~InputManager();
    bool keyIsPressed(int key);
    bool keyIsReleased(int key);
    bool multipleKeysPressed(int key, ...);
    void getMousePosition(math::Vector2d & pos);
    void getMousePosition(double& posX, double& posY);
    static float getMousePositionX();
    static float getMousePositionY();

    int getMouseButton(int id);
    static InputManager& getInstance();
    void init(Window& window);
   // void init(GLFWwindow* window);

    int worldKeyboard(int key);
    const SDL_Event& getLastEvent() {return event;}
    bool isClosed() {return closed;}
private:
    static std::map<int, bool> keys;
    static std::map<int, bool> keysReleased;
    const Uint8 *state;
    SDL_Event event;
    //static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
    //static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static InputManager _instance;
    // static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    // static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    //GLFWwindow* window;
    bool typeKeyboard = true; /*false = querty | true = azerty*/
    static float posX;
    static float posY;
    
    bool closed = false;

};
}
