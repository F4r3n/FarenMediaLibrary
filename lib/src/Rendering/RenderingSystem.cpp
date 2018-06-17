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
const int NUMBER_POINTLIGHT_MAX = 8;
struct PointLight
{
   fm::math::vec4 position;
   fm::math::vec4 color;
   fm::math::vec4 custom;

};

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
    fm::Model* cube = new fm::Model();
    cube->name = "Cube";
    quad->meshContainer = fm::StandardShapes::CreateQuad();
    circle->meshContainer = fm::StandardShapes::CreateCircle();
    quadFS->meshContainer = fm::StandardShapes::CreateQuadFullScreen();
    cube->meshContainer = fm::StandardShapes::CreateCube();


    quad->generate();
    circle->generate();
    quadFS->generate();
    cube->generate();
    fm::ResourcesManager::get().load<fm::Model>(quad->name, quad);
    fm::ResourcesManager::get().load<fm::Model>(quadFS->name, quadFS);
    fm::ResourcesManager::get().load<fm::Model>(circle->name, circle);
    fm::ResourcesManager::get().load<fm::Model>(cube->name, cube);

    fm::Renderer::getInstance().createQuadScreen();

    uboLight = std::make_unique<fm::UniformBuffer>(fm::UniformBuffer());
    uboLight->Generate(sizeof(PointLight)*NUMBER_POINTLIGHT_MAX, 2);
}

void RenderingSystem::init(EntityManager& em, EventManager& event)
{
    fm::Debug::log("INIT Standard Shapes");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        for(auto e : em.iterate<fmc::CCamera>())
        {
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
    this->camTransform = ct;
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

    intermediate = std::make_shared<fm::RenderTexture>(
                cam->_renderTexture->getWidth(),
                cam->_renderTexture->getHeight(),
                2,
                formats,
                types,
                0);
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

void RenderingSystem::setView(fm::math::mat& viewMatrix,
                              const fm::math::Vector3f& position,
                              const fm::math::Vector2f& size,
                              const fm::math::Vector3f& rotation)
{
    if(camera == nullptr) return;
    fmc::CCamera *cam = camera->get<fmc::CCamera>();
    if(cam->IsOrthographic())
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


        //fm::math::vec3 front;
        //fm::math::vec3 right;
        //fm::math::vec3 up;
        //
        //UpdateCameraVectors(rotation.x, rotation.y, rotation.z, front, right, up, fm::math::vec3(0.0f,1.0f,0.0f));
        //viewMatrix = fm::math::lookAt(position, position + front, up);
        //std::cout << viewMatrix << std::endl;
        //viewMatrix.identity();
        //TODO temporary
    }
}

void RenderingSystem::pre_update(EntityManager& em)
{
    assert(camera != nullptr);

    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    if(!lightRenderTexture->isCreated()) lightRenderTexture->create();
    if(!intermediate->isCreated()) intermediate->create();
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
        fm::Debug::logError("No render texture created");
        return;
    }
    lightRenderTexture->bind();
    graphics.clear(true, true);

    intermediate->bind();
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
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    fillQueue(em);

    if(!queue.Empty()) {
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
        finalShader->setValue("reverse", 1);
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
        graphics.clear(true, true, false);
    }
     fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

}

