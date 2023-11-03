#include "Rendering/RenderingSystem.h"
#include "Components/CTransform.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/Renderer.h"
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
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
#include "Rendering/material.hpp"
#include <EntityManager.h>

#include "Rendering/commandBuffer.hpp"
#include "Components/CCamera.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/Model.hpp"
#include "Engine.h"
#include "Rendering/OpenGL/OGLShader.hpp"
#include "Rendering/OpenGL/OGLModel.hpp"
#include "Rendering/OpenGL/OGLMaterial.hpp"
#include "Rendering/OpenGL/OGLCamera.hpp"
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

namespace fms
{
RenderingSystem::RenderingSystem(size_t width, size_t height)
    : _width(width), _height(height)
{

	_type = SYSTEM_MODE::ALWAYS;

}
void RenderingSystem::init(EntityManager& em, EventManager&)
{
	fm::Debug::log("INIT Standard Shapes");

	_InitStandardShapes();


	_ssbo.Generate(sizeof(fms::GPUObjectData) * 10000, 2, GL_SHADER_STORAGE_BUFFER);
	auto finalShader = fm::ResourcesManager::get().getResource<fm::Shader>(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "simple.shader"));
	//auto lightShader = fm::ResourcesManager::get().getResource<fm::Shader>(fm::FilePath(fm::LOCATION::INTERNAL_SHADERS_LOCATION, "simple.shader"));

	_finalShader = std::make_unique<fm::OGLShader>(finalShader->GetSubShader(fm::SHADER_KIND::PLAIN).value());
	_finalShader->compile();
	//_lightShader = std::make_unique<fm::OGLShader>(finalShader->GetSubShader(fm::SHADER_KIND::PLAIN));

	//_textdef.projection = fm::math::ortho(
	//	0.0f, (float)_width, 0.0f, (float)_height);

}

RenderingSystem::~RenderingSystem()
{
}


void RenderingSystem::_InitStandardShapes()
{
    std::shared_ptr<fm::Model> quad =	std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Quad"));
    std::shared_ptr<fm::Model> quadFS = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "QuadFS"));
    std::shared_ptr<fm::Model> circle = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Circle"));
    std::shared_ptr<fm::Model> cube =	std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_MODELS_LOCATION, "Cube"));
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

    fm::Renderer::getInstance().SetQuadScreen(_models.find(quadFS->GetID())->second.get());
}



void RenderingSystem::pre_update(EntityManager& em)
{
	for (auto &&e : em.iterate<fmc::CCamera, fmc::CTransform>(fm::IsEntityActive))
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
			cam->InitRenderConfig(tr, sizeof(PointLight)*NUMBER_POINTLIGHT_MAX);
		}

		cam->UpdateRenderTexture();
		cam->UpdateRenderConfigBounds(tr);
		cam->UpdateViewMatrix(ct->GetTransform());
	}
}


