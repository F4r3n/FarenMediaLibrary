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
	_IsAffectedByStartAndStop = false;

}

void RenderingSystem::_InitUniformBufferCamera(fmc::CCamera* inCamera)
{


#if OPENGL_ES == 0
    glGenBuffers(1, &inCamera->_rendererConfiguration.generatedBlockBinding);
    glBindBuffer(GL_UNIFORM_BUFFER, inCamera->_rendererConfiguration.generatedBlockBinding);
    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof(inCamera->shader_data),
                 &inCamera->shader_data,
                 GL_DYNAMIC_COPY);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(auto shader : fm::ResourcesManager::get().getAll<fm::Shader>()) {
        fm::Shader* s = dynamic_cast<fm::Shader*>(shader.second);
        s->Use();

        glBindBufferBase(
                    GL_UNIFORM_BUFFER, inCamera->_rendererConfiguration.bindingPointIndex, inCamera->_rendererConfiguration.generatedBlockBinding);
        glUniformBlockBinding(s->Program,
                              glGetUniformBlockIndex(s->Program, "shader_data"),
			inCamera->_rendererConfiguration.bindingPointIndex);
    }
#endif
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::_UpdateUniformBufferCamera(fmc::CCamera* inCamera)
{
#if OPENGL_ES == 0
    glBindBuffer(GL_UNIFORM_BUFFER, inCamera->_rendererConfiguration.generatedBlockBinding);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &inCamera->shader_data, sizeof(inCamera->shader_data));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
}

void RenderingSystem::_InitStandardShapes()
{
    fm::Model* quad = new fm::Model("Quad");
    fm::Model* quadFS = new fm::Model("QuadFS");
    fm::Model* circle = new fm::Model("Circle");
    fm::Model* cube = new fm::Model("Cube");
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
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _InitStandardShapes();

    for(auto &&e : em.iterate<fmc::CMesh>()) {
        fmc::CMesh* mesh = e->get<fmc::CMesh>();

        mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
    }

    for(auto &&e : em.iterate<fmc::CMaterial>())
    {
        fmc::CMaterial* material = e->get<fmc::CMaterial>();
        std::vector<fm::Material*> materials = material->GetAllMaterials();
        for(auto &m : materials)
        {
            if(m->shader == nullptr && m->shaderName != "")
            {
                m->shader =
                        fm::ResourcesManager::get().getResource<fm::Shader>(
                            m->shaderName);
            }
            if(m->shader != nullptr && !m->shader->IsReady())
            {
                m->shader->compile();
            }
        }

    }

    fm::Debug::log("INIT MainCamera");
    for(auto &&e : em.iterate<fmc::CCamera>())
    {
        InitCamera(e);
        break;
    }
    

}

void RenderingSystem::InitCamera(Entity* inEntityCamera)
{
	fmc::CCamera* camera = inEntityCamera->get<fmc::CCamera>();

	if (camera!= nullptr && !camera->_rendererConfiguration.isInit)
	{
		camera->_rendererConfiguration.uboLight = std::make_unique<fm::UniformBuffer>(fm::UniformBuffer());
		camera->_rendererConfiguration.uboLight->Generate(sizeof(PointLight)*NUMBER_POINTLIGHT_MAX, 2);

		fmc::CTransform* ct = inEntityCamera->get<fmc::CTransform>();
		camera->_viewMatrix = fm::math::mat();
		_SetView(camera->_viewMatrix, ct->position, { camera->viewPort.w, camera->viewPort.h }, ct->rotation, camera->IsOrthographic());

		fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };
		fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
		camera->_rendererConfiguration.lightRenderTexture = fm::RenderTexture(
			camera->_renderTexture.getWidth(),
			camera->_renderTexture.getHeight(),
			2,
			formats,
			types,
			0);

		camera->_rendererConfiguration.postProcessRenderTexture = fm::RenderTexture(
			camera->_renderTexture.getWidth(),
			camera->_renderTexture.getHeight(),
			1,
			formats,
			types,
			0);

		camera->_rendererConfiguration.resolveMSAARenderTexture = fm::RenderTexture(camera->_renderTexture, 0);

		camera->_rendererConfiguration.lightRenderTexture.create();
		camera->_rendererConfiguration.postProcessRenderTexture.create();
		camera->_rendererConfiguration.resolveMSAARenderTexture.create();

		camera->_rendererConfiguration.isInit = true;
	}
	

	
}

