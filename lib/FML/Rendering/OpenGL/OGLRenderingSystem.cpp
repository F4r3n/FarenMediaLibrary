#include "OGLRenderingSystem.h"
#include "Components/CTransform.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Shader.h"
#include "Components/CMaterial.h"
#include "Components/CPointLight.h"
#include "Components/CText.h"
#include "Components/CMesh.h"
#include "Components/CDirectionalLight.h"

#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include "Profiler/ProfilerMacro.h"

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

#include "Rendering/RenderingEvent.h"
#include "Rendering/StandardShapes.h"
#include "Window.h"
#include "Core/Debug.h"
#include <cassert>
#include "OGLUniformbuffer.hpp"
#include "OGLShader.hpp"
#include "OGLModel.hpp"
#include "OGLMaterial.hpp"
#include "OGLCamera.hpp"
#include "OGLRenderer.h"
#include "OGLTexture.hpp"

#include "Rendering/material.hpp"

#include "Rendering/commandBuffer.hpp"
#include "Components/CCamera.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/Model.hpp"
#include "Engine.h"

#include "Rendering/RenderTexture.hpp"
#include "OGLFrameBuffer.hpp"
#include "Rendering/Image.h"
#include "Rendering/Texture.h"
#include "Rendering/meshloader.hpp"
#include <entt/entt.hpp>
#include "Components/CIdentity.h"
#define LOG_DEBUG 	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);


#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

const int NUMBER_POINTLIGHT_MAX = 8;


struct PointLight
{
	fm::math::vec4 position;
	fm::math::vec4 color;
	fm::math::vec4 custom;
};

using namespace fms;

OGLRenderingSystem::OGLRenderingSystem(std::shared_ptr<fm::Window> inWindow)
{
	_window = inWindow;
	_type = SYSTEM_MODE::ALWAYS;

}
void OGLRenderingSystem::init(EventManager&)
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		assert(false);
	}

	glViewport(0, 0, _window->GetSize().x, _window->GetSize().y);

	fm::Debug::log("INIT Standard Shapes");

	_InitStandardShapes();


	_ssbo.Generate(sizeof(fms::GPUObjectData) * 10000, 2, GL_SHADER_STORAGE_BUFFER);
	auto finalShader = fm::ResourcesManager::get().getResource<fm::Shader>(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "simple.shader"));

	_finalShader = std::make_unique<fm::OGLShader>(finalShader->GetSubShader(fm::SHADER_KIND::PLAIN).value());
	_finalShader->compile();

	_texturesCache = std::make_unique<fm::OGLTextureCache>();
	_camerasCache = std::make_unique<fm::OGLCameraCache>();
	_materialsCache = std::make_unique<fm::OGLMaterialCache>();
	_shaderCache = std::make_unique<fm::OGLShaderCache>();

	if (fm::Image blankImage(true); blankImage.create(fm::math::vec2i(1, 1)))
	{
		_texturesCache->_blankTexture = std::make_shared<fm::OGLTexture>();
		_texturesCache->_blankTexture->UploadImage(blankImage);
	}
}

OGLRenderingSystem::~OGLRenderingSystem()
{
}


void OGLRenderingSystem::_InitStandardShapes()
{
	std::shared_ptr<fm::Model> quad = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Quad"));
	std::shared_ptr<fm::Model> quadFS = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "QuadFS"));
	std::shared_ptr<fm::Model> circle = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Circle"));
	//std::shared_ptr<fm::Model> cube = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Cube"));

	fm::FilePath models(fm::ResourcesManager::GetFilePathResource(fm::LOCATION::INTERNAL_MODELS_LOCATION));
	fm::FilePath cubeModel = models.ToSub("cube.obj");
	std::shared_ptr<fm::Model> cube = fm::MeshLoader::Load(cubeModel, "Cube").value();
	fm::ResourcesManager::get().load<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Cube"), cube);

	quad->AddMesh(fm::StandardShapes::CreateQuad());
	circle->AddMesh(fm::StandardShapes::CreateCircle());
	quadFS->AddMesh(fm::StandardShapes::CreateQuadFullScreen());

	fm::ResourcesManager::get().load(quad);
	fm::ResourcesManager::get().load(quadFS);
	fm::ResourcesManager::get().load(circle);
	fm::ResourcesManager::get().load(cube);
	_models.emplace(quad->GetObjectID(), std::make_unique<fm::OGLModel>(quad));
	_models.emplace(cube->GetObjectID(), std::make_unique<fm::OGLModel>(cube));
	_models.emplace(quadFS->GetObjectID(), std::make_unique<fm::OGLModel>(quadFS));
	_models.emplace(circle->GetObjectID(), std::make_unique<fm::OGLModel>(circle));

	for (const auto& m : _models)
	{
		m.second->UploadData();
	}

	fm::OGLRenderer::getInstance().SetQuadScreen(_models.find(quadFS->GetObjectID())->second.get());
}