void RenderingSystem::update(float, EntityManager& em, EventManager&)
{
	uint32_t instance = 0;

	for (auto &&e : em.iterate<fmc::CCamera>(fm::IsEntityActive))
	{
		LOG_DEBUG
		fmc::CCamera* cam = e.get<fmc::CCamera>();
		if (!cam->Enabled || (!cam->_isAuto && cam->_commandBuffers.empty()))
			continue;


		fmc::CTransform* transform = e.get<fmc::CTransform>();

		if (!cam->GetRenderTexture().isCreated())
		{
			fm::Debug::logError("No render texture created");
			return;
		}

		if (cam->_onStartRendering != nullptr)
		{
			cam->_onStartRendering();
		}

		if (cam->_target != nullptr)
		{
			if (!cam->_target->isCreated())
			{
				cam->_target->create();
			}
		}
		LOG_DEBUG

		if (cam->_isAuto)
		{
			if (cam->_target != nullptr)
			{
				cam->_target->bind();
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

			cam->_rendererConfiguration.postProcessRenderTexture.bind();
			_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

			cam->_renderTexture.bind();
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

		LOG_DEBUG
		//Prepare camera data
		cam->UpdateShaderData();

		int error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}

		if (cam->_isAuto)
		{
			_FillQueue(cam, em);
			for (const fm::RenderQueue::Batch& batch : cam->_rendererConfiguration.queue.Iterate())
			{
				_DrawMeshInstaned(cam, batch.node.transform, batch.node.model.lock(), batch.node.material.lock(), nullptr, batch.number, batch.baseInstance);
			}
		}
		else
		{
			_ExecuteCommandBuffer(fm::RENDER_QUEUE_BEFORE_RENDERING_FILL_QUEUE, cam, instance);
		}

		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		//if (cam->_isAuto)
		//{
			cam->_rendererConfiguration.postProcessRenderTexture.bind();
			_finalShader->Use();
			_finalShader->setValue("screenSize", fm::math::vec2((float)cam->GetViewport().w, (float)cam->GetViewport().h));
			_finalShader->setValue("viewPos", transform->GetWorldPosition());
			_finalShader->setValue("screenTexture", 0);

			fm::Renderer::getInstance().postProcess(_graphics, cam->_renderTexture.GetColorBufferTexture(0));


			if (cam->_target != nullptr)
			{
				fm::Renderer::getInstance().blit(_graphics, cam->_rendererConfiguration.postProcessRenderTexture, *(cam->_target.get()), fm::BUFFER_BIT::COLOR_BUFFER_BIT);
			}
			else
			{
				fm::Renderer::getInstance().blit(_graphics, cam->_rendererConfiguration.postProcessRenderTexture, fm::BUFFER_BIT::COLOR_BUFFER_BIT);
			}
		//}
			fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		_ExecuteCommandBuffer(fm::RENDER_QUEUE_AFTER_RENDERING, cam, instance);
		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		if (cam->_onPostRendering != nullptr)
		{
			cam->_onPostRendering();
		}

		_graphics.BindFrameBuffer(0);

		fmc::CameraCommandBuffer().swap(cam->_commandBuffers);
	}
}


bool RenderingSystem::_HasCommandBuffer(fm::RENDER_QUEUE inRenderQueue, fmc::CCamera* currentCamera)
{
	return !currentCamera->_commandBuffers[inRenderQueue].empty();
}


void RenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE queue, fmc::CCamera* currentCamera, uint32_t& instance)
{
	LOG_DEBUG
	fmc::CameraCommandBuffer::iterator it = currentCamera->_commandBuffers.find(queue);
	_currentCamera = nullptr;
	_currentMaterial = nullptr;
	if (it != currentCamera->_commandBuffers.end())
	{
		std::queue<fm::CommandBuffer> &cmdBuffers = it->second;

		while (!cmdBuffers.empty())
		{
			fm::CommandBuffer cmdBuffer = cmdBuffers.front();
			fm::Command && cmd = cmdBuffer.Pop();
			if (cmd._command == fm::Command::COMMAND_KIND::BLIT)
			{
				fm::OGLShader* shader = _finalShader.get();
				if (auto m  = cmd._material.lock(); m->GetSubShader().has_value())
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
						std::vector<fm::OGLTexture> textures = currentCamera->_renderTexture.GetColorBuffer();
						fm::Renderer::getInstance().SetSources(_graphics, currentCamera->_renderTexture.GetColorBuffer(), textures.size());
					}
				}


				if (cmd._destination.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
				{
					if (cmd._destination.renderTexture != nullptr)
					{
						fm::Renderer::getInstance().blit(_graphics, *cmd._destination.renderTexture, shader);
					}
				}
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DRAW_MESH)
			{
				assert(true);
				_DrawMeshInstaned(currentCamera, cmd._transform, cmd._model.lock(), cmd._material.lock(), &cmd._materialProperties, 1, instance);
				instance++;
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::CLEAR)
			{
				if(cmd._source.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
					cmd._source.renderTexture->bind();
				_graphics.Clear(cmd._bufferBit);
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::CLEAR_ALL)
			{
				if (currentCamera->_target != nullptr)
				{
					currentCamera->_target->bind();
				}
				else
				{
					_graphics.BindFrameBuffer(0);
				}

				_graphics.SetViewPort(currentCamera->GetViewport());
				_graphics.Clear(cmd._bufferBit);
				currentCamera->_rendererConfiguration.postProcessRenderTexture.bind();
				_graphics.Clear(cmd._bufferBit);

				currentCamera->_renderTexture.bind();
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
		currentCamera->_commandBuffers.erase(it);
	}
	LOG_DEBUG
}

std::shared_ptr<fm::OGLShader> RenderingSystem::_FindOrCreateShader(const fm::SubShader& inSubShader)
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


void RenderingSystem::_PrepareShader(fmc::CCamera* cam, const fm::Transform& inTransform,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties* inMaterialProperties)
{
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
			std::unique_ptr<fm::OGLMaterial> mat = std::make_unique< fm::OGLMaterial>(info);
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

		_currentCamera->BindBuffer();
		_currentCamera->SetBuffer((void*)&camData, sizeof(fmc::Shader_data));
	}

	fm::MaterialProperties properties;
	if (inMaterialProperties != nullptr)
	{
		for (const auto& [name, property] : *inMaterialProperties)
		{
			properties.AddValue(name, property);
		}
	}
	fm::math::mat modelPosition = inTransform.worldTransform;
	properties.AddValue("FM_M", modelPosition);
	_currentMaterial->Bind(properties);
}

void RenderingSystem::_DrawMeshInstaned(fmc::CCamera* cam, const fm::Transform& inTransform, std::shared_ptr<fm::Model> inModel,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties* inMaterialProperties, uint32_t inNumber, uint32_t inBaseInstance)
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

void RenderingSystem::_DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, fm::Model *inModel,
	std::shared_ptr<fm::Material> inMaterial, fm::MaterialProperties *inMaterialProperties)
{
	//if (!inMaterial->IsReady()) return;

#if 1
	LOG_DEBUG
	_PrepareShader(cam, inTransform, inMaterial, inMaterialProperties);
	LOG_DEBUG

	if (inModel != nullptr)
	{
		auto it = _models.find(inModel->GetID());
		if (it != _models.end())
		{
			it->second->Draw();
		}
	}
	LOG_DEBUG
#else
	std::shared_ptr<fm::Shader> shader = inMaterial->GetShader();

	if (shader != nullptr)
	{
		shader->Use();
		shader->setValue("FM_PV", cam->shader_data.FM_PV);
		fm::math::mat modelPosition = inTransform.worldTransform;
		shader->setValue("FM_PVM", cam->shader_data.FM_PV * modelPosition);
		shader->setValue("FM_M", modelPosition);
		shader->setValue("lightNumber", _lightNumber);
		shader->setValue("viewPos", inTransform.position);

		cam->_rendererConfiguration.uboLight->Bind();
		std::dynamic_pointer_cast<fm::OGLShader>(shader)->SetUniformBuffer("PointLights", cam->_rendererConfiguration.uboLight->GetBindingPoint());

		for (auto const& value : inMaterial->GetUniforms())
		{
			shader->setValue(value.name, value.materialValue);
		}

		if (inMaterialProperties != nullptr)
		{
			for (auto const& value : *inMaterialProperties)
			{
				shader->setValue(value.name, value.materialValue);
			}
		}
		if (inModel != nullptr)
		{
			auto it = _models.find(inModel->GetID());
			if (it != _models.end())
			{
				_graphics.Draw(it->second.get());
			}
		}
	}
#endif
}



