#pragma once
#include "Window.h"
#include <map>



namespace fm {

	enum FM_KEY {
		FM_KEY_UNKNOWN		 =  GLFW_KEY_UNKNOWN,
		FM_KEY_SPACE		 =  GLFW_KEY_SPACE,
		FM_KEY_APOSTROPHE    =  GLFW_KEY_APOSTROPHE,
		FM_KEY_COMMA		 =  GLFW_KEY_COMMA,
		FM_KEY_MINUS		 =  GLFW_KEY_MINUS,
		FM_KEY_PERIOD		 =  GLFW_KEY_PERIOD,
		FM_KEY_SLASH		 =  GLFW_KEY_SLASH,
		FM_KEY_0			 =  GLFW_KEY_0,
		FM_KEY_1			 =  GLFW_KEY_1,
		FM_KEY_2			 =  GLFW_KEY_2,
		FM_KEY_3			 =  GLFW_KEY_3,
		FM_KEY_4			 =  GLFW_KEY_4,
		FM_KEY_5			 =  GLFW_KEY_5,
		FM_KEY_6			 =  GLFW_KEY_6,
		FM_KEY_7			 =  GLFW_KEY_7,
		FM_KEY_8			 =  GLFW_KEY_8,
		FM_KEY_9			 =  GLFW_KEY_9,
		FM_KEY_SEMICOLON	 =  GLFW_KEY_SEMICOLON,
		FM_KEY_EQUAL		 =  GLFW_KEY_EQUAL,
		FM_KEY_A			 =  GLFW_KEY_A,
		FM_KEY_B			 =  GLFW_KEY_B,
		FM_KEY_C			 =  GLFW_KEY_C,
		FM_KEY_D			 =  GLFW_KEY_D,
		FM_KEY_E			 =  GLFW_KEY_E,
		FM_KEY_F			 =  GLFW_KEY_F,
		FM_KEY_G			 =  GLFW_KEY_G,
		FM_KEY_H			 =  GLFW_KEY_H,
		FM_KEY_I			 =  GLFW_KEY_I,
		FM_KEY_J			 =  GLFW_KEY_J,
		FM_KEY_K			 =  GLFW_KEY_K,
		FM_KEY_L			 =  GLFW_KEY_L,
		FM_KEY_M			 =  GLFW_KEY_M,
		FM_KEY_N			 =  GLFW_KEY_N,
		FM_KEY_O			 =  GLFW_KEY_O,
		FM_KEY_P			 =  GLFW_KEY_P,
		FM_KEY_Q			 =  GLFW_KEY_Q,
		FM_KEY_R			 =  GLFW_KEY_R,
		FM_KEY_S			 =  GLFW_KEY_S,
		FM_KEY_T			 =  GLFW_KEY_T,
		FM_KEY_U			 =  GLFW_KEY_U,
		FM_KEY_V			 =  GLFW_KEY_V,
		FM_KEY_W			 =  GLFW_KEY_W,
		FM_KEY_X			 =  GLFW_KEY_X,
		FM_KEY_Y			 =  GLFW_KEY_Y,
		FM_KEY_Z			 =  GLFW_KEY_Z,
		FM_KEY_LEFT_BRACKET  =  GLFW_KEY_LEFT_BRACKET,
		FM_KEY_BACKSLASH     =  GLFW_KEY_BACKSLASH,
		FM_KEY_RIGHT_BRACKET =  GLFW_KEY_RIGHT_BRACKET,
		FM_KEY_GRAVE_ACCENT  =  GLFW_KEY_GRAVE_ACCENT,
		FM_KEY_WORLD_1       =  GLFW_KEY_WORLD_1,
		FM_KEY_WORLD_2       =  GLFW_KEY_WORLD_2,

