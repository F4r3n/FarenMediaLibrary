#pragma once
#include <System.h>
#include <Entity.h>
#include "Components/CCamera.h"

#include "Rendering/RenderQueue.h"
#include "Rendering/Shader.h"
#include "Resource/RFont.h"
#include "Core/Bounds.h"
#include "Rendering/Graphics.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/MaterialValue.h"

namespace fmc 
{
	class CText;
	class CTransform;
	class CMesh;
}

namespace fm
{
	class UniformBuffer;
	class Model;
}

struct TextDef 
{
    GLuint VBO;
    fm::math::mat projection;
};


namespace fms 
{

class RenderingSystem : public System<RenderingSystem> 
{
public:
    RenderingSystem(int width, int height);
    void InitCamera(Entity* camera);

    virtual void update(float dt, EntityManager& em, EventManager& event);
    virtual void over();
    virtual void init(EntityManager& em, EventManager& event);
    virtual void pre_update(EntityManager& em);

	virtual void Start() {}
	virtual void Stop() {}
 private:
    void _SetModelPosition(fm::math::mat& model, const fm::Transform& transform, const fm::math::Vector3f &worldPos, bool isOrthographic);
    void _InitStandardShapes();
    void _InitUniformBufferCamera(fmc::CCamera* camera);
    void _UpdateUniformBufferCamera(fmc::CCamera* camera);
    void _DrawText(float posX, float posY, RFont* font, fmc::CText* ctext);
    void _ComputeLighting(std::shared_ptr<fm::RenderTexture> lightRenderTexture, fmc::CCamera* cam, bool hasLight);
    void _FillQueue(fmc::CCamera* cam, EntityManager& em);
    void _DrawMesh(fmc::CCamera *cam);
	void _DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, fm::Model *inModel, fm::Material* inMaterial, fm::MaterialProperties *inMaterialProperties = nullptr);

    void _SetView(fm::math::mat& viewMatrix, const fm::math::Vector3f& position, const fm::math::Vector2f& size, const fm::math::Vector3f& rotation, bool isOrthographic);
	void _ExecuteCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);
	bool _HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);

	~RenderingSystem();

private:
	fm::Shader *_finalShader;
	fm::Shader *_lightShader;
	int _width;
	int _height;
	int _lightNumber = 0;

	fm::Graphics _graphics;

	TextDef _textdef;    
};
}
