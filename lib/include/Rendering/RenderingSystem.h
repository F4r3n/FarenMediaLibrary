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
#include "Rendering/MaterialValue.h"

namespace fmc {
class CText;
class CTransform;
class CMesh;
}

struct TextDef {
    GLuint VBO;
    fm::math::mat projection;
};

namespace fm
{
    class UniformBuffer;
}

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
    void setModel(fm::math::mat& model, fmc::CTransform* transform, const fm::math::Vector3f &worldPos);
    void initStandardShapes();
    ~RenderingSystem();
    void initUniformBufferCamera(fmc::CCamera* camera);
    void updateUniformBufferCamera(fmc::CCamera* camera);
    void draw(const fmc::CMesh* cmesh);
    void drawText(int posX, int posY, RFont* font, fmc::CText* ctext);
    void computeLighting(std::shared_ptr<fm::RenderTexture> lightRenderTexture, fmc::CCamera* cam, bool hasLight);
    void fillQueue(EntityManager& em);
    void draw(fmc::CCamera *cam);
    void
    setView(fm::math::mat& viewMatrix, const fm::math::Vector3f& position, const fm::math::Vector2f& size, const fm::math::Vector3f& rotation);

    Entity* camera = nullptr;
    int width;
    int height;

    fm::Shader *finalShader;
    fm::Shader *lightShader;

    TextDef textdef;

    const GLuint bindingPointIndex = 2;
    GLuint generatedBlockBinding;
    fm::RenderQueue queue;

    bool blendingMode = false;
    bool computeLightinh = false;
    int queuePreviousValue = 0;

    std::shared_ptr<fm::RenderTexture> lightRenderTexture;
    std::shared_ptr<fm::RenderTexture> intermediate;

    fm::Graphics graphics;
    
    fm::Bounds bounds;
    std::unique_ptr<fm::UniformBuffer> uboLight;
int lightNumber = 0;
//    fm::Model *quad;
    
};
}
