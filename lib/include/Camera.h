#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shape.h"
#include "Tag.h"
namespace fm {
	struct ViewPort {
		float x;
		float y;
		float width;
		float height;
	};

	class Camera
	{
	public:
		Camera();
		Camera(int width, int height);
		Camera(int width, int height, const ViewPort &view, float zoom = 1);
		glm::mat4 getProjection() const;
		void draw(Shape &shape);

		void view();
		~Camera();
		void setTag(int tag);
		int tag = tg::ALL;

		void moveTo(int x, int y);
		void rotate(float angle);
		void apply();
	private:
		int width;
		int height;

		float x = 0;
		float y = 0;
		float angle = 0;

		glm::mat4 projection;
		ViewPort viewPort;

		glm::mat4 viewMatrix;
		
	};
}