void RenderingSystem::_Draw(fmc::CCamera* cam)
{

    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
	_currentMaterial = nullptr;
	_currentCamera = nullptr;


	//int i = 0;
/*    while (!cam->_rendererConfiguration.queue.Empty())
    {
        const fm::RenderNode node = cam->_rendererConfiguration.queue.getFrontElement();
        const fm::Transform transform = node.transform;
		const fm::RENDER_QUEUE state = node.state;
		const fm::Materials* materials = node.mat;
        std::optional<std::weak_ptr<fm::Model>> model = node.model;
		fmc::CText* text = node.text;

		if(state == fm::RENDER_QUEUE::OPAQUE)
		{
			if (text != nullptr)
			{
				for (const auto& m : *materials)
				{
					_DrawText(cam, transform, text, m.get());
				}
			}


			if (std::shared_ptr<fm::Model> wModel = model->lock())
			{
				for (const auto& m : *materials)
				{
					_DrawMeshInstaned(cam, transform, wModel.get(), m, nullptr, 1, i);
				}
			}
			i++;
                
		}

		cam->_rendererConfiguration.queuePreviousValue = state;
		cam->_rendererConfiguration.queue.next();
        
    }*/

}

void RenderingSystem::_ComputeLighting( std::shared_ptr<fm::RenderTexture> lightRenderTexture,
										 fmc::CCamera* cam,
										 bool hasLight) 
{
	fm::Renderer::getInstance().lightComputation(_graphics, cam->_rendererConfiguration.postProcessRenderTexture.GetColorBufferTexture(0), false);
}