void OGLRenderingSystem::Draw(fmc::CCamera* cam, fmc::CTransform& transform, entt::registry& registry)
{
	uint32_t instance = 0;

	cam->UpdateViewMatrix(transform.GetTransform());
	cam->UpdateProjectionMatrix();

	std::shared_ptr<fm::OGLCamera> camera = _camerasCache->FindOrCreateCamera(cam);
	cam->ExecuteStartRendering();

	if (cam->IsAuto())
	{
		camera->BindTarget(_graphics);
		_graphics.SetViewPort(cam->GetViewport());
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);;

		camera->BindPostProcess();
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

		camera->BindInternal();
		_graphics.SetViewPort(cam->GetViewport());
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
		_graphics.Enable(fm::RENDERING_TYPE::DEPTH_TEST);

		if (_graphics.Enable(fm::RENDERING_TYPE::BLEND))
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		_graphics.Disable(fm::RENDERING_TYPE::BLEND);
		_ExecuteCommandBuffer(fm::RENDER_QUEUE_FIRST_STATE, cam, instance);
	}

	LOG_DEBUG;

	if (cam->IsAuto())
	{
		fm::RenderQueue queue = _FillQueue(cam, registry);
		for (const fm::RenderQueue::Batch& batch : queue.Iterate())
		{
			_DrawMeshInstaned(cam, batch.node.transform, batch.node.model.lock(), batch.node.material.lock(), nullptr, batch.number, batch.baseInstance);
		}
		_currentMaterial = nullptr;
		_currentCamera = nullptr;
	}
	else
	{
		_ExecuteCommandBuffer(fm::RENDER_QUEUE_BEFORE_RENDERING_FILL_QUEUE, cam, instance);
	}

	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

	//if (cam._isAuto)
	//{
	camera->BindPostProcess();
	_finalShader->Use();
	_finalShader->setValue("screenSize", fm::math::vec2((float)cam->GetViewport().w, (float)cam->GetViewport().h));
	_finalShader->setValue("viewPos", transform.GetWorldPosition());
	_finalShader->setValue("screenTexture", 0);

	fm::OGLRenderer::getInstance().postProcess(_graphics, *camera->GetInternal()->GetColorBufferTexture(0));


	if (cam->HasTarget())
	{
		fm::OGLRenderer::getInstance().blit(_graphics, *camera->GetPostProcess(), *camera->GetTarget(), fm::BUFFER_BIT::COLOR_BUFFER_BIT);
	}
	else
	{
		fm::OGLRenderer::getInstance().blit(_graphics, *camera->GetPostProcess(), fm::BUFFER_BIT::COLOR_BUFFER_BIT);
	}

	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

	_ExecuteCommandBuffer(fm::RENDER_QUEUE_AFTER_RENDERING, cam, instance);
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

	cam->ExecutePostRendering();

	_graphics.BindFrameBuffer(0);

	fmc::CameraCommandBuffer().swap(cam->GetCommandBuffer());
}


void OGLRenderingSystem::update(float, entt::registry& registry, EventManager&)
{
	uint32_t instance = 0;
	auto view = registry.view<fmc::CCamera, fmc::CTransform, fmc::CIdentity>();
	for (auto&& e : view)
	{
		LOG_DEBUG;
		fmc::CCamera& cam = registry.get<fmc::CCamera>(e);
		if ((!cam.IsAuto() && cam.GetCommandBuffer().empty()))
			continue;

		if (cam.IsAuto() && !_running)
			continue;
		const auto identity = registry.get<fmc::CIdentity>(e);
		if (!identity.IsActive())
			continue;

		fmc::CTransform& transform = registry.get<fmc::CTransform>(e);

		Draw(&cam, transform, registry);
	}
}


