#pragma once
#include <System.h>
#include <Entity.h>

#include "Rendering/RenderQueueEvents.hpp"
#include "Resource/RFont.h"
#include "Core/Bounds.h"
#include "Rendering/Graphics.hpp"
#include "Core/Math/Matrix.h"
#include "GraphicsAPI.h"
namespace fmc 
{
	class CText;
	class CTransform;
	class CMesh;
	class CCamera;

}

namespace fm
{
	class UniformBuffer;
	class Model;
	class MaterialValue;
	class Shader;
	class MaterialProperties;
	class RenderTexture;
	struct Transform;
	class Material;
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
    RenderingSystem(size_t width, size_t height);
    void InitCamera(Entity* camera);

    virtual void update(float dt, EntityManager& em, EventManager& event);
    virtual void over();
    virtual void init(EntityManager& em, EventManager& event);
    virtual void pre_update(EntityManager& em);

	virtual void Start() {}
	virtual void Stop() {}
 private:
    void _InitStandardShapes();
    void _ComputeLighting(std::shared_ptr<fm::RenderTexture> lightRenderTexture, fmc::CCamera* cam, bool hasLight);
    void _FillQueue(fmc::CCamera* cam, EntityManager& em);
    void _Draw(fmc::CCamera *cam);
	void _DrawText(fmc::CCamera* cam, const fm::Transform& inTransform, fmc::CText* ctext, fm::Material* inMaterial);
	void _DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, fm::Model *inModel, fm::Material* inMaterial, fm::MaterialProperties *inMaterialProperties = nullptr);

	void _ExecuteCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);
	bool _HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);

	virtual ~RenderingSystem();

private:
	fm::Shader *_finalShader;
	fm::Shader *_lightShader;
	size_t _width;
	size_t _height;
	int _lightNumber = 0;

	fm::Graphics _graphics;

	TextDef _textdef;
	GRAPHIC_API _api = GRAPHIC_API::OPENGL;
};
}
