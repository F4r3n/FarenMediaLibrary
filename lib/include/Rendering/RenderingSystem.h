#pragma once
#include <System.h>
#include <Entity.h>
#include "Components/CCamera.h"

#include "Rendering/RenderQueue.h"
#include <chrono>
#include "Rendering/Shader.h"
#include "Resource/RFont.h"

namespace fmc {
class CText;
class CTransform;
class CMesh;
}

struct TextDef {
    GLuint VAO;
    GLuint VBO;
    glm::mat4 projection;
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
    void setModel(glm::mat4& model, fmc::CTransform* transform, const fm::math::Vector2f& worldPos);
    ~RenderingSystem();
    void initUniformBufferCamera(fmc::CCamera* camera);
    void updateUniformBufferCamera(fmc::CCamera* camera);
    void draw(const fmc::CMesh* cmesh);
    void drawText(int posX, int posY, RFont* font, const fmc::CText* ctext);

    void view(glm::mat4& matrixView, const fm::math::Vector2f& position, const fm::math::Vector2f& size, float rotation);

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
};
}