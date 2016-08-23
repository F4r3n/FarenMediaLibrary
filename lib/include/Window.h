#pragma once
// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include "ResourcesManager.h"
#include "Camera.h"
#include "Shape.h"
namespace fm {
	class Window
	{
		friend class InputManager;
	public:
		Window(int width, int height, const std::string &name);
		~Window();
		void swapBuffers();
		void clear();
		bool isClosed();

		void draw(Shape &shape);
		void frameLimit(unsigned short fps);
	private:
		void events();
		int init(GLFWwindow *window);
		GLFWwindow* window;
		int width;
		int height;
		Camera camera;

		double wait_time = 1/60.0f;
		double curr_frame_time = 0;
		double dur = 0;
		double frame_start = 0;
	};
}

