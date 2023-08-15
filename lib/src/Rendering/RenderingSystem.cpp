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
#include "Rendering/uniformbuffer.hpp"
#include "Rendering/material.hpp"
#include <EntityManager.h>

#include "Rendering/commandBuffer.hpp"
#include "Components/CCamera.h"
#include "Rendering/VertexBuffer.hpp"
#include "Rendering/Model.hpp"
#include "Engine.h"
#include "Rendering/OpenGL/OGLShader.h"
#include "Rendering/OpenGL/OGLModel.h"
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
	fm::ResourcesManager::get().LoadShaders(GRAPHIC_API::OPENGL);
	fm::ResourcesManager::get().LoadFonts();
	fm::ResourcesManager::get().LoadMaterials();

    _finalShader = fm::ResourcesManager::get().getResource<fm::Shader>("simple");
    _lightShader = fm::ResourcesManager::get().getResource<fm::Shader>("light");

    _textdef.projection = fm::math::ortho(
                0.0f, (float)_width, 0.0f, (float)_height);
	_type = SYSTEM_MODE::ALWAYS;

}


RenderingSystem::~RenderingSystem()
{
}


void RenderingSystem::_InitStandardShapes()
{
    std::shared_ptr<fm::Model> quad =	std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Quad"));
    std::shared_ptr<fm::Model> quadFS = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "QuadFS"));
    std::shared_ptr<fm::Model> circle = std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Circle"));
    std::shared_ptr<fm::Model> cube =	std::make_shared<fm::Model>(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Cube"));
    quad->AddMesh(fm::StandardShapes::CreateQuad());
    circle->AddMesh(fm::StandardShapes::CreateCircle());
    quadFS->AddMesh(fm::StandardShapes::CreateQuadFullScreen());
    cube->AddMesh(fm::StandardShapes::CreateCube());

    fm::ResourcesManager::get().load<fm::Model>(quad->GetName(), quad);
    fm::ResourcesManager::get().load<fm::Model>(quadFS->GetName(), quadFS);
    fm::ResourcesManager::get().load<fm::Model>(circle->GetName(), circle);
    fm::ResourcesManager::get().load<fm::Model>(cube->GetName(), cube);
	_models.emplace(quad->GetID(), std::make_unique<fm::OGLModel>(quad));
	_models.emplace(cube->GetID(), std::make_unique<fm::OGLModel>(cube));
	_models.emplace(quadFS->GetID(), std::make_unique<fm::OGLModel>(quadFS));
	_models.emplace(cube->GetID(), std::make_unique<fm::OGLModel>(cube));

	for (const auto& m : _models)
	{
		m.second->UploadData();
	}

    fm::Renderer::getInstance().SetQuadScreen(_models.find(quadFS->GetID())->second.get());
}


