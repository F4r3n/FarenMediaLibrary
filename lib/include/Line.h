#pragma once
#include "Drawable.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "Segment.h"
namespace fm {
class Line : public Drawable {
	public:
		Line(const Vector2f &startPoint, const Vector2f &endPoint);
		Line(const Segment &Segment);
		~Line();
		const std::string getNameShader() const;
		void draw(Shader &shader);
	private:
		Line();
		void init(const Vector2f &startPoint, const Vector2f &endPoint);

		Rectangle rect;
};
}