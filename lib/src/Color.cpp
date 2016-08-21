
#include "Color.h"

using namespace fm;
Color::Color(float r, float g, float b): r(r), g(g), b(b)
{
}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
{
}

float Color::mean() {
	return (r + g + b) / 255;
}

Color::~Color()
{
}
