#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include <GL/glew.h>
#include <Component.h>
#include "Color.h"
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace fmc
{
enum SHAPE { RECTANGLE, CIRCLE };
class CMesh : public Component<CMesh>
{
    struct Vertex
    {
        fm::Vector2f position;
        fm::Vector2f uv;
    };

public:
    CMesh();

    CMesh(SHAPE shape);

    void addVertex(const fm::Vector2f& position, const fm::Vector2f& uv = { 0, 0 });
    void create();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> listIndices;

    GLuint VAO, VBO, EBO;
    fm::Color color = { 1, 1, 1, 1 };
};
}