bool OGLRenderingSystem::_HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera)
{
	return !currentCamera->GetCommandBuffer()[inRenderQueue].empty();
}

void OGLRenderingSystem::Start(entt::registry& registry)
{
	_running = true;
}

void OGLRenderingSystem::Stop(entt::registry& registry)
{
	_running = false;
}


void OGLRenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE inQueue, fmc::CCamera* currentCamera, uint32_t& instance)
{
	LOG_DEBUG;
	_currentCamera = nullptr;
	_currentMaterial = nullptr;
	fmc::CameraCommandBuffer::iterator it = currentCamera->GetCommandBuffer().find(inQueue);
	auto oglCamera = _camerasCache->FindOrCreateCamera(currentCamera);
	if (it != currentCamera->GetCommandBuffer().end())
	{
		std::queue<fm::CommandBuffer>& cmdBuffers = it->second;

		while (!cmdBuffers.empty())
		{
			fm::CommandBuffer cmdBuffer = cmdBuffers.front();
			fm::Command&& cmd = cmdBuffer.Pop();
			if (cmd._command == fm::Command::COMMAND_KIND::BLIT)
			{
				fm::OGLShader* shader = _finalShader.get();
				if (auto m = cmd._material.lock(); m->GetSubShader().has_value())
				{

					shader = _shaderCache->FindOrCreate(m->GetSubShader().value()).get();
					shader->Use();
					for (auto const& [name, value] : m->GetUniforms())
					{
						shader->setValue(name, value);
					}
				}


				if (cmd._source.kind == fm::Command::TEXTURE_KIND::TEXTURE)
				{
					if (cmd._source.texture != nullptr)
					{
						_graphics.BindTexture2D(0, (int)cmd._source.texture->GetKind(), cmd._source.texture->getID());
					}
					else
					{
						std::vector<std::shared_ptr<fm::OGLTexture>> textures = oglCamera->GetInternal()->GetColorBuffer();
						fm::OGLRenderer::getInstance().SetSources(_graphics, oglCamera->GetInternal()->GetColorBuffer(), textures.size());
					}
				}


				if (cmd._destination.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
				{
					if (cmd._destination.renderTexture != nullptr)
					{
						fm::OGLRenderer::getInstance().blit(_graphics, *cmd._destination.renderTexture, shader);
					}
				}
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DRAW_MESH)
			{
				_DrawMeshInstaned(currentCamera, cmd._transform, cmd._model.lock(), cmd._material.lock(), &cmd._materialProperties, 1, instance);
				instance++;
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DRAW_INSTACED_MESH)
			{
				fm::RenderQueue queue;

				assert((cmd._materials.size() == cmd._models.size()) && (cmd._models.size() == cmd._transforms.size()));
				for (size_t i = 0; i < cmd._materials.size(); ++i)
				{
					fm::RenderNode node;
					node.material = cmd._materials[i];
					node.model = cmd._models[i];
					node.transform = cmd._transforms[i];
					node.state = fm::RENDER_QUEUE_OPAQUE;
					queue.AddElement(node);
				}
				queue.Sort();
				auto ssboData = queue.GetSSBOData();
				_ssbo.SetData(ssboData.data(), ssboData.size() * sizeof(fms::GPUObjectData));
				for (const fm::RenderQueue::Batch& batch : queue.Iterate())
				{
					_DrawMeshInstaned(currentCamera, batch.node.transform, batch.node.model.lock(), batch.node.material.lock(), nullptr, batch.number, batch.baseInstance);
				}

			}
			else if (cmd._command == fm::Command::COMMAND_KIND::CLEAR)
			{
				//if (cmd._source.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
				//	cmd._source.renderTexture->bind();
				//_graphics.Clear(cmd._bufferBit);
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::CLEAR_ALL)
			{
				oglCamera->BindTarget(_graphics);
				_graphics.SetViewPort(currentCamera->GetViewport());
				_graphics.Clear(cmd._bufferBit);

				oglCamera->BindPostProcess();
				_graphics.Clear(cmd._bufferBit);

				oglCamera->BindInternal();
				_graphics.SetViewPort(currentCamera->GetViewport());
				_graphics.Clear(cmd._bufferBit);
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::ENABLE)
			{
				_graphics.Enable(cmd._renderingType);
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DISABLE)
			{
				_graphics.Disable(cmd._renderingType);
			}
			cmdBuffers.pop();
		}
		currentCamera->GetCommandBuffer().erase(it);
	}

	LOG_DEBUG
}



void OGLRenderingSystem::_PrepareShader(fmc::CCamera* cam, const fm::Transform& inTransform,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties)
{
	if (inMaterial == nullptr)
		return;

	if (_currentCamera == nullptr || _currentCamera->GetID() != cam->GetObjectID())
	{
		_currentCamera = _camerasCache->FindOrCreateCamera(cam);
		fmc::Shader_data camData;
		camData.FM_P = cam->GetProjectionMatrix();
		camData.FM_V = cam->GetViewMatrix();
		camData.FM_PV = camData.FM_P * camData.FM_V;
		_currentCamera->SetBuffer((void*)&camData, sizeof(fmc::Shader_data));
	}

	fm::MaterialValues properties;
	if (inMaterialProperties != nullptr)
	{
		for (const auto& [name, property] : *inMaterialProperties)
		{
			properties.emplace(name, property);
		}
	}

	if (_currentMaterial == nullptr || _currentMaterial->GetID() != inMaterial->GetObjectID())
	{
		_currentMaterial = _materialsCache->FindOrCreate(inMaterial, *_shaderCache, *_texturesCache);

		if (_currentCamera != nullptr)
		{
			_currentCamera->BindBuffer();
		}
	}
	_currentMaterial->Bind(properties, *_texturesCache);
}

void OGLRenderingSystem::_DrawMeshInstaned(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties, uint32_t inNumber, uint32_t inBaseInstance)
{
	_PrepareShader(cam, inTransform, inMaterial, inMaterialProperties);

	if (inModel != nullptr)
	{
		auto it = _models.find(inModel->GetObjectID());
		if (it != _models.end())
		{
			it->second->DrawInstance(inNumber, inBaseInstance);
		}
	}
}

void OGLRenderingSystem::_DrawMesh(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties)
{

	LOG_DEBUG;
	_PrepareShader(cam, inTransform, inMaterial, inMaterialProperties);

	if (inModel != nullptr)
	{
		auto it = _models.find(inModel->GetObjectID());
		if (it != _models.end())
		{
			it->second->Draw();
		}
	}
}



void OGLRenderingSystem::_Draw(fmc::CCamera* cam)
{

	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
	_currentMaterial = nullptr;
	_currentCamera = nullptr;


}

fm::RenderQueue OGLRenderingSystem::_FillQueue(fmc::CCamera* cam, entt::registry& registry)
{
	fm::RenderQueue queue;
	const auto view = registry.view<fmc::CTransform, fmc::CIdentity>();
	for (auto&& e : view)
	{
		const auto identity = registry.get<fmc::CIdentity>(e);
		if (!identity.IsActive())
			continue;

		fmc::CTransform* transform = registry.try_get<fmc::CTransform>(e);
		fm::RenderNode node;
		node.state = fm::RENDER_QUEUE_OPAQUE;
		node.transform = transform->GetTransform();
		node.text = registry.try_get<fmc::CText>(e);


		fmc::CMesh* mesh = registry.try_get<fmc::CMesh>(e);
		fmc::CMaterial* material = registry.try_get<fmc::CMaterial>(e);
		bool add = false;
		if (mesh != nullptr && material != nullptr && mesh->GetModel() != nullptr)
		{
			node.model = mesh->GetModel();
			node.material = material->GetMainMaterial();
			add = true;
		}
		else if (node.text != nullptr && material != nullptr)
		{
			node.material = material->GetMainMaterial();
			add = true;
		}

		if (add)
		{
			queue.AddElement(node);
		}
	}


	queue.Sort();
	std::vector<fms::GPUObjectData> objectDatas = queue.GetSSBOData();
	if (!objectDatas.empty())
	{
		_ssbo.SetData(objectDatas.data(), objectDatas.size() * sizeof(fms::GPUObjectData));
	}
	return queue;
}


void OGLRenderingSystem::_DrawText(fmc::CCamera* cam,
	const fm::Transform& inTransform,
	fmc::CText* inText,
	fm::Material* inMaterial)
{

	if (inText->GetText().empty())
		return;

	assert(false);


}

