#pragma once
#include <System.h>
#include <Entity.h>
#include "Components/CCamera.h"

#include "Rendering/RenderQueue.h"
#include <chrono>
#include "Rendering/Shader.h"
#include "Resource/RFont.h"
#include "Core/Bounds.h"
#include "Rendering/Graphics.hpp"
#include "Rendering/Model.hpp"
#include <string>
namespace fmc {
class CText;
class CTransform;
class CMesh;
}

struct TextDef {
    GLuint VAO;
    GLuint VBO;
    fm::math::mat projection;
};

namespace fms {
class RenderingSystem : public System<RenderingSystem> {
public:
    RenderingSystem(int width, int height);
    void setCamera(Entity* camera);

    void update(float dt, EntityManager& em, EventManager& event);
    void over();
    void init(EntityManager& em, EventManager& event);
    void pre_update(EntityManager& em);

private:
    fm::Bounds bounds;
    void setModel(fm::math::mat& model, fmc::CTransform* transform, const fm::math::Vector2f& worldPos);

    ~RenderingSystem();
    void initUniformBufferCamera(fmc::CCamera* camera);
    void updateUniformBufferCamera(fmc::CCamera* camera);
    void draw(const fmc::CMesh* cmesh);
    void drawText(int posX, int posY, RFont* font, const fmc::CText* ctext);
    void computeLighting(std::shared_ptr<fm::RenderTexture> lightRenderTexture, fmc::CCamera* cam, bool hasLight);
    void fillQueue(EntityManager& em);
    void draw(fmc::CCamera *cam);
    void
    view(fm::math::mat& viewMatrix, const fm::math::Vector2f& position, const fm::math::Vector2f& size, float rotation);

    Entity* camera;
    int width;
    int height;

    std::shared_ptr<fm::Shader> finalShader;
    std::shared_ptr<fm::Shader> lightShader;

    TextDef textdef;

    const GLuint bindingPointIndex = 2;
    GLuint generatedBlockBinding;
    fm::RenderQueue queue;

    bool blendingMode = false;
    bool computeLightinh = false;
    int queuePreviousValue = 0;

    std::shared_ptr<fm::RenderTexture> lightRenderTexture;
    fm::Graphics graphics;
    
    //fm::rendering::MeshContainer *quad;
    //fm::rendering::MeshContainer *circle;
    fm::Model *quad;
    fm::Model *circle;
    
    std::unordered_map<std::string, int> meshesID;
};
}