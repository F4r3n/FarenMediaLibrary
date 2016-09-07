#pragma once

#include "Vector2.h"
#include <cmath>
#include <iostream>
namespace fm {
class Segment {
	public:
	 	Segment(Vector2f start, Vector2f end);
	 	const Vector2f getStart() const{return start;}
	 	const Vector2f getEnd() const{return end;}
	 	~Segment();
	 	float length();
	 	Vector2f half();

	 	void print();
	private:
		Vector2f start;
		Vector2f end;
};
}