void RenderingSystem::init(EntityManager& em, EventManager&)
{
    fm::Debug::log("INIT Standard Shapes");

    _InitStandardShapes();

    for(auto &&e : em.iterate<fmc::CMaterial>(fm::IsEntityActive))
    {
        fmc::CMaterial* material = e.get<fmc::CMaterial>();
        std::vector<std::shared_ptr<fm::Material>> materials = material->GetAllMaterials();
        for(auto &m : materials)
        {
			m->Compile();
        }
    }

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
			_ExecuteCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, cam);
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

			if (!cam->_rendererConfiguration.queue.Empty())
			{
				cam->_rendererConfiguration.queue.start();
				_Draw(cam);
			}
		}
		else
		{
			_ExecuteCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, cam);
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

		_ExecuteCommandBuffer(fm::RENDER_QUEUE::AFTER_RENDERING, cam);
		fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

		if (cam->_onPostRendering != nullptr)
		{
			//glFinish();
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


void RenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE queue, fmc::CCamera* currentCamera)
{
	LOG_DEBUG
	fmc::CameraCommandBuffer::iterator it = currentCamera->_commandBuffers.find(queue);

	if (it != currentCamera->_commandBuffers.end())
	{
		std::queue<fm::CommandBuffer> &cmdBuffers = it->second;

		while (!cmdBuffers.empty())
		{
			fm::CommandBuffer cmdBuffer = cmdBuffers.front();
			fm::Command && cmd = cmdBuffer.Pop();
			if (cmd._command == fm::Command::COMMAND_KIND::BLIT)
			{
				std::shared_ptr<fm::Shader> shader = _finalShader;
				if (auto m  = cmd._material.lock())
				{
					shader = m->GetShader();
					shader->Use();
					for (auto const& value : m->getValues())
					{
						shader->setValue(value.name, value.materialValue);
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
						std::vector<fm::Texture> textures = currentCamera->_renderTexture.GetColorBuffer();
						fm::Renderer::getInstance().SetSources(_graphics, currentCamera->_renderTexture.GetColorBuffer(), textures.size());
					}
				}


				if (cmd._destination.kind == fm::Command::TEXTURE_KIND::RENDER_TEXTURE)
				{
					if (cmd._destination.renderTexture != nullptr)
					{
						fm::Renderer::getInstance().blit(_graphics, *cmd._destination.renderTexture, shader.get());
					}
				}
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DRAW_MESH)
			{
				_DrawMesh(currentCamera, cmd._transform, cmd._model.lock().get(), cmd._material.lock().get(), &cmd._materialProperties);
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

void RenderingSystem::_DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, fm::Model *inModel, fm::Material* inMaterial, fm::MaterialProperties *inMaterialProperties)
{
	if (!inMaterial->IsReady()) return;

	std::shared_ptr<fm::Shader> shader = inMaterial->GetShader();

	if (shader != nullptr)
	{
		shader->Use();
		shader->setValue("FM_PV", cam->shader_data.FM_PV);
		shader->setValue("lightNumber", _lightNumber);
		shader->setValue("viewPos", inTransform.position);
		fm::math::mat modelPosition = inTransform.worldTransform;

		shader->setValue("FM_PVM", cam->shader_data.FM_PV * modelPosition);
		shader->setValue("FM_M", modelPosition);

		cam->_rendererConfiguration.uboLight->Bind();
		std::dynamic_pointer_cast<fm::OGLShader>(shader)->SetUniformBuffer("PointLights", cam->_rendererConfiguration.uboLight->GetBindingPoint());

		for (auto const& value : inMaterial->getValues())
		{
			shader->setValue(value.name, value.materialValue);
		}

		if (inMaterialProperties != nullptr)
		{
			for (auto const& value : inMaterialProperties->GetValues())
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
}



void RenderingSystem::_Draw(fmc::CCamera* cam)
{

    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

    while(!cam->_rendererConfiguration.queue.Empty())
    {
        const fm::RenderNode node = cam->_rendererConfiguration.queue.getFrontElement();
        const fm::Transform transform = node.transform;
		const fm::RENDER_QUEUE state = node.state;
		const fm::Materials* materials = node.mat;
        std::optional<std::weak_ptr<fm::Model>> model = node.model;
		fmc::CText* text = node.text;
        if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
        {
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
						_DrawMesh(cam, transform, wModel.get(), m.get());
					}
				}

                
            }

			cam->_rendererConfiguration.queuePreviousValue = state;
			cam->_rendererConfiguration.queue.next();
        }
		else if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
        {

        }
    }

}

void RenderingSystem::_ComputeLighting( std::shared_ptr<fm::RenderTexture> lightRenderTexture,
										 fmc::CCamera* cam,
										 bool hasLight) 
{
	if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
    {
		fm::Renderer::getInstance().lightComputation(_graphics, cam->_rendererConfiguration.postProcessRenderTexture.GetColorBufferTexture(0), false);
    }

}

void RenderingSystem::_FillQueue(fmc::CCamera* cam, EntityManager& em)
{
	cam->_rendererConfiguration.queue.init();
    PointLight pointLights[NUMBER_POINTLIGHT_MAX];
    _lightNumber = 0;
    for(auto &&e : em.iterate<fmc::CTransform>(fm::IsEntityActive))
    {
        fm::RenderNode node = {e.get<fmc::CTransform>()->GetTransform(),
                               nullptr,
                               std::nullopt,
                               e.get<fmc::CDirectionalLight>(),
                               e.get<fmc::CPointLight>(),
                               e.get<fmc::CText>(),
                               fm::RENDER_QUEUE::OPAQUE,
                               0,
                               e.id()};
        

		fmc::CMesh* mesh = e.get<fmc::CMesh>();
		if (mesh != nullptr && mesh->model == nullptr)
		{
			mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
		}
		fmc::CMaterial* material = e.get<fmc::CMaterial>();
		bool add = false;
		if (mesh != nullptr && material != nullptr)
		{
			node.model = mesh->model;
			node.mat = &material->GetAllMaterials();
			add = true;
		}
		else if (node.text != nullptr && material != nullptr)
		{
			node.mat = &material->GetAllMaterials();
			add = true;
		}

		if (add)
		{
			cam->_rendererConfiguration.queue.addElement(node);
		}



        if(node.plight != nullptr)
        {
            PointLight p;
            fm::math::vec4 c;
            c.x = node.plight->color.r;
            c.y = node.plight->color.g;
            c.z = node.plight->color.b;
            c.w = node.plight->color.a;

            p.color = c;

            p.position = fm::math::vec4(node.transform.position);
            p.custom.x = node.plight->radius;
            pointLights[_lightNumber] = p;
            _lightNumber++;
        }
		
    }
	cam->_rendererConfiguration.uboLight->Bind();
	cam->_rendererConfiguration.uboLight->SetData(&pointLights[0], _lightNumber*sizeof(PointLight));
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

	std::weak_ptr<fm::RFont> font = fm::ResourcesManager::get().getResource<fm::RFont>(inText->GetFontName());
	std::weak_ptr<fm::Shader> shader = inMaterial->GetShader();
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

		for (auto const& value : inMaterial->getValues())
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


}

}  // namespace fms
