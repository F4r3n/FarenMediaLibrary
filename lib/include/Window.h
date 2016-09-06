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
#include "NonCopyable.h"

namespace fm {
	class Window : public fm_system::NonCopyable
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
		void update(float fps);
		
	private:
		void initFrameBuffer();
		void events();
		int generateAttachmentTexture(bool depth, bool stencil);
		int init(GLFWwindow *window);
		void createQuadScreen();
		void createShaders();
		void postProcess(bool horizontal);
		void errorDisplay();
		GLFWwindow* window;
		int width;
		int height;
		Camera camera;

		double wait_time = 1/60.0f;
		double curr_frame_time = 0;
		double dur = 0;
		double frame_start = 0;

		GLuint framebuffer;
		GLuint quadVAO;
		GLuint textureColorbuffer[2];

		GLuint pingpongFBO[2];
    	GLuint pingpongColorbuffers[2];
	};
}