		FM_KEY_ESCAPE        =  GLFW_KEY_ESCAPE,
		FM_KEY_ENTER         =  GLFW_KEY_ENTER,
		FM_KEY_TAB           =  GLFW_KEY_TAB,
		FM_KEY_BACKSPACE     =  GLFW_KEY_BACKSPACE,
		FM_KEY_INSERT        =  GLFW_KEY_INSERT,
		FM_KEY_DELETE        =  GLFW_KEY_DELETE,
		FM_KEY_RIGHT         =  GLFW_KEY_RIGHT,
		FM_KEY_LEFT          =  GLFW_KEY_LEFT,
		FM_KEY_DOWN          =  GLFW_KEY_DOWN,
		FM_KEY_UP            =  GLFW_KEY_UP,
		FM_KEY_PAGE_UP       =  GLFW_KEY_PAGE_UP,
		FM_KEY_PAGE_DOWN     =  GLFW_KEY_PAGE_DOWN,
		FM_KEY_HOME          =  GLFW_KEY_HOME,
		FM_KEY_END           =  GLFW_KEY_END,
		FM_KEY_CAPS_LOCK     =  GLFW_KEY_CAPS_LOCK,
		FM_KEY_SCROLL_LOCK   =  GLFW_KEY_SCROLL_LOCK,
		FM_KEY_NUM_LOCK      =  GLFW_KEY_NUM_LOCK,
		FM_KEY_PRINT_SCREEN  =  GLFW_KEY_PRINT_SCREEN,
		FM_KEY_PAUSE         =  GLFW_KEY_PAUSE,
		FM_KEY_F1            =  GLFW_KEY_F1,
		FM_KEY_F2            =  GLFW_KEY_F2,
		FM_KEY_F3            =  GLFW_KEY_F3,
		FM_KEY_F4            =  GLFW_KEY_F4,
		FM_KEY_F5            =  GLFW_KEY_F5,
		FM_KEY_F6            =  GLFW_KEY_F6,
		FM_KEY_F7            =  GLFW_KEY_F7,
		FM_KEY_F8            =  GLFW_KEY_F8,
		FM_KEY_F9            =  GLFW_KEY_F9,
		FM_KEY_F10           =  GLFW_KEY_F10,
		FM_KEY_F11           =  GLFW_KEY_F11,
		FM_KEY_F12           =  GLFW_KEY_F12,
		FM_KEY_F13           =  GLFW_KEY_F13,
		FM_KEY_F14           =  GLFW_KEY_F14,
		FM_KEY_F15           =  GLFW_KEY_F15,
		FM_KEY_F16           =  GLFW_KEY_F16,
		FM_KEY_F17           =  GLFW_KEY_F17,
		FM_KEY_F18           =  GLFW_KEY_F18,
		FM_KEY_F19           =  GLFW_KEY_F19,
		FM_KEY_F20           =  GLFW_KEY_F20,
		FM_KEY_F21           =  GLFW_KEY_F21,
		FM_KEY_F22           =  GLFW_KEY_F22,
		FM_KEY_F23           =  GLFW_KEY_F23,
		FM_KEY_F24           =  GLFW_KEY_F24,
		FM_KEY_F25           =  GLFW_KEY_F25,
		FM_KEY_KP_0          =  GLFW_KEY_KP_0,
		FM_KEY_KP_1          =  GLFW_KEY_KP_1,
		FM_KEY_KP_2          =  GLFW_KEY_KP_2,
		FM_KEY_KP_3          =  GLFW_KEY_KP_3,
		FM_KEY_KP_4          =  GLFW_KEY_KP_4,
		FM_KEY_KP_5          =  GLFW_KEY_KP_5,
		FM_KEY_KP_6          =  GLFW_KEY_KP_6,
		FM_KEY_KP_7          =  GLFW_KEY_KP_7,
		FM_KEY_KP_8          =  GLFW_KEY_KP_8,
		FM_KEY_KP_9          =  GLFW_KEY_KP_9,
		FM_KEY_KP_DECIMAL    =  GLFW_KEY_KP_DECIMAL,
		FM_KEY_KP_DIVIDE     =  GLFW_KEY_KP_DIVIDE,
		FM_KEY_KP_MULTIPLY   =  GLFW_KEY_KP_MULTIPLY,
		FM_KEY_KP_SUBTRACT   =  GLFW_KEY_KP_SUBTRACT,
		FM_KEY_KP_ADD        =  GLFW_KEY_KP_ADD,
		FM_KEY_KP_ENTER      =  GLFW_KEY_KP_ENTER,
		FM_KEY_KP_EQUAL      =  GLFW_KEY_KP_EQUAL,
		FM_KEY_LEFT_SHIFT    =  GLFW_KEY_LEFT_SHIFT,
		FM_KEY_LEFT_CONTROL  =  GLFW_KEY_LEFT_CONTROL,
		FM_KEY_LEFT_ALT      =  GLFW_KEY_LEFT_ALT,
		FM_KEY_LEFT_SUPER    =  GLFW_KEY_LEFT_SUPER,
		FM_KEY_RIGHT_SHIFT   =  GLFW_KEY_RIGHT_SHIFT,
		FM_KEY_RIGHT_CONTROL =  GLFW_KEY_RIGHT_CONTROL,
		FM_KEY_RIGHT_ALT     =  GLFW_KEY_RIGHT_ALT,
		FM_KEY_RIGHT_SUPER   =  GLFW_KEY_RIGHT_SUPER,
		FM_KEY_MENU          =  GLFW_KEY_MENU
     
	};

	class InputManager
	{


	public:
		InputManager(Window &window);
		void pollEvents(Window &window);
		~InputManager();
		bool keyIsPressed(int key);
		bool keyIsReleased(int key);
		bool multipleKeysPressed(int key, ...);
	private:
		static std::map <int, bool> keys;
		static std::map <int, bool> keysReleased;
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
		//static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		GLFWwindow* window;

	};
}

