#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
#include <GL/glew.h>
#include <Component.h>
#include "Color.h"
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
    CMesh()
    {
        shaderName = "default";

        addVertex({ 0.0, 1.0 }, { 0.0, 1.0 });
        addVertex({ 0.0, 0.0 }, { 1.0, 0.0 });
        addVertex({ 1.0, 0.0 }, { 0.0, 0.0 });
        addVertex({ 1.0, 1.0 }, { 1.0, 1.0 });
        // std::vector<unsigned int> v = { 0, 1, 2, 0, 2, 3 };
        listIndices = { 0, 1, 2, 0, 2, 3 };
    }
    CMesh(SHAPE shape)
    {
        if(shape == RECTANGLE) {
            shaderName = "default";

            addVertex({ 0.0, 1.0 }, { 0.0, 1.0 });
            addVertex({ 0.0, 0.0 }, { 1.0, 0.0 });
            addVertex({ 1.0, 0.0 }, { 0.0, 0.0 });
            addVertex({ 1.0, 1.0 }, { 1.0, 1.0 });
            // std::vector<unsigned int> v = { 0, 1, 2, 0, 2, 3 };
            listIndices = { 0, 1, 2, 0, 2, 3 };
        }
    }

    void addVertex(const fm::Vector2f& position, const fm::Vector2f& uv = { 0, 0 })
    {
        Vertex v{ { position.x, position.y }, { uv.x, uv.y } };
        vertices.push_back(v);
    }

    // void construct()
    std::vector<Vertex> vertices;
    std::string shaderName = "default";
    std::vector<unsigned int> listIndices;

    GLuint VAO, VBO, EBO;
    fm::Color color;
};
}