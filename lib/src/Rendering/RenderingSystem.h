#pragma once
#include <System.h>
#include <Entity.h>

#include "Rendering/RenderQueueEvents.hpp"
#include "Resource/RFont.h"
#include "Core/Bounds.h"
#include "Rendering/Graphics.hpp"
#include "Core/Math/Matrix.h"
#include "GraphicsAPI.h"
#include "Rendering/OpenGL/OGLMaterial.hpp"
#include <memory>
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
#include "Rendering/GPUData.hpp"
#include "ShaderKind.hpp"
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
	class OGLModel;
	class OGLMaterial;
	class OGLCamera;
	class SubShader;
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
	void _DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, std::shared_ptr<fm::Model> inModel,
		std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties *inMaterialProperties);
	void _DrawMeshInstaned(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
		std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties* inMaterialProperties, uint32_t inNumber, uint32_t inBaseInstance);

	void _ExecuteCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera, uint32_t &instance);
	bool _HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);
	void _PrepareShader(fmc::CCamera* cam, const fm::Transform& inTransform,
		std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties* inMaterialProperties);
	virtual ~RenderingSystem();

	std::shared_ptr<fm::OGLShader> _FindOrCreateShader(const fm::SubShader& inSubShader);
private:
	std::unique_ptr<fm::OGLShader> _finalShader;
	//std::unique_ptr<fm::OGLShader> _lightShader;
	size_t _width;
	size_t _height;
	//int _lightNumber = 0;

	fm::Graphics _graphics;

	//TextDef _textdef;
	GRAPHIC_API _api = GRAPHIC_API::OPENGL;
	std::unordered_map<uint32_t, std::unique_ptr<fm::OGLModel>>		_models;
	std::unordered_map<uint32_t, std::unique_ptr<fm::OGLMaterial>>	_materials;

	//ShaderID + SHADER_KIND
	std::unordered_map<fm::ShaderID, std::shared_ptr<fm::OGLShader>> _shaders;

	fm::OGLMaterial*												_currentMaterial = nullptr;
	std::unordered_map<uint32_t, std::unique_ptr<fm::OGLCamera>>	_cameras;
	
	fm::OGLCamera*													_currentCamera = nullptr;
	fm::OGLUniformbuffer											_ssbo;
};
}
