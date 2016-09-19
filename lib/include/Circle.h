#pragma once

#include <vector>
#include <glm/gtc/constants.hpp>
#include "Shape.h"
namespace fm
{
    
class Circle : public Shape
{
public:
    Circle();
    ~Circle();
    Circle(const Color& color, int radius, int number);

private:
    void initBuffer(std::vector<unsigned int>& indices);
    void initVertices(float r, float g, float b);

    int numberVertices = 10;
    int radius = 10;
};
}