void RenderingSystem::draw(fmc::CCamera* cam)
{

    bool hasLight = false;
    bool computeLightDone = false;
    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
    //std::vector<PointLight> pointLights;

    //bool firstMesh = true;
    while(!queue.Empty())
    {
        //std::cout << queue.Size() << std::endl;

        fm::RenderNode node = queue.getFrontElement();
        fmc::CTransform* transform = node.transform;
        fmc::CMaterial* material = node.mat;
        fmc::CMesh* mesh = node.mesh;
        fm::math::Vector3f worldPos = transform->getWorldPos();
        //EventManager::get().emit<CameraInfo>({node.idEntity, cam});

        fm::RENDER_QUEUE state = node.state;

        if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
        {

            if(state == fm::RENDER_QUEUE::OPAQUE)
            {
                //if(firstMesh)
                //{
                    //size_t sizeToSet = pointLights.size() > NUMBER_POINTLIGHT_MAX ? NUMBER_POINTLIGHT_MAX : pointLights.size();
                    //uboLight->SetData(pointLights.data(), sizeof(PointLight)*sizeToSet);
                    //firstMesh = false;
                //}
                if(mesh && material)
                {
                    if(mesh->model == nullptr || mesh->model->name.compare(mesh->GetModelType()) != 0)
                    {
                        mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(mesh->GetModelType());
                    }

                    if(material->Reload())
                    {
                        uboLight->Bind();
                        material->shader->SetUniformBuffer("PointLights", 2);
                        fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
                    }



                    if(material->shader != nullptr &&!material->shader->IsReady())
                    {
                        material->shader->compile();
                    }

                    fm::Shader* shader = material->shader;
                    //std::cout << (int)pointLights.size() << std::endl;
                    //if(pointLights.size() > 0)
                    //    std::cout << pointLights[0].color << std::endl;

                    if(shader != nullptr)
                    {
                        shader->Use()
                                ->setValue("FM_PV", cam->shader_data.FM_PV)
                                ->setValue("BloomEffect", material->bloom);
                        shader->setValue("lightNumber",lightNumber );
                        shader->setValue("viewPos", camTransform->getWorldPos());
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

            // After all lights, we compute the frame buffer
            if(state >= fm::RENDER_QUEUE::AFTER_LIGHT &&
                    queuePreviousValue < fm::RENDER_QUEUE::AFTER_LIGHT)
            {
                computeLighting(lightRenderTexture, cam, hasLight);
                computeLightDone = true;
            }

            if(state == fm::RENDER_QUEUE::TRANSPARENT)
            {
                if(!blendingMode)
                {
                    blendingMode = true;

                    graphics.enable(fm::RENDERING_TYPE::BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }
                // TODO draw
            }

            if(state == fm::RENDER_QUEUE::OVERLAY)
            {
                if(node.text)
                {
                    if(!blendingMode)
                    {
                        blendingMode = true;

                        graphics.enable(fm::RENDERING_TYPE::BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    }

                    fm::Shader* shader = fm::ResourcesManager::get().getResource<fm::Shader>("text");
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
        }else if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
        {

        }



    }

    if(blendingMode) {
        graphics.disable(fm::RENDERING_TYPE::BLEND);
    }
    blendingMode = false;

    if(!computeLightDone) {
        computeLighting(lightRenderTexture, cam, hasLight);
    }
 fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);
}

void RenderingSystem::computeLighting(
        std::shared_ptr<fm::RenderTexture> lightRenderTexture,
        fmc::CCamera* cam,
        bool hasLight) {

    //From MSAA
    glBindFramebuffer(GL_READ_FRAMEBUFFER, cam->_renderTexture->GetId());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate->GetId());
    glBlitFramebuffer(0, 0, cam->_renderTexture->getWidth(), cam->_renderTexture->getHeight(), 0, 0,
                      intermediate->getWidth(), intermediate->getHeight(),
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);


    lightRenderTexture->bind();
    graphics.setViewPort(cam->viewPort);
    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
    {
        fm::Renderer::getInstance().lightComputation(graphics, intermediate->getColorBuffer(), hasLight);
    } else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
    {
        fm::Renderer::getInstance().lightComputation(graphics, intermediate->getColorBuffer(), false);
    }

}

void RenderingSystem::fillQueue(EntityManager& em)
{
    queue.init();
    PointLight pointLights[NUMBER_POINTLIGHT_MAX];
    lightNumber = 0;
    for(auto e : em.iterate<fmc::CTransform>())
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
            node.mesh->bounds.setCenter(fm::math::vec3(node.transform->position) +
                                        fm::math::vec3(node.transform->scale) / 2.0f);
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
            pointLights[lightNumber] = p;
            lightNumber++;
        }

        node.queue = 0;
        if(!valid)
        {
            continue;
        }
        queue.addElement(node);
    }
    //std::cout << lightNumber*sizeof(PointLight) << std::endl;
    uboLight->SetData(&pointLights[0], lightNumber*sizeof(PointLight));
    //if(pointLights.size() > 0)
    //    std::cout <<pointLights[0].color << std::endl;
}

void RenderingSystem::over()
{
    fm::Debug::logErrorExit((int)glGetError(), __FILE__, __LINE__);

}

void RenderingSystem::setModel(fm::math::mat& model,
                               fmc::CTransform* transform,
                               const fm::math::Vector3f& worldPos)
{
    if(camera == nullptr) return;
    fmc::CCamera *cam = camera->get<fmc::CCamera>();
    if(cam->IsOrthographic())
    {
        model = fm::math::translate( model, fm::math::vec3(worldPos.x, worldPos.y, -transform->layer));
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
        //model.identity();

    }

}

void RenderingSystem::drawText(int posX, int posY,
                               RFont* font,
                               fmc::CText* ctext)
{
    if(font == nullptr || ctext == nullptr) {
        fm::Debug::logError("Font not found");
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
