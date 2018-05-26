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
namespace fms {
RenderingSystem::RenderingSystem(int width, int height)
    : width(width), height(height)
{
    finalShader = fm::ResourcesManager::get().getResource<fm::Shader>("simple");
    lightShader = fm::ResourcesManager::get().getResource<fm::Shader>("light");

    textdef.projection = fm::math::ortho(
                0.0f, (float)fm::Window::width, 0.0f, (float)fm::Window::height);

}

void RenderingSystem::initUniformBufferCamera(fmc::CCamera* camera)
{
#if OPENGL_ES == 0
    glGenBuffers(1, &generatedBlockBinding);
    glBindBuffer(GL_UNIFORM_BUFFER, generatedBlockBinding);
    glBufferData(GL_UNIFORM_BUFFER,
                 sizeof(camera->shader_data),
                 &camera->shader_data,
                 GL_DYNAMIC_COPY);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(auto shader : fm::ResourcesManager::get().getAll<fm::Shader>()) {
        fm::Shader* s = dynamic_cast<fm::Shader*>(shader.second);
        s->Use();

        glBindBufferBase(
                    GL_UNIFORM_BUFFER, bindingPointIndex, generatedBlockBinding);
        glUniformBlockBinding(s->Program,
                              glGetUniformBlockIndex(s->Program, "shader_data"),
                              bindingPointIndex);
    }
#endif
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::updateUniformBufferCamera(fmc::CCamera* camera)
{
#if OPENGL_ES == 0
    glBindBuffer(GL_UNIFORM_BUFFER, generatedBlockBinding);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &camera->shader_data, sizeof(camera->shader_data));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
}

void RenderingSystem::initStandardShapes()
{
    fm::Model* quad = new fm::Model();
    quad->name = "Quad";
    fm::Model* quadFS = new fm::Model();
    quadFS->name = "QuadFS";
    fm::Model* circle = new fm::Model();
    circle->name = "Circle";
    quad->meshContainer = fm::StandardShapes::CreateQuad();
    circle->meshContainer = fm::StandardShapes::CreateCircle();
    quadFS->meshContainer = fm::StandardShapes::CreateQuadFullScreen();

    quad->generate();
    circle->generate();
    quadFS->generate();
    fm::ResourcesManager::get().load<fm::Model>(quad->name, quad);
    fm::ResourcesManager::get().load<fm::Model>(quadFS->name, quadFS);

    fm::ResourcesManager::get().load<fm::Model>(circle->name, circle);
    fm::Renderer::getInstance().createQuadScreen();
}

void RenderingSystem::init(EntityManager& em, EventManager& event)
{
    fm::Debug::log("INIT Standard Shapes");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << width << " " << height << std::endl;

    initStandardShapes();

    for(auto e : em.iterate<fmc::CMesh>()) {
        fmc::CMesh* mesh = e->get<fmc::CMesh>();

        mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
    }

    for(auto e : em.iterate<fmc::CMaterial>())
    {
        fmc::CMaterial* material = e->get<fmc::CMaterial>();
        if(material->shader == nullptr && material->shaderName != "")
        {
            material->shader =
                    fm::ResourcesManager::get().getResource<fm::Shader>(
                        material->shaderName);
        }
        if(material->shader != nullptr && !material->shader->IsReady())
        {
            material->shader->compile();
        }
    }
    //If no camera found the first one from the entities
    if(!camera)
    {
        fm::Debug::log("INIT MainCamera");
        for(auto e : em.iterate<fmc::CCamera>()) {
           setCamera(e);
           break;
        }
    }

}

void RenderingSystem::setCamera(Entity* camera)
{
    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    cam->_viewMatrix = fm::math::mat();
    setView(cam->_viewMatrix, ct->position, {cam->viewPort.w, cam->viewPort.h}, ct->rotation);
    this->camera = camera;
    //#if OPENGL_ES_VERSION > 2
    // initUniformBufferCamera(cam);
    //#endif
    fm::Format formats[] = {fm::Format::RGBA, fm::Format::RGBA};
    fm::Type types[] = {fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE};
    lightRenderTexture = std::make_shared<fm::RenderTexture>(
                cam->_renderTexture->getWidth(),
                cam->_renderTexture->getHeight(),
                2,
                formats,
                types,
                0);
}

void RenderingSystem::setView(fm::math::mat& viewMatrix,
                              const fm::math::Vector2f& position,
                              const fm::math::Vector2f& size,
                              float rotation) {
    viewMatrix = fm::math::translate(viewMatrix,fm::math::vec3(position.x, position.y, 0));
    viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    viewMatrix = fm::math::rotate(viewMatrix, rotation, fm::math::vec3(0, 0, 1));
    viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
}

void RenderingSystem::pre_update(EntityManager& em)
{
    if(camera == nullptr) return;

    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    if(!lightRenderTexture->isCreated()) lightRenderTexture->create();
    if(!cam->_renderTexture->isCreated()) cam->_renderTexture->create();

    cam->UpdateRenderTexture();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    bounds.setSize(fm::math::vec3(cam->viewPort.w,
                                  cam->viewPort.h,
                                  cam->GetFarPlane() - cam->GetNearPlane()));
    bounds.setCenter(fm::math::vec3(ct->position.x, ct->position.y, -1) +
                     bounds.getSize() / 2.0f);
    bounds.setScale(fm::math::vec3(1, 1, 1));
    setView(cam->_viewMatrix, ct->position, {cam->viewPort.w, cam->viewPort.h},ct->rotation);

}

void RenderingSystem::update(float dt, EntityManager& em, EventManager& event)
{
    assert(camera != nullptr);
    // fm::Renderer::getInstance().bindFrameBuffer();
    fmc::CCamera* cam = camera->get<fmc::CCamera>();

    if(!cam->_renderTexture->isCreated())
    {
        std::cout << "No render texture created" << std::endl;
        return;
    }
    lightRenderTexture->bind();
    graphics.clear(true, true);

    cam->_renderTexture->bind();
    graphics.setViewPort(cam->viewPort);
    graphics.clear(true, true);

    cam->shader_data.FM_PV = cam->projection * cam->_viewMatrix;
    cam->shader_data.FM_P = cam->projection;
    cam->shader_data.FM_V = cam->_viewMatrix;
    //#if OPENGL_ES_VERSION > 2
    // updateUniformBufferCamera(cam);
    //#endif
    // PROFILER_START(Rendering)
    // PROFILER_START(RenderingSort)

    fillQueue(em);

    if(!queue.empty()) {
        queue.start();
        // PROFILER_STOP(RenderingSort)
        // PROFILER_START(Draw)
        draw(cam);
    }

    // PROFILER_STOP(Draw)
    // PROFILER_STOP(Rendering)
    // PROFILER_DISPLAY(Rendering)
    // PROFILER_DISPLAY(RenderingSort)
    // PROFILER_DISPLAY(Draw)

    if(cam->target != nullptr)
    {
        if(!cam->target->isCreated())
        {
            cam->target->create();
        }
        cam->target->bind();
    }
    else
    {
        graphics.bindFrameBuffer(0);
    }
    graphics.setViewPort(cam->viewPort);
    graphics.clear(true, true);

    finalShader->Use();
    finalShader->Use()->setValue("screenSize", fm::math::vec2(cam->viewPort.w, cam->viewPort.h));

    finalShader->setValue("viewPos", camera->get<fmc::CTransform>()->position);

    fm::Renderer::getInstance().SetSources(graphics, lightRenderTexture->getColorBuffer(), 2);
    if(cam->target != nullptr)
    {
        fm::Renderer::getInstance().blit(graphics, *cam->target.get(), finalShader);
    }
    else
    {
        fm::Renderer::getInstance().blit(graphics, finalShader);
    }
    if(cam->target != nullptr)
    {
        graphics.bindFrameBuffer(0);
        graphics.setViewPort(cam->viewPort);
        graphics.clear(true, true);
    }

}

void RenderingSystem::draw(fmc::CCamera* cam)
{
    int lightNumber = 0;
    bool hasLight = false;
    bool computeLightDone = false;

    while(!queue.empty()) {
        fm::RenderNode node = queue.getFrontElement();
        fmc::CTransform* transform = node.transform;
        fmc::CMaterial* material = node.mat;
        fmc::CMesh* mesh = node.mesh;
        fm::math::Vector2f worldPos = transform->getWorldPos();
        EventManager::get().emit<CameraInfo>({node.idEntity, cam});

        fm::RENDER_QUEUE state = node.state;

        if(state == fm::RENDER_QUEUE::OPAQUE) {
            if(mesh)
            {

                if(mesh->model == nullptr || mesh->model->name.compare(mesh->GetModelType()) != 0)
                {
                    mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
                }

                if(material->shader == nullptr && material->shaderName != "")
                {
                    material->shader =
                            fm::ResourcesManager::get().getResource<fm::Shader>(
                                material->shaderName);
                }
                if(material->shader != nullptr &&!material->shader->IsReady())
                {
                    material->shader->compile();
                }
                fm::Shader* shader = material->shader;
                if(shader != nullptr)
                {
                    shader->Use()
                          ->setValue("FM_PV", cam->shader_data.FM_PV)
                          ->setValue("BloomEffect", material->bloom);

                    fm::math::mat model = fm::math::mat();
                    setModel(model, transform, worldPos);

                    shader->setValue("FM_PVM", cam->shader_data.FM_PV * model);
                    shader->setValue("FM_M", model)
                          ->setValue("mainColor", material->color);
                    if(material->textureReady)
                    {
                        glActiveTexture(GL_TEXTURE0);
                        material->getTexture().bind();
                    }

                    for(auto const& value : material->getValues()) {
                        shader->setValue(value.first, value.second);
                    }
                    graphics.draw(mesh->model);
                }
            }
        }

        // Set the lights
        if(state == fm::RENDER_QUEUE::LIGHT) {
            if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                if(node.plight) {
                    hasLight = true;
                    std::string ln =
                            "light[" + std::to_string(lightNumber) + "]";

                    lightShader->Use()
                            ->setValue(
                                ln + ".position",
                                fm::math::vec3(
                                    worldPos.x, worldPos.y, transform->layer))
                            ->setValue(ln + ".color", node.plight->color)
                            ->setValue(ln + ".ready", 1);
                    lightNumber++;
                }

                if(node.dlight) {
                    hasLight = true;
                    lightShader->Use()->setValue("dlight.color",
                                                 node.dlight->color);
                }
            }
        }

        // After all lights, we compute the frame buffer
        if(state >= fm::RENDER_QUEUE::AFTER_LIGHT &&
                queuePreviousValue < fm::RENDER_QUEUE::AFTER_LIGHT) {
            computeLighting(lightRenderTexture, cam, hasLight);
            computeLightDone = true;
        }

        if(state == fm::RENDER_QUEUE::TRANSPARENT) {
            if(!blendingMode) {
                blendingMode = true;

                graphics.enable(fm::RENDERING_TYPE::BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            // TODO draw
        }

        if(state == fm::RENDER_QUEUE::OVERLAY) {
            if(node.text) {
                if(!blendingMode) {
                    blendingMode = true;

                    graphics.enable(fm::RENDERING_TYPE::BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }

                fm::Shader* shader =
                        fm::ResourcesManager::get().getResource<fm::Shader>("text");
                shader->Use()
                        ->setValue("projection", textdef.projection)
                        ->setValue("textColor", material->color);

                drawText(worldPos.x,
                         worldPos.y,
                         fm::ResourcesManager::get().getResource<RFont>(
                             node.text->fontName),
                         node.text);
            }
        }
        queuePreviousValue = state;
        queue.next();
    }

    if(blendingMode) {
        graphics.disable(fm::RENDERING_TYPE::BLEND);
    }
    blendingMode = false;

    if(!computeLightDone) {
        computeLighting(lightRenderTexture, cam, hasLight);
    }
}

void RenderingSystem::computeLighting(
        std::shared_ptr<fm::RenderTexture> lightRenderTexture,
        fmc::CCamera* cam,
        bool hasLight) {
    lightRenderTexture->bind();
    graphics.setViewPort(cam->viewPort);

    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
        fm::Renderer::getInstance().lightComputation(
                    graphics, cam->_renderTexture->getColorBuffer(), hasLight);
    } else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
        fm::Renderer::getInstance().lightComputation(
                    graphics, cam->_renderTexture->getColorBuffer(), false);
    }
}

void RenderingSystem::fillQueue(EntityManager& em)
{
    queue.init();
    // int i = 0;
    for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial>())
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
            node.mesh->bounds.setCenter(
                        fm::math::vec3(node.transform->position) +
                        fm::math::vec3(node.transform->scale) / 2.0f);
            node.mesh->bounds.setScale(fm::math::vec3(node.transform->scale));
            if(!node.mesh->bounds.intersects(bounds))
                continue;
            valid = true;
            node.state = fm::RENDER_QUEUE::OPAQUE;
        }
        if(node.dlight || node.plight)
        {
            node.state = fm::RENDER_QUEUE::LIGHT;
            valid = true;
        }
        if(node.text)
        {
            node.state = fm::RENDER_QUEUE::OVERLAY;
            valid = true;
        }
        node.queue = 0;
        if(!valid)
        {
            continue;
        }
        queue.addElement(node);
    }
}

