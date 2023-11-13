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
#include "Event.h"
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
#include <EntityManager.h>

#include "Rendering/commandBuffer.hpp"
#include "Components/CCamera.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/Model.hpp"
#include "Engine.h"

#include "Rendering/RenderTexture.h"
#include "Rendering/Image.h"
#include "Rendering/Texture.h"
#include "Rendering/meshloader.hpp"
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
void OGLRenderingSystem::init(EntityManager& em, EventManager&)
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

	if (fm::Image blankImage(true); blankImage.create(fm::math::vec2i(1, 1)))
	{
		_textures._blankTexture = std::make_shared<fm::OGLTexture>();
		_textures._blankTexture->UploadImage(blankImage);
	}
}

OGLRenderingSystem::~OGLRenderingSystem()
{
	for (auto& material : _materials)
	{
		material.second->Destroy();
	}
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
	cube->AddMesh(fm::StandardShapes::CreateCube());

	fm::ResourcesManager::get().load(quad);
	fm::ResourcesManager::get().load(quadFS);
	fm::ResourcesManager::get().load(circle);
	fm::ResourcesManager::get().load(cube);
	_models.emplace(quad->GetID(), std::make_unique<fm::OGLModel>(quad));
	_models.emplace(cube->GetID(), std::make_unique<fm::OGLModel>(cube));
	_models.emplace(quadFS->GetID(), std::make_unique<fm::OGLModel>(quadFS));
	_models.emplace(circle->GetID(), std::make_unique<fm::OGLModel>(circle));

	for (const auto& m : _models)
	{
		m.second->UploadData();
	}

	fm::OGLRenderer::getInstance().SetQuadScreen(_models.find(quadFS->GetID())->second.get());
}



void OGLRenderingSystem::pre_update(EntityManager& em)
{
	for (auto&& e : em.iterate<fmc::CCamera, fmc::CTransform>(fm::IsEntityActive))
	{
		fmc::CCamera* cam = e.get<fmc::CCamera>();
		fmc::CTransform* ct = e.get<fmc::CTransform>();
		const fm::Transform tr = ct->GetTransform();
		fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

		if (!cam->Enabled)
			continue;

		if (!cam->IsInit())
		{
			cam->Init();
		}

		if (!cam->GetRendererConfig().isInit)
		{
			cam->InitRenderConfig(tr, sizeof(PointLight) * NUMBER_POINTLIGHT_MAX);
		}

		cam->UpdateRenderTexture();
		cam->UpdateRenderConfigBounds(tr);
		cam->UpdateViewMatrix(ct->GetTransform());
	}
}


void OGLRenderingSystem::update(float, EntityManager& em, EventManager&)
{
	uint32_t instance = 0;

	for (auto&& e : em.iterate<fmc::CCamera>(fm::IsEntityActive))
	{
		LOG_DEBUG;
		fmc::CCamera* cam = e.get<fmc::CCamera>();
		if (!cam->Enabled || (!cam->IsAuto() && cam->GetCommandBuffer().empty()))
			continue;

		if (cam->IsAuto() && !_running)
			continue;

		fmc::CTransform* transform = e.get<fmc::CTransform>();

		if (!cam->GetRenderTexture().isCreated())
		{
			fm::Debug::logError("No render texture created");
			return;
		}

		cam->ExecuteStartRendering();

		if (cam->HasTarget())
		{
			if (!cam->GetTarget()->isCreated())
			{
				cam->GetTarget()->create();
			}
		}
		LOG_DEBUG;

		if (cam->IsAuto())
		{
			if (cam->HasTarget())
			{
				cam->GetTarget()->bind();
				_graphics.SetViewPort(cam->GetViewport());
				_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
			}
			else
			{
				//Clear buffers
				_graphics.BindFrameBuffer(0);
				_graphics.SetViewPort(cam->GetViewport());
				_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
			}

			cam->GetRendererConfig().postProcessRenderTexture.bind();
			_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

			cam->GetRenderTexture().bind();
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
		//Prepare camera data
		cam->UpdateShaderData();

		int error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}

		if (cam->IsAuto())
		{
			fm::RenderQueue queue = _FillQueue(cam, em);
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

		//if (cam->_isAuto)
		//{
		cam->GetRendererConfig().postProcessRenderTexture.bind();
		_finalShader->Use();
		_finalShader->setValue("screenSize", fm::math::vec2((float)cam->GetViewport().w, (float)cam->GetViewport().h));
		_finalShader->setValue("viewPos", transform->GetWorldPosition());
		_finalShader->setValue("screenTexture", 0);

		fm::OGLRenderer::getInstance().postProcess(_graphics, *cam->GetRenderTexture().GetColorBufferTexture(0).get());


		if (cam->HasTarget())
		{
			fm::OGLRenderer::getInstance().blit(_graphics, cam->GetRendererConfig().postProcessRenderTexture, *(cam->GetTarget().get()), fm::BUFFER_BIT::COLOR_BUFFER_BIT);
		}
		else
		{
			fm::OGLRenderer::getInstance().blit(_graphics, cam->GetRendererConfig().postProcessRenderTexture, fm::BUFFER_BIT::COLOR_BUFFER_BIT);
		}
		//}
		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		_ExecuteCommandBuffer(fm::RENDER_QUEUE_AFTER_RENDERING, cam, instance);
		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		cam->ExecutePostRendering();

		_graphics.BindFrameBuffer(0);

		fmc::CameraCommandBuffer().swap(cam->GetCommandBuffer());
	}
}


bool OGLRenderingSystem::_HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera)
{
	return !currentCamera->GetCommandBuffer()[inRenderQueue].empty();
}

