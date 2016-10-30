#pragma once
#include <System.h>
#include <Entity.h>
#include "Components/CCamera.h"


#include <chrono>
#include "Shader.h"
#include "RFont.h"

namespace fmc {
    class CText;
    class CTransform;
    class CMesh;
}
namespace fms
{
class RenderingSystem : public System
{
public:
    RenderingSystem(int width, int height);
    void addCamera(Entity* camera);
  
    void update(float dt, EntityManager& em, EventManager &event);
    void over();
    void init(EntityManager& em, EventManager &event);
    void pre_update(EntityManager& em);
    
    void draw(const fmc::CMesh* cmesh);
    void drawText(int posX, int posY, RFont* font, const fmc::CText* ctext);
    
    void view(glm::mat4& matrixView, const fm::Vector2f& position, const fm::Vector2f& size, float rotation);
    void setModel(glm::mat4& model, fmc::CTransform* transform, const fm::Vector2f &worldPos);
    ~RenderingSystem();

private:
    std::vector<Entity*> cameras;
    int width;
    int height;
    
    double timer = 0;
    size_t frame = 1;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    
    std::shared_ptr<fm::Shader> finalShader;
    std::shared_ptr<fm::Shader> lightShader;
};
}