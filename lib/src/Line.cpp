#include "Line.h"

using namespace fm;
Line::Line(const Vector2f &startPoint, const Vector2f &endPoint) {
	init(startPoint, endPoint);
}

Line::Line(const Segment &segment) {
	init(segment.getStart(), segment.getEnd());
}

void Line::init(const Vector2f &startPoint, const Vector2f &endPoint) {
	if(startPoint.x == endPoint.x && startPoint.y ==  endPoint.y) return;

	float distance = sqrt((startPoint.x - endPoint.x)*(startPoint.x - endPoint.x) 
					+ (startPoint.y - endPoint.y)*(startPoint.y - endPoint.y));

	//std::cout << (float)std::abs(startPoint.x - endPoint.x)/distance << std::endl;
	//std::cout << startPoint.x << " " << startPoint.y << " " << endPoint.x << " " << endPoint.y << std::endl;
	float degree = 0;
	if((float)(endPoint.x - startPoint.x)/distance <= 1) {
		if(startPoint.y < endPoint.y)
	 		degree = acos((float)(endPoint.x - startPoint.x)/distance);
	 	else degree = -acos((float)(endPoint.x - startPoint.x)/distance);
	 	//std::cout << degree << " " << degree*180/3.14 << std::endl;
	} //else std::cout << "Fail" << std::endl;
	rect.move(startPoint);
	rect.scale(distance, 1);
	rect.rotate(degree);
	rect.setColor({10, 10,10});
	float moveY = (float)(distance/2.0f)*sin(degree);
	float moveX = (float)(distance/2.0f) - cos(degree)*distance/2.0f;

	assert(moveX == moveX);
	assert(degree == degree);
	assert(moveY == moveY);
	rect.move(Vector2i{(int)(rect.getPosition().x - moveX), (int)(rect.getPosition().y + moveY)});
	//std::cout << degree*180/3.14 << " " << distance << " " << moveY<< " "<< moveX <<std::endl;
	//std::cout << std::endl;

}

void Line::setColor(const Color &color) {
	rect.setColor(color);
}


Line::~Line() {}

const std::string Line::getNameShader() const {
	return "default";
}

void Line::draw(Shader &shader) {
	rect.draw(shader);
}