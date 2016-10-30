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
namespace fmc {
enum SHAPE { RECTANGLE, CIRCLE, LAST_SHAPE };
static const char* ShapeNames[] = { "Rectangle", "Circle" };
struct Vertex {
    fm::Vector2f position;
    fm::Vector2f uv;
};

class CMesh : public Component<CMesh> {

public:
    CMesh();
    void setShape(int shape);
    CMesh(SHAPE shape);

    void addVertex(const fm::Vector2f& position, const fm::Vector2f& uv = { 0, 0 });
    void create();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> listIndices;
    unsigned int size = 4;
    GLuint VAO, VBO, EBO;
    // fm::Color color = { 1, 1, 1, 1 };
    static const std::string name;
#ifdef GUI
    std::string names = "\0";

    int previous = 0;
    int current = 0;

    void display(bool* value) {
        if(ImGui::CollapsingHeader("Mesh", value)) {

            ImGui::PushItemWidth(120);
            ImGui::Combo("##Shape", &current, ShapeNames, SHAPE::LAST_SHAPE);
            ImGui::PopItemWidth();

            if(previous != current && current < LAST_SHAPE) {

                setShape(current);
                previous = current;
            }
        }
    }
#endif
private:
    bool init(SHAPE shape);
};
}