#include "Line.h"

using namespace fm;
Line::Line(const Vector2f &startPoint, const Vector2f &endPoint) {
	init(startPoint, endPoint);
}

Line::Line(const Segment &segment) {
	init(segment.getStart(), segment.getEnd());
}

void Line::init(const Vector2f &startPoint, const Vector2f &endPoint) {
	int distance = sqrt((startPoint.x - endPoint.x)*(startPoint.x - endPoint.x) 
					+ (startPoint.y - endPoint.y)*(startPoint.y - endPoint.y));

	float degree = acos((float)std::abs(startPoint.x - endPoint.x)/distance);
	rect.move(startPoint);
	rect.scale(distance, 3);
	rect.rotate(degree);
	float moveY = (float)(distance/2)*sin(degree);
	float moveX = (float)(distance/2) - cos(degree)*distance/2;
	rect.move(Vector2i{rect.getPosition().x - (int)moveX,rect.getPosition().y + (int)moveY});
}


Line::~Line() {}

const std::string Line::getNameShader() const {
	return "default";
}

void Line::draw(Shader &shader) {
	rect.draw(shader);
}