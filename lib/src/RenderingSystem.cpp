#include "RenderingSystem.h"
#include "CTransform.h"
#include "ResourcesManager.h"
using namespace fms;

RenderingSystem::RenderingSystem() {}

RenderingSystem::~RenderingSystem() {}

void RenderingSystem::update(float dt, std::shared_ptr<Entity> e) {
    for(auto camera : cameras) {
        
        std::shared_ptr<fmc::CTransform> ct = camera->get<fmc::CTransform>();
        std::shared_ptr<fmc::CCamera> cam = camera->get<fmc::CCamera>();
        glm::mat4 viewMatrix = glm::mat4();
        viewMatrix = glm::translate(viewMatrix, glm::vec3(ct->position.x, ct->position.y, 0));
    
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.5f * cam->viewPort.width, 0.5f * cam->viewPort.height, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, ct->rotation, glm::vec3(0, 0, 1));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.5f * cam->viewPort.width, -0.5f * cam->viewPort.height, 0.0f));
        
        //std::shared_ptr<fm::Shader> s = fm::ResourcesManager::getShader(e->get<Mesh>()->shaderName);
        //s->Use()->setMatrix("projection", projection)->setMatrix("view", viewMatrix)->setFloat("BloomEffect",0);
    }
}