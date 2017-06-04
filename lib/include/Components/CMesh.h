#pragma once
#include <string>
#include <vector>
#include "Core/Math/Vector2.h"
#include <GL/glew.h>
#include <Component.h>
#include "Core/Color.h"
#include "Core/Bounds.h"

#include "Serializer.h"
namespace fmc {
enum SHAPE { RECTANGLE, CIRCLE, LAST_SHAPE };
struct Vertex {
    fm::math::Vector2f position;
    fm::math::Vector2f uv;
};

class CMesh : public Component<CMesh>, public Serializer {

public:
    CMesh();
    void setShape(int shape);
    ~CMesh();

    void destroy();

    unsigned int size = 4;
    GLuint VAO, VBO, EBO;
    static const std::string name;
    int currentShape = -1;
    
    void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) override {}
    void parse(rapidjson::Value &value) override {}
    void create();
    inline bool isCreated() {
        return created;
    }
    void update();
    
    
    //Manage indices and vertex
    void addVertex(const Vertex &vertex);
    void addVertexPositionUV(float px, float py, float uvx, float uvy);

    void addVertexPositionUVVectors(const fm::math::Vector2f& position, const fm::math::Vector2f& uv = { 0, 0 });
    void addIndex(unsigned int index);
    void clean();
    void removeVertices(unsigned int start, unsigned int end);
    void removeIndices(unsigned int start, unsigned int end);
    void removeVertex(unsigned int index);
    void removeIndice(unsigned int index);
    void computeBoundingSize();
    fm::Bounds bounds;

private:
    bool init(SHAPE shape);
    void createAndRegister();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> listIndices;
    bool created = false;
};
}