void UpdateCameraVectors(float yaw, float pitch, float roll, fm::math::vec3 &outFront,fm::math::vec3 &outRight,fm::math::vec3 &outUp,  const fm::math::vec3 &worldUp)
{
    // Calculate the new Front vector
    fm::math::vec3 front;
    front.x = cos(fm::math::radians(yaw)) * cos(fm::math::radians(pitch));
    front.y = sin(fm::math::radians(pitch));
    front.z = sin(fm::math::radians(yaw)) * cos(fm::math::radians(pitch));
    outFront = fm::math::normalize(front);
    // Also re-calculate the Right and Up vector
    outRight = fm::math::normalize(fm::math::cross(outFront, fm::math::vec3(cos(fm::math::radians(roll)), sin(fm::math::radians(roll)), 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    outUp    = fm::math::normalize(fm::math::cross(outRight, outFront));
}

void RenderingSystem::_SetView(fm::math::mat& viewMatrix,
                              const fm::math::Vector3f& position,
                              const fm::math::Vector2f& size,
                              const fm::math::Vector3f& rotation, bool isOrthographic)
{

    if(isOrthographic)
    {
        fm::math::mat m;
        m.identity();
        viewMatrix = fm::math::translate(m,fm::math::vec3(-position.x, -position.y, -position.z));
        viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        viewMatrix = fm::math::rotate(viewMatrix, rotation.x, fm::math::vec3(0, 0, 1));
        viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    }
    else
    {
        viewMatrix.identity();
        viewMatrix = fm::math::rotate( viewMatrix, fm::math::radians(rotation.x), fm::math::vec3(1,0,0));
        viewMatrix = fm::math::rotate( viewMatrix, fm::math::radians(rotation.y), fm::math::vec3(0,1,0));
        viewMatrix = fm::math::rotate( viewMatrix, fm::math::radians(rotation.z), fm::math::vec3(0,0,1));
        viewMatrix = fm::math::translate(viewMatrix,fm::math::vec3(-position.x, -position.y, position.z));
    }
}

void RenderingSystem::pre_update(EntityManager& em)
{
	for (auto &&e : em.iterate<fmc::CCamera>())
	{
		fmc::CCamera* cam = e->get<fmc::CCamera>();
		if (!cam->Enabled)
			continue;

		if (!cam->IsInit())
		{
			cam->Init();
		}

		if (!cam->_rendererConfiguration.isInit)
		{
			InitCamera(e);
		}


		cam->UpdateRenderTexture();
		fmc::CTransform* ct = e->get<fmc::CTransform>();

		cam->_rendererConfiguration.bounds.setSize(fm::math::vec3(cam->viewPort.w, cam->viewPort.h, cam->GetFarPlane() - cam->GetNearPlane()));
		cam->_rendererConfiguration.bounds.setCenter(fm::math::vec3(ct->position.x, ct->position.y, -1) + cam->_rendererConfiguration.bounds.getSize() / 2.0f);
		cam->_rendererConfiguration.bounds.setScale(fm::math::vec3(1, 1, 1));

		_SetView(cam->_viewMatrix, ct->position, { cam->viewPort.w, cam->viewPort.h }, ct->rotation, cam->IsOrthographic());
	}

}

void RenderingSystem::update(float, EntityManager& em, EventManager&)
{
	for (auto &&e : em.iterate<fmc::CCamera>())
	{
		fmc::CCamera* cam = e->get<fmc::CCamera>();

		if (!cam->Enabled)
			continue;

		fmc::CTransform* transform = e->get<fmc::CTransform>();

		if (!cam->_renderTexture.isCreated())
		{
			fm::Debug::logError("No render texture created");
			return;
		}


		//Clear buffers
		_graphics.BindFrameBuffer(0);
		_graphics.SetViewPort(cam->viewPort);
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

		if (cam->target != nullptr)
		{
			if (!cam->target->isCreated())
			{
				cam->target->create();
			}
			cam->target->bind();
			_graphics.SetViewPort(cam->viewPort);
			_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
		}

		cam->_rendererConfiguration.lightRenderTexture.bind();
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

		cam->_rendererConfiguration.postProcessRenderTexture.bind();
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

		cam->_rendererConfiguration.resolveMSAARenderTexture.bind();
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);

		cam->_renderTexture.bind();
		_graphics.SetViewPort(cam->viewPort);
		_graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
		_graphics.Enable(fm::RENDERING_TYPE::DEPTH_TEST);

		//Prepare camera data
		cam->shader_data.FM_PV = cam->projection * cam->_viewMatrix;
		cam->shader_data.FM_P = cam->projection;
		cam->shader_data.FM_V = cam->_viewMatrix;

		int error = glGetError();
		if (error != 0) {
			fm::Debug::logError("ERROR opengl" + std::string(LINE_STRING));
			exit(-1);
		}

		_FillQueue(cam, em);

		if (!cam->_rendererConfiguration.queue.Empty())
		{
			cam->_rendererConfiguration.queue.start();
			_DrawMesh(cam);
		}

		//Resolve MSAA
		fm::Renderer::getInstance().blit(_graphics, cam->_renderTexture, cam->_rendererConfiguration.resolveMSAARenderTexture, fm::BUFFER_BIT::COLOR_BUFFER_BIT);


		cam->_rendererConfiguration.postProcessRenderTexture.bind();
		_finalShader->Use();
		_finalShader->setValue("screenSize", fm::math::vec2(cam->viewPort.w, cam->viewPort.h));
		_finalShader->setValue("viewPos", transform->position);
		_finalShader->setValue("screenTexture", 0);

		fm::Renderer::getInstance().postProcess(_graphics, cam->_rendererConfiguration.resolveMSAARenderTexture.getColorBuffer()[0]);

		if (cam->target != nullptr)
		{
			fm::Renderer::getInstance().blit(_graphics, cam->_rendererConfiguration.postProcessRenderTexture, *(cam->target.get()), fm::BUFFER_BIT::COLOR_BUFFER_BIT);
		}
		else
		{
			fm::Renderer::getInstance().blit(_graphics, cam->_rendererConfiguration.postProcessRenderTexture, fm::BUFFER_BIT::COLOR_BUFFER_BIT);
		}


		_ExecuteCommandBuffer(fm::RENDER_QUEUE::AFTER_RENDERING, cam);

		_graphics.BindFrameBuffer(0);

	}
}

void RenderingSystem::_ExecuteCommandBuffer(fm::RENDER_QUEUE queue, fmc::CCamera* currentCamera)
{
	std::queue<fm::CommandBuffer> &cmdBuffers = currentCamera->_commandBuffers[queue];

	while (!cmdBuffers.empty())
	{
		fm::CommandBuffer cmdBuffer = cmdBuffers.front();
		fm::Command && cmd = cmdBuffer.Pop();
		if (cmd._command == fm::Command::COMMAND_KIND::BLIT)
		{
			fm::Shader *shader = _finalShader;
			if (cmd._material != nullptr)
			{
				shader = cmd._material->shader;
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
					_graphics.BindTexture2D(0, cmd._source.texture->GetKind(), cmd._source.texture->getID());
				}
				else
				{
					fm::Renderer::getInstance().SetSources(_graphics, currentCamera->_renderTexture.getColorBuffer(), 2);
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
		cmdBuffers.pop();
	}
}


void RenderingSystem::_DrawMesh(fmc::CCamera* cam)
{

    bool hasLight = false;
    bool computeLightDone = false;
    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

    while(!cam->_rendererConfiguration.queue.Empty())
    {

        const fm::RenderNode node = cam->_rendererConfiguration.queue.getFrontElement();
        const fmc::CTransform* transform = node.transform;
		const fm::math::Vector3f worldPos = transform->getWorldPos();
		const fm::RENDER_QUEUE state = node.state;

        fmc::CMaterial* material = node.mat;
        fmc::CMesh* mesh = node.mesh;

        if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
        {

            if(state == fm::RENDER_QUEUE::OPAQUE)
            {

                if(mesh && material)
                {
                    if(mesh->model == nullptr || mesh->model->GetName().compare(mesh->GetModelType()) != 0)
                    {
                        mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
                    }



                    for(auto &m : material->GetAllMaterials())
                    {

						if(m->shader != nullptr && !m->shader->IsReady())
						{
						    m->shader->compile();
						}

						fm::Shader* shader = m->shader;

						if(m->Reload())
						{
							shader = m->shader;
							shader->Use();
							cam->_rendererConfiguration.uboLight->Bind();

							shader->SetUniformBuffer("PointLights", cam->_rendererConfiguration.uboLight->GetBindingPoint());
						    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
						}

						if(shader != nullptr)
						{
						    shader->Use()
						            ->setValue("FM_PV", cam->shader_data.FM_PV);
						    shader->setValue("lightNumber",_lightNumber );
						    shader->setValue("viewPos", transform->getWorldPos());
						    fm::math::mat model = fm::math::mat();
						    _SetModelPosition(model, transform, worldPos, cam->IsOrthographic());

						    shader->setValue("FM_PVM", cam->shader_data.FM_PV * model);
						    shader->setValue("FM_M", model);

						    for(auto const& value : m->getValues())
						    {
						        shader->setValue(value.name, value.materialValue);
						    }
							if(mesh->model != nullptr)
								_graphics.Draw(mesh->model);
						}
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



    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
    {
        fm::Renderer::getInstance().lightComputation(_graphics, cam->_rendererConfiguration.postProcessRenderTexture.getColorBuffer(), hasLight);
    } 
	else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
    {
        fm::Renderer::getInstance().lightComputation(_graphics, cam->_rendererConfiguration.postProcessRenderTexture.getColorBuffer(), false);
    }

}

void RenderingSystem::_FillQueue(fmc::CCamera* cam, EntityManager& em)
{
	cam->_rendererConfiguration.queue.init();
    PointLight pointLights[NUMBER_POINTLIGHT_MAX];
    _lightNumber = 0;
    for(auto &&e : em.iterate<fmc::CTransform>())
    {
        fm::RenderNode node = {e->get<fmc::CTransform>(),
                               e->get<fmc::CMaterial>(),
                               e->get<fmc::CMesh>(),
                               e->get<fmc::CDirectionalLight>(),
                               e->get<fmc::CPointLight>(),
                               e->get<fmc::CText>(),
                               fm::RENDER_QUEUE::BACKGROUND,
                               0,
                               e->ID};
        bool valid = false;
        if(node.mesh)
        {
            node.mesh->bounds.setCenter(fm::math::vec3(node.transform->position) + fm::math::vec3(node.transform->scale) / 2.0f);
            node.mesh->bounds.setScale(fm::math::vec3(node.transform->scale));

            valid = true;
            node.state = fm::RENDER_QUEUE::OPAQUE;
        }

        if(node.plight)
        {
            PointLight p;
            fm::math::vec4 c;
            c.x = node.plight->color.r;
            c.y = node.plight->color.g;
            c.z = node.plight->color.b;
            c.w = node.plight->color.a;

            p.color = c;

            p.position = fm::math::vec4(node.transform->getWorldPos());
            p.custom.x = node.plight->radius;
            pointLights[_lightNumber] = p;
            _lightNumber++;
        }

        if(!valid)
        {
            continue;
        }
		cam->_rendererConfiguration.queue.addElement(node);
    }
	cam->_rendererConfiguration.uboLight->Bind();
	cam->_rendererConfiguration.uboLight->SetData(&pointLights[0], _lightNumber*sizeof(PointLight));
}

void RenderingSystem::over()
{
    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
}

void RenderingSystem::_SetModelPosition(fm::math::mat& model,
                               const fmc::CTransform* transform,
                               const fm::math::Vector3f& worldPos, bool isOrthographic)
{
    if(isOrthographic)
    {
        model = fm::math::translate( model, fm::math::vec3(worldPos.x, worldPos.y, (float)-transform->layer));
        model = fm::math::translate( model,fm::math::vec3( 0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
        model = fm::math::rotate( model, transform->rotation.x, fm::math::vec3(0.0f, 0.0f, 1.0f));
        model = fm::math::translate( model, fm::math::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
        model = fm::math::scale(model, fm::math::vec3(transform->scale.x, transform->scale.y, 1.0f));
    }else
    {
        model = fm::math::translate( model, fm::math::vec3(worldPos.x, worldPos.y, worldPos.z));
        model = fm::math::rotate( model, fm::math::radians(transform->rotation.x), fm::math::vec3(1,0,0));
        model = fm::math::rotate( model, fm::math::radians(transform->rotation.y), fm::math::vec3(0,1,0));
        model = fm::math::rotate( model, fm::math::radians(transform->rotation.z), fm::math::vec3(0,0,1));

        model = fm::math::scale(model, fm::math::vec3(transform->scale.x, transform->scale.y, transform->scale.z));
    }

}

void RenderingSystem::_DrawText(int posX, int posY,
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
