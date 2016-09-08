#include "Segment.h"

using namespace fm;
Segment::Segment(Vector2f start, Vector2f end):start(start), end(end) {}

Segment::~Segment() {
}

float Segment::length() {
	return sqrt((start.x - end.x)*(start.x - end.x) + (start.y - end.y)*(start.y - end.y));
}

Vector2f Segment::half() {
	return {(end.x + start.x)/2, (end.y + start.y)/2};
}

Vector2f Segment::direction() {
	return {(end.x - start.x), (end.y - start.y)};
}


void Segment::print() {
	std::cout << "[ " <<getStart().x << " , " << getStart().y;
	std::cout << " ] -> [ " << getEnd().x << " , " << getEnd().y << " ]" << std::endl;
}