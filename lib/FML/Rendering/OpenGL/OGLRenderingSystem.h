#pragma once
#include "Rendering/RenderQueueEvents.hpp"
#include "Resource/RFont.h"
#include "Core/Bounds.h"
#include "Rendering/Graphics.hpp"
#include "Core/Math/Matrix.h"
#include "Rendering/GraphicsAPI.h"
#include "Rendering/OpenGL/OGLMaterial.hpp"
#include <memory>
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
#include "Rendering/GPUData.hpp"
#include "Rendering/ShaderKind.hpp"
#include "Rendering/RenderQueue.h"
#include "OGLTextureCache.hpp"
#include "OGLMaterialCache.hpp"
#include "OGLCameraCache.hpp"
#include "OGLShaderCache.hpp"
#include "Core/System/System.hpp"
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
	class RenderTexture;
	struct Transform;
	class Material;
	class OGLModel;
	class OGLMaterial;
	class OGLCamera;
	class SubShader;
	class Window;
}

struct TextDef
{
	GLuint VBO;
	fm::math::mat projection;
};


namespace fms
{


	class OGLRenderingSystem : public System<OGLRenderingSystem>
	{
	public:
		OGLRenderingSystem(std::shared_ptr<fm::Window> inWindow);

		virtual void update(float dt, entt::registry& registry, EventManager&) override;
		virtual void init(EventManager& event) override;

		virtual void Start(entt::registry& registry) override;
		virtual void Stop(entt::registry& registry) override;

		void Draw(fmc::CCamera* cam, fmc::CTransform& tranform, entt::registry& registry);
	private:
		void _InitStandardShapes();
		fm::RenderQueue _FillQueue(fmc::CCamera* cam, entt::registry& registry);
		void _Draw(fmc::CCamera* cam);
		void _DrawText(fmc::CCamera* cam, const fm::Transform& inTransform, fmc::CText* ctext, fm::Material* inMaterial);
		void _DrawMesh(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
			std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties);
		void _DrawMeshInstaned(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
			std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialValues, uint32_t inNumber, uint32_t inBaseInstance);

		void _ExecuteCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera, uint32_t& instance);
		bool _HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera);
		void _PrepareShader(fmc::CCamera* cam, const fm::Transform& inTransform,
			std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties);
		virtual ~OGLRenderingSystem();

	private:
		std::unique_ptr<fm::OGLShader> _finalShader;

		fm::Graphics _graphics;

		//TextDef _textdef;
		GRAPHIC_API _api = GRAPHIC_API::OPENGL;
		std::unordered_map<uint32_t, std::unique_ptr<fm::OGLModel>>		_models;
		std::unique_ptr<fm::OGLTextureCache>							_texturesCache = nullptr;
		std::unique_ptr<fm::OGLMaterialCache>							_materialsCache = nullptr;
		std::unique_ptr<fm::OGLCameraCache>								_camerasCache = nullptr;

		//ShaderID + SHADER_KIND
		std::unique_ptr<fm::OGLShaderCache>								_shaderCache;

		std::shared_ptr<fm::OGLMaterial>								_currentMaterial = nullptr;

		std::shared_ptr<fm::OGLCamera>									_currentCamera = nullptr;
		fm::OGLUniformbuffer											_ssbo;
		std::shared_ptr<fm::Window>										_window = nullptr;
		bool															_running = false;
	};
}
