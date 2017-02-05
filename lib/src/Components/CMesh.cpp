#include "Components/CMesh.h"
#include <iostream>
#include "ResourcesManager.h"
#include <memory>
using namespace fmc;
const std::string CMesh::name = "Mesh";


CMesh::~CMesh() {
    //destroy();
}

void CMesh::update() {
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    
    if(!listIndices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * listIndices.size(), listIndices.data(), GL_STATIC_DRAW);
    }
    
}

//Create and fill buffer, or update if already created
void CMesh::create() {

    if(created)  {
        update();
        return;
    }
    
    assert(!vertices.empty());
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    if(!listIndices.empty())
        glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    if(!listIndices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * listIndices.size(), listIndices.data(), GL_STATIC_DRAW);
    }

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    created = true;
    size = listIndices.size();
}

void CMesh::addIndex(unsigned int index) {
    listIndices.push_back(index);
}

// Clean indices and vertex
void CMesh::clean() {
    listIndices.clear();
    vertices.clear();
}
void CMesh::removeVertices(unsigned int start, unsigned int end) {
    vertices.erase(vertices.begin() + start, vertices.end() + end);
}
void CMesh::removeIndices(unsigned int start, unsigned int end) {
    listIndices.erase(listIndices.begin() + start, listIndices.end() + end);
}

void CMesh::removeVertex(unsigned int index) {
    vertices[index] = vertices.back();
    vertices.pop_back();
}
void CMesh::removeIndice(unsigned int index) {
    listIndices[index] = listIndices.back();
    listIndices.pop_back();
}

bool CMesh::init(SHAPE shape) {
    if(currentShape == (int)shape) return false;
    fm::MeshData* m = fm::ResourcesManager::get().getMeshData((size_t)shape);
    
    if(m) {
        VAO = m->VAO;
        VBO = m->VBO;
        EBO = m->EBO;
        size = m->size;
        currentShape = (int)shape;
        return false;
    } 
    if(shape == RECTANGLE) {

        addVertexPositionUVVectors({ 0, 1 }, { 0.0, 1.0 });
        addVertexPositionUVVectors({ 0, 0 }, { 0.0, 0.0 });
        addVertexPositionUVVectors({ 1, 0 }, { 1.0, 0.0 });
        addVertexPositionUVVectors({ 1, 1 }, { 1.0, 1.0 });
        listIndices = { 0, 1, 2, 0, 2, 3 };

    } else if(shape == CIRCLE) {
        unsigned int numberVertices = 100;
        float intervall = 2 * glm::pi<float>() / numberVertices;

        addVertexPositionUVVectors({ 0.5, 0.5 }, { 0.5, 0.5 });

        for(float teta = 0; teta < 2 * glm::pi<float>(); teta += intervall) {
            fm::Vector2f uv(0.5 + glm::cos(teta) / 2, 0.5 + glm::sin(teta) / 2);
            addVertexPositionUVVectors(uv, uv);
        }

        int j = 0;
        for(unsigned int i = 1; i < numberVertices; ++i) {
            j = i;
            listIndices.push_back(j++);
            listIndices.push_back(j);
            listIndices.push_back(0);
        }
        listIndices.push_back(j);
        listIndices.push_back(1);
        listIndices.push_back(0);
    }
    currentShape = (int)shape;
    return true;
}

void CMesh::addVertex(const Vertex &vertex) {
        vertices.push_back(vertex);
}


void CMesh::setShape(int shape) {
    if(init((SHAPE)shape))
        createAndRegister();
}

void CMesh::addVertexPositionUVVectors(const fm::Vector2f& position, const fm::Vector2f& uv) {
    Vertex v{ { position.x, position.y }, { uv.x, uv.y } };
    vertices.push_back(v);
}

void CMesh::addVertexPositionUV(float px, float py, float uvx, float uvy) {
        Vertex v{ { px, py }, { uvx, uvy } };
        vertices.push_back(v);
}


CMesh::CMesh() {

}

void CMesh::destroy() {
    vertices.clear();
    if(VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    created = false;
}

void CMesh::createAndRegister() {
    create();
    fm::ResourcesManager::get().registerMesh(new fm::MeshData(VAO, VBO, EBO, listIndices.size()));
    glBindVertexArray(0); // Unbind VAO
}