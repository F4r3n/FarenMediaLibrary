#pragma once
#include <System.h>
#include <Entity.h>
#include "CCamera.h"
namespace fms
{
class RenderingSystem : public System
{
public:
    RenderingSystem();

    void update(float dt, std::shared_ptr<Entity> e);
    ~RenderingSystem();
    private:
    std::vector<std::shared_ptr<Entity>> cameras;
};
}