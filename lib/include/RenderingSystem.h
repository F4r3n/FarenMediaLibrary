#pragma once
#include <System.h>
#include <Entity.h>
#include "CCamera.h"
#include "CMesh.h"
#include "CTransform.h"
namespace fms
{
class RenderingSystem : public System
{
public:
    RenderingSystem(int width, int height);
    void addCamera(std::shared_ptr<Entity> camera);
  
    void update(float dt, std::shared_ptr<Entity> e);
    void over();
    void init();
  
    void draw(std::shared_ptr<fmc::CMesh>& cmesh);
    void view(glm::mat4& matrixView, const fm::Vector2f& position, const fm::Vector2f& size, float rotation);
    void setModel(glm::mat4& model, std::shared_ptr<fmc::CTransform> transform);
    ~RenderingSystem();

private:
    std::vector<std::shared_ptr<Entity> > cameras;
    int width;
    int height;
};
}