void OGLRenderingSystem::Start()
{
	_running = true;
}

void OGLRenderingSystem::Stop()
{
	_running = false;
}


void OGLRenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE inQueue, fmc::CCamera* currentCamera, uint32_t& instance)
{
	LOG_DEBUG;
	fmc::CameraCommandBuffer::iterator it = currentCamera->GetCommandBuffer().find(inQueue);

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

					shader = _FindOrCreateShader(m->GetSubShader().value()).get();
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
						std::vector<std::shared_ptr<fm::OGLTexture>> textures = currentCamera->GetRenderTexture().GetColorBuffer();
						fm::OGLRenderer::getInstance().SetSources(_graphics, currentCamera->GetRenderTexture().GetColorBuffer(), textures.size());
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
				if (cmd._source.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
					cmd._source.renderTexture->bind();
				_graphics.Clear(cmd._bufferBit);
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::CLEAR_ALL)
			{
				if (currentCamera->HasTarget())
				{
					currentCamera->GetTarget()->bind();
				}
				else
				{
					_graphics.BindFrameBuffer(0);
				}

				_graphics.SetViewPort(currentCamera->GetViewport());
				_graphics.Clear(cmd._bufferBit);
				currentCamera->GetRendererConfig().postProcessRenderTexture.bind();
				_graphics.Clear(cmd._bufferBit);

				currentCamera->GetRenderTexture().bind();
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
	_currentCamera = nullptr;
	_currentMaterial = nullptr;
	LOG_DEBUG
}

std::shared_ptr<fm::OGLShader> OGLRenderingSystem::_FindOrCreateShader(const fm::SubShader& inSubShader)
{
	fm::ShaderID ID = inSubShader.GetID();
	auto it = _shaders.find(ID);
	if (it != _shaders.end())
		return it->second;

	std::shared_ptr<fm::OGLShader> shader = std::make_shared<fm::OGLShader>(inSubShader);
	_shaders.emplace(ID, shader);
	shader->compile();
	return shader;
}


void OGLRenderingSystem::_PrepareShader(fmc::CCamera* cam, const fm::Transform& inTransform,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties)
{
	if (inMaterial == nullptr)
		return;

	if (_currentMaterial == nullptr || _currentMaterial->GetID() != inMaterial->GetID())
	{
		if (auto it = _materials.find(inMaterial->GetID()); it == _materials.end())
		{
			std::optional<fm::SubShader> sub = inMaterial->GetSubShader();
			if (!sub.has_value())
			{
				assert(false);
			}

			std::shared_ptr<fm::OGLShader> shader = _FindOrCreateShader(sub.value());

			fm::OGLMaterialCreateInfo info;
			info.material = inMaterial;
			info.shader = shader;
			std::unique_ptr<fm::OGLMaterial> mat = std::make_unique< fm::OGLMaterial>(info, _textures);
			_currentMaterial = _materials.emplace(inMaterial->GetID(), std::move(mat)).first->second.get();
		}
		else
		{
			_currentMaterial = it->second.get();
		}
	}

	if (_currentCamera == nullptr || _currentCamera->GetID() != cam->GetInstance())
	{
		if (auto it = _cameras.find(cam->GetInstance()); it == _cameras.end())
		{
			std::unique_ptr<fm::OGLCamera> mat = std::make_unique< fm::OGLCamera>(cam->GetInstance());
			_currentCamera = _cameras.emplace(cam->GetInstance(), std::move(mat)).first->second.get();
			_currentCamera->PrepareBuffer(sizeof(fmc::CCamera::shader_data));
		}
		else
		{
			_currentCamera = it->second.get();
		}
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

	_currentMaterial->Bind(properties, _textures);
}

void OGLRenderingSystem::_DrawMeshInstaned(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialValues* inMaterialProperties, uint32_t inNumber, uint32_t inBaseInstance)
{
	_PrepareShader(cam, inTransform, inMaterial, inMaterialProperties);

	if (inModel != nullptr)
	{
		auto it = _models.find(inModel->GetID());
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
		auto it = _models.find(inModel->GetID());
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

void OGLRenderingSystem::_ComputeLighting(std::shared_ptr<fm::RenderTexture> lightRenderTexture,
	fmc::CCamera* cam,
	bool hasLight)
{
	fm::OGLRenderer::getInstance().lightComputation(_graphics, *cam->GetRendererConfig().postProcessRenderTexture.GetColorBufferTexture(0), false);
}

fm::RenderQueue OGLRenderingSystem::_FillQueue(fmc::CCamera* cam, EntityManager& em)
{
	fm::RenderQueue queue;

	for (auto&& e : em.iterate<fmc::CTransform>(fm::IsEntityActive))
	{
		fmc::CTransform* transform = e.get<fmc::CTransform>();
		fm::RenderNode node;
		node.state = fm::RENDER_QUEUE_OPAQUE;
		node.transform = transform->GetTransform();
		node.text = e.get<fmc::CText>();


		fmc::CMesh* mesh = e.get<fmc::CMesh>();
		fmc::CMaterial* material = e.get<fmc::CMaterial>();
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

void OGLRenderingSystem::over()
{
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
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

