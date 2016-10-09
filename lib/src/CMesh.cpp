#include "CMesh.h"
#include <iostream>
#include "ResourcesManager.h"
#include <memory>
using namespace fmc;

CMesh::CMesh(SHAPE shape) {
    if(init(shape))
        create();
}

bool CMesh::init(SHAPE shape) {
    fm::MeshData* m = fm::ResourcesManager::get().getMeshData((int)shape);
    if(m) {
        VAO = m->VAO;
        VBO = m->VBO;
        EBO = m->EBO;
        size = m->size;
        return false;
    }
    if(shape == RECTANGLE) {

        addVertex({ 0.0, 1.0 }, { 0.0, 1.0 });
        addVertex({ 0.0, 0.0 }, { 1.0, 0.0 });
        addVertex({ 1.0, 0.0 }, { 0.0, 0.0 });
        addVertex({ 1.0, 1.0 }, { 1.0, 1.0 });
        listIndices = { 0, 1, 2, 0, 2, 3 };

    } else if(shape == CIRCLE) {
        unsigned int numberVertices = 100;
        float intervall = 2 * glm::pi<float>() / numberVertices;

        addVertex({ 0, 0 }, { 0.5, 0.5 });

        for(float teta = 0; teta < 2 * glm::pi<float>(); teta += intervall) {
            fm::Vector2f uv(0.5 + glm::cos(teta) / 2, 0.5 + glm::sin(teta) / 2);
            addVertex({ glm::cos(teta) / 2, glm::sin(teta) / 2 }, uv);
        }

        int j = 0;
        for(int i = 1; i < numberVertices; ++i) {
            j = i;
            listIndices.push_back(j++);
            listIndices.push_back(j);
            listIndices.push_back(0);
        }
        listIndices.push_back(j);
        listIndices.push_back(1);
        listIndices.push_back(0);
    }
    return true;
}

void CMesh::setShape(int shape) {
    if(init((SHAPE)shape))
        create();
}

void CMesh::addVertex(const fm::Vector2f& position, const fm::Vector2f& uv) {
    Vertex v{ { position.x, position.y }, { uv.x, uv.y } };
    vertices.push_back(v);
}

CMesh::CMesh() {

    /*addVertex({ -1.0, 1.0 }, { 0.0, 1.0 });
    addVertex({ -1.0, -1.0 }, { 1.0, 0.0 });
    addVertex({ 1.0, -1.0 }, { 0.0, 0.0 });
    addVertex({ 1.0, 1.0 }, { 1.0, 1.0 });
    // std::vector<unsigned int> v = { 0, 1, 2, 0, 2, 3 };
    listIndices = { 0, 1, 2, 0, 2, 3 };
    create();*/
}

void CMesh::create() {
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

    fm::ResourcesManager::get().registerMesh(
    new fm::MeshData(VAO, VBO, EBO, listIndices.size()));
    glBindVertexArray(0); // Unbind VAO
}