void RenderingSystem::_FillQueue(fmc::CCamera* cam, EntityManager& em)
{
	cam->_rendererConfiguration.queue.Init();

	std::vector<fms::GPUObjectData> objectDatas;
    for(auto &&e : em.iterate<fmc::CTransform>(fm::IsEntityActive))
    {
		fmc::CTransform* transform = e.get<fmc::CTransform>();
		fm::RenderNode node;
		node.state = fm::RENDER_QUEUE_OPAQUE;
		node.transform = transform->GetTransform();
		node.text = e.get<fmc::CText>();
        

		fmc::CMesh* mesh = e.get<fmc::CMesh>();
		fmc::CMaterial* material = e.get<fmc::CMaterial>();
		bool add = false;
		bool hasMesh = false;
		if (mesh != nullptr && material != nullptr && mesh->GetModel() != nullptr)
		{
			hasMesh = true;
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
			cam->_rendererConfiguration.queue.AddElement(node);
		}

		if (hasMesh)
		{
			fms::GPUObjectData data;
			data.modelMatrix = transform->GetWorldPosMatrix();
			objectDatas.emplace_back(data);
		}
		
    }
	if (!objectDatas.empty())
	{
		_ssbo.SetData(objectDatas.data(), objectDatas.size() * sizeof(fms::GPUObjectData));
	}

	cam->_rendererConfiguration.queue.Sort();

}

void RenderingSystem::over()
{
    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
}



void RenderingSystem::_DrawText(fmc::CCamera* cam,
								const fm::Transform& inTransform,
								fmc::CText* inText,
								fm::Material* inMaterial)
{

	if (inText->GetText().empty())
		return;

	assert(false);
	/*
	std::weak_ptr<fm::RFont> font = fm::ResourcesManager::get().getResource<fm::RFont>(inText->GetFontName());
	std::weak_ptr<fm::Shader> shader = fm::ResourcesManager::get().getResource<fm::Shader>(inMaterial->GetShaderPath());
	fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
	if (std::shared_ptr<fm::Shader> wshader = shader.lock())
	{
		wshader->Use();
		wshader->setValue("FM_PV", cam->shader_data.FM_PV);
		wshader->setValue("viewPos", inTransform.position);
		fm::math::mat modelPosition = inTransform.worldTransform;

		wshader->setValue("FM_PVM", cam->shader_data.FM_PV * modelPosition);
		wshader->setValue("FM_M", modelPosition);
		wshader->setValue("text", 0);
		fm::math::mat projection;
		if (inText->GetTextType() == fmc::CText::TEXT_RENDERING::OVERLAY)
			projection = cam->GetOrthographicProjectionForText();
		fm::math::mat m = fmc::CTransform::CreateMatrixModel(fm::math::vec3(inTransform.position.x, inTransform.position.y, 0),
			fm::math::vec3(inTransform.scale.x, inTransform.scale.y, 1),
			inTransform.rotation, true);

		fm::math::mat pm = m * projection;
		wshader->setValue("PM", pm);
		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		for (auto const& value : inMaterial->GetUniforms())
		{
			wshader->setValue(value.name, value.materialValue);
		}

		if (std::shared_ptr<fm::RFont> wFont = font.lock())
		{
			inText->UpdateBuffer(inTransform, wFont.get());
			_graphics.ActivateTexture2D(0);
			wFont->texture->bind();
		}


		_graphics.BindVertexBuffer(dynamic_cast<fm::rendering::OGLVertextBuffer*>(inText->GetVertexBuffer()));
		_graphics.Draw(0, 0, inText->GetVertexBuffer()->GetNumberVertices());

		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
	}
	*/

}

}  // namespace fms