void RenderingSystem::over()
{
}

void RenderingSystem::setModel(fm::math::mat& model,
                               fmc::CTransform* transform,
                               const fm::math::Vector2f& worldPos)
{
    model = fm::math::translate( model, fm::math::vec3(worldPos.x, worldPos.y, -transform->layer));
    model = fm::math::translate( model,fm::math::vec3( 0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
    model = fm::math::rotate( model, transform->rotation, fm::math::vec3(0.0f, 0.0f, 1.0f));
    model = fm::math::translate( model, fm::math::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
    model = fm::math::scale(model, fm::math::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::drawText(int posX, int posY,
                               RFont* font,
                               fmc::CText* ctext)
{
    if(font == nullptr || ctext == nullptr) {
        std::cout << "Font not found" << std::endl;
        return;
    }

    if(ctext->buffer == nullptr) {
        ctext->buffer = new fm::rendering::VertexBuffer();
        ctext->buffer->generate();
    }

    font->texture->bind();

    if(!(ctext->text.compare(ctext->previousText) == 0)) {
        fm::math::vec4 coords[6 * ctext->text.size()];
        float x = posX;
        float y = posY;
        std::string::const_iterator c;
        int n = 0;
        for(c = ctext->text.begin(); c != ctext->text.end(); c++) {
            Character ch = font->Characters[*c];
            float x2 = x + ch.b_lt.x * ctext->scale;
            float y2 = -y - ch.b_lt.y * ctext->scale;
            float w = ch.b_wh.x * ctext->scale;
            float h = ch.b_wh.y * ctext->scale;
            x += ch.advance.x * ctext->scale;
            y += ch.advance.y * ctext->scale;
            if(!w || !h)
                continue;

            coords[n++] = (fm::math::vec4){
                    x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y};
            coords[n++] = (fm::math::vec4){x2, -y2, ch.t.x, ch.t.y};
            coords[n++] = (fm::math::vec4){
                    x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height};
            coords[n++] = (fm::math::vec4){
                    x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y};
            coords[n++] = (fm::math::vec4){
                    x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height};
            coords[n++] =
                    (fm::math::vec4){x2 + w,
                    -y2 - h,
                    ch.t.x + ch.b_wh.x / font->atlas_width,
                    ch.t.y + ch.b_wh.y / font->atlas_height};
        }
        ctext->previousText = ctext->text;
        ctext->buffer->setBufferData(&coords[0], n, sizeof(float) * 4, false);
    }
    if(ctext->buffer != NULL && ctext->buffer->isGenerated()) {
        graphics.setVertexBuffer(ctext->buffer);
        graphics.draw(0, 0, ctext->buffer->numberVertices);
    }
}

}  // namespace fms
