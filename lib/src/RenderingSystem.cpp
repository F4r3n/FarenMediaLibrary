#include "RenderingSystem.h"
#include "CTransform.h"
#include "ResourcesManager.h"
#include <iostream>
using namespace fms;
#include "Renderer.h"
#include "Shader.h"
#include "CMaterial.h"
RenderingSystem::RenderingSystem(int width, int height)
    : width(width)
    , height(height) {
    addComponent<fmc::CMesh>();
    addComponent<fmc::CTransform>();
    addComponent<fmc::CMaterial>();
}

RenderingSystem::~RenderingSystem() {
}

void RenderingSystem::init() {
}

void RenderingSystem::addCamera(std::shared_ptr<Entity> camera) {
    cameras.push_back(camera);
}

void
RenderingSystem::view(glm::mat4& viewMatrix, const fm::Vector2f& position, const fm::Vector2f& size, float rotation) {
    viewMatrix = glm::translate(viewMatrix, glm::vec3(position.x, position.y, 0));

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, rotation, glm::vec3(0, 0, 1));
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
}

void RenderingSystem::update(float dt, std::shared_ptr<Entity> e) {
    for(auto camera : cameras) {
        std::shared_ptr<fmc::CTransform> ct = camera->get<fmc::CTransform>();
        std::shared_ptr<fmc::CCamera> cam = camera->get<fmc::CCamera>();

        glm::mat4 viewMatrix = glm::mat4();
        view(viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);

        std::shared_ptr<fmc::CMesh> cmesh = e->get<fmc::CMesh>();
        std::shared_ptr<fmc::CTransform> transform = e->get<fmc::CTransform>();
        std::shared_ptr<fmc::CMaterial> material = e->get<fmc::CMaterial>();
        std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::getShader(material->shaderName);
        shader->Use()->setMatrix("projection", cam->projection)->setMatrix("view", viewMatrix)->setFloat("BloomEffect",
                                                                                                         0);

        glm::mat4 model = glm::mat4();
        setModel(model, transform);
        shader->setMatrix("model", model)->setVector4f(
            "mainColor", glm::vec4(material->color.r, material->color.g, material->color.b, material->color.a));

        if(material->textureReady) {
            glActiveTexture(GL_TEXTURE0);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            material->getTexture().bind();
        }

        draw(cmesh);
    }
}

void RenderingSystem::over() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    fm::Renderer::getInstance().blur();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::shared_ptr<fm::Shader> s = fm::ResourcesManager::getShader("simple");
    s->Use()->setVector2f("screenSize", glm::vec2(width, height));

    fm::Renderer::getInstance().postProcess(true);
    fm::Renderer::getInstance().bindFrameBuffer();
}

void RenderingSystem::setModel(glm::mat4& model, std::shared_ptr<fmc::CTransform> transform) {
    model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
    model = glm::rotate(model, transform->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
    model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::draw(std::shared_ptr<fmc::CMesh>& cmesh) {

    glBindVertexArray(cmesh->VAO);
    if(!cmesh->listIndices.empty()) {
        glDrawElements(GL_TRIANGLES, cmesh->listIndices.size(), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, cmesh->vertices.size());
    }
    glBindVertexArray(0); // Unbind VAO
}