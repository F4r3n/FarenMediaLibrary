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
RenderingSystem::RenderingSystem(int width, int height)
    : _width(width), _height(height)
{
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
    fm::Model* quad = new fm::Model(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Quad"));
    fm::Model* quadFS = new fm::Model(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "QuadFS"));
    fm::Model* circle = new fm::Model(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Circle"));
    fm::Model* cube = new fm::Model(fm::FilePath(fm::LOCATION::INTERNAL_RESOURCES_LOCATION, "Cube"));
    quad->AddMesh(fm::StandardShapes::CreateQuad());
    circle->AddMesh(fm::StandardShapes::CreateCircle());
    quadFS->AddMesh(fm::StandardShapes::CreateQuadFullScreen());
    cube->AddMesh(fm::StandardShapes::CreateCube());


    quad->generate();
    circle->generate();
    quadFS->generate();
    cube->generate();
    fm::ResourcesManager::get().load<fm::Model>(quad->GetName(), quad);
    fm::ResourcesManager::get().load<fm::Model>(quadFS->GetName(), quadFS);
    fm::ResourcesManager::get().load<fm::Model>(circle->GetName(), circle);
    fm::ResourcesManager::get().load<fm::Model>(cube->GetName(), cube);

    fm::Renderer::getInstance().createQuadScreen();
}


void RenderingSystem::init(EntityManager& em, EventManager&)
{
    fm::Debug::log("INIT Standard Shapes");

    _InitStandardShapes();

    for(auto &&e : em.iterate<fmc::CMesh>()) 
	{
        fmc::CMesh* mesh = e->get<fmc::CMesh>();

        mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
    }

    for(auto &&e : em.iterate<fmc::CMaterial>())
    {
        fmc::CMaterial* material = e->get<fmc::CMaterial>();
        std::vector<fm::Material*> materials = material->GetAllMaterials();
        for(auto &m : materials)
        {
			m->Compile();
        }
    }

    //fm::Debug::log("INIT MainCamera");
    //for(auto &&e : em.iterate<fmc::CCamera>())
    //{
    //    InitCamera(e);
    //    break;
    //}
}




void RenderingSystem::pre_update(EntityManager& em)
{
	for (auto &&e : em.iterate<fmc::CCamera, fmc::CTransform>())
	{
		fmc::CCamera* cam = e->get<fmc::CCamera>();
		fmc::CTransform* ct = e->get<fmc::CTransform>();
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
	for (auto &&e : em.iterate<fmc::CCamera>())
	{
		fmc::CCamera* cam = e->get<fmc::CCamera>();
		if (!cam->Enabled || (!cam->_isAuto && cam->_commandBuffers.empty()))
			continue;


		fmc::CTransform* transform = e->get<fmc::CTransform>();

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
		}
		else
		{
			_ExecuteCommandBuffer(fm::RENDER_QUEUE::FIRST_STATE, cam);
		}


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
				_DrawMesh(cam);
			}
		}
		else
		{
			_ExecuteCommandBuffer(fm::RENDER_QUEUE::BEFORE_RENDERING_FILL_QUEUE, cam);
		}

		error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}

		error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}
		//if (cam->_isAuto)
		//{
			cam->_rendererConfiguration.postProcessRenderTexture.bind();
			_finalShader->Use();
			_finalShader->setValue("screenSize", fm::math::vec2((float)cam->GetViewport().w, (float)cam->GetViewport().h));
			_finalShader->setValue("viewPos", transform->position);
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
		error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}
		_ExecuteCommandBuffer(fm::RENDER_QUEUE::AFTER_RENDERING, cam);
		error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
		}
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


void RenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE queue, fmc::CCamera* currentCamera)
{
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
				fm::Shader *shader = _finalShader;
				if (cmd._material != nullptr)
				{
					shader = cmd._material->GetShader();
					shader->Use();
					for (auto const& value : cmd._material->getValues())
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
						fm::Renderer::getInstance().blit(_graphics, *cmd._destination.renderTexture, shader);
					}
				}
			}
			else if (cmd._command == fm::Command::COMMAND_KIND::DRAW_MESH)
			{
				_DrawMesh(currentCamera, cmd._transform, cmd._model, cmd._material, &cmd._materialProperties);
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
					_graphics.SetViewPort(currentCamera->GetViewport());
					_graphics.Clear(cmd._bufferBit);
				}
				else
				{
					//Clear buffers
					_graphics.BindFrameBuffer(0);
					_graphics.SetViewPort(currentCamera->GetViewport());
					_graphics.Clear(cmd._bufferBit);
				}

				currentCamera->_rendererConfiguration.postProcessRenderTexture.bind();
				_graphics.Clear(cmd._bufferBit);

				currentCamera->_renderTexture.bind();
				_graphics.SetViewPort(currentCamera->GetViewport());
				_graphics.Clear(cmd._bufferBit);
			}
			cmdBuffers.pop();
		}
		currentCamera->_commandBuffers.erase(it);
	}
}

void RenderingSystem::_DrawMesh(fmc::CCamera *cam, const fm::Transform &inTransform, fm::Model *inModel, fm::Material* inMaterial, fm::MaterialProperties *inMaterialProperties)
{
	if (!inMaterial->IsReady()) return;

	fm::Shader* shader = inMaterial->GetShader();

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
		shader->SetUniformBuffer("PointLights", cam->_rendererConfiguration.uboLight->GetBindingPoint());

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
			_graphics.Draw(inModel);
	}
}



void RenderingSystem::_DrawMesh(fmc::CCamera* cam)
{

    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

    while(!cam->_rendererConfiguration.queue.Empty())
    {
        const fm::RenderNode node = cam->_rendererConfiguration.queue.getFrontElement();
        const fm::Transform transform = node.transform;
		const fm::RENDER_QUEUE state = node.state;
		const fm::Materials* materials = node.mat;
        fm::Model* model = node.model;

        if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
        {
            if(state == fm::RENDER_QUEUE::OPAQUE)
            {
                if(model != nullptr)
                {
                    for(const auto &m : *materials)
                    {
						_DrawMesh(cam, transform, model, m);
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



    //if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
    //{
    //    fm::Renderer::getInstance().lightComputation(_graphics, cam->_rendererConfiguration.postProcessRenderTexture.getColorBuffer(), hasLight);
    //} 
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
    for(auto &&e : em.iterate<fmc::CTransform>())
    {
        fm::RenderNode node = {e->get<fmc::CTransform>()->GetTransform(),
                               nullptr,
                               nullptr,
                               e->get<fmc::CDirectionalLight>(),
                               e->get<fmc::CPointLight>(),
                               e->get<fmc::CText>(),
                               fm::RENDER_QUEUE::OPAQUE,
                               0,
                               e->ID};
        

		fmc::CMesh* mesh = e->get<fmc::CMesh>();
		fmc::CMaterial* material = e->get<fmc::CMaterial>();

		if (mesh != nullptr && material != nullptr)
		{
			node.model = mesh->model;

			node.mat = &material->GetAllMaterials();
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



void RenderingSystem::_DrawText(float posX, float posY,
                               RFont* font,
                               fmc::CText* ctext)
{
    if(font == nullptr || ctext == nullptr) 
	{
        fm::Debug::logError("Font not found");
        return;
    }

    if(ctext->buffer == nullptr) 
	{
        ctext->buffer = new fm::rendering::VertexBuffer();
        ctext->buffer->generate();
    }

    font->texture->bind();

    if(!(ctext->text.compare(ctext->previousText) == 0)) 
	{
        fm::math::vec4 *coords = new fm::math::vec4[6 * ctext->text.size()];
        float x = posX;
        float y = posY;
        std::string::const_iterator c;
        int n = 0;
        for(c = ctext->text.begin(); c != ctext->text.end(); c++) 
		{
            Character ch = font->Characters[*c];
            float x2 = x + ch.b_lt.x * ctext->scale;
            float y2 = -y - ch.b_lt.y * ctext->scale;
            float w = ch.b_wh.x * ctext->scale;
            float h = ch.b_wh.y * ctext->scale;
            x += ch.advance.x * ctext->scale;
            y += ch.advance.y * ctext->scale;
            if(!w || !h)
                continue;

            coords[n++] = fm::math::vec4(x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y);
            coords[n++] = fm::math::vec4(x2, -y2, ch.t.x, ch.t.y);
            coords[n++] = fm::math::vec4(x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height);
            coords[n++] = fm::math::vec4( x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y);
            coords[n++] = fm::math::vec4( x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height);
            coords[n++] =fm::math::vec4(x2 + w, -y2 - h,
                    ch.t.x + ch.b_wh.x / font->atlas_width,
                    ch.t.y + ch.b_wh.y / font->atlas_height);
        }
        ctext->previousText = ctext->text;
        ctext->buffer->setBufferData(&coords[0], n, sizeof(float) * 4, false);
		delete coords;
    }

    if(ctext->buffer != NULL && ctext->buffer->isGenerated()) 
	{
        _graphics.BindVertexBuffer(ctext->buffer);
        _graphics.Draw(0, 0, ctext->buffer->GetNumberVertices());
    }
}

}  // namespace fms
