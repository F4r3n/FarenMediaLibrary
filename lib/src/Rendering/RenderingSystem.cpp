#include <iostream>

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

struct PointText {
        GLfloat x;
        GLfloat y;
        GLfloat s;
        GLfloat t;
};

namespace fms {
RenderingSystem::RenderingSystem(int width, int height)
        : width(width), height(height) {
        finalShader = fm::ResourcesManager::get().getResource<fm::Shader>("simple");
        lightShader = fm::ResourcesManager::get().getResource<fm::Shader>("light");

        textdef.projection = fm::math::ortho(
                0.0f, (float)fm::Window::width, 0.0f, (float)fm::Window::height);

        //glGenVertexArrays(1, &textdef.VAO);
        glGenBuffers(1, &textdef.VBO);
        //glBindVertexArray(textdef.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        std::cout << "Create" << std::endl;

        fm::Renderer::getInstance().createQuadScreen();
}

void RenderingSystem::initUniformBufferCamera(fmc::CCamera* camera) {
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
}

RenderingSystem::~RenderingSystem() {
}

void RenderingSystem::updateUniformBufferCamera(fmc::CCamera* camera) {
#ifndef __EMSCRIPTEN__
        glBindBuffer(GL_UNIFORM_BUFFER, generatedBlockBinding);
        GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
        memcpy(p, &camera->shader_data, sizeof(camera->shader_data));
        glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
}

void RenderingSystem::initStandardShapes() {
        fm::Model *quad = new fm::Model();
        fm::Model* circle = new fm::Model();
        quad->meshContainer = fm::StandardShapes::CreateQuad();
        circle->meshContainer = fm::StandardShapes::CreateCircle();

        quad->generate();
        circle->generate();
        fm::ResourcesManager::get().load<fm::Model>("Quad", quad);
        fm::ResourcesManager::get().load<fm::Model>("Circle", circle);
}

void RenderingSystem::init(EntityManager& em, EventManager& event) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        std::cout << width << " " << height << std::endl;

        initStandardShapes();

        for(auto e : em.iterate<fmc::CMesh>()) {
                fmc::CMesh* mesh = e->get<fmc::CMesh>();

                mesh->model = fm::ResourcesManager::get().getResource<fm::Model>(
                        mesh->getModelType());
        }

        for(auto e : em.iterate<fmc::CMaterial>()) {
                fmc::CMaterial* material = e->get<fmc::CMaterial>();
                if(material->shader == nullptr && material->shaderName != "") {
                        material->shader =
                                fm::ResourcesManager::get().getResource<fm::Shader>(
                                        material->shaderName);
                }
                if(material->shader != nullptr && !material->shader->IsReady()) {
                        material->shader->compile();
                }
        }
}

void RenderingSystem::setCamera(Entity* camera) {
        fmc::CCamera* cam = camera->get<fmc::CCamera>();
        fmc::CTransform* ct = camera->get<fmc::CTransform>();
        cam->viewMatrix = fm::math::mat();
        view(cam->viewMatrix,
             ct->position,
             {cam->viewPort.w, cam->viewPort.h},
             ct->rotation);
        this->camera = camera;
#if OPENGL_ES_VERSION > 2
        initUniformBufferCamera(cam);
#endif
        fm::Format formats[] = {fm::Format::RGBA, fm::Format::RGBA};
        fm::Type types[] = {fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE};
        lightRenderTexture = std::make_shared<fm::RenderTexture>(
                cam->getRenderTexture()->getWidth(),
                cam->getRenderTexture()->getHeight(),
                2,
                formats,
                types,
                0);
}

void RenderingSystem::view(fm::math::mat& viewMatrix,
                           const fm::math::Vector2f& position,
                           const fm::math::Vector2f& size,
                           float rotation) {
        viewMatrix = fm::math::translate(viewMatrix,
                                         fm::math::vec3(position.x, position.y, 0));

        viewMatrix = fm::math::translate(
                viewMatrix, fm::math::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        viewMatrix =
                fm::math::rotate(viewMatrix, rotation, fm::math::vec3(0, 0, 1));
        viewMatrix = fm::math::translate(
                viewMatrix, fm::math::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
}

void RenderingSystem::pre_update(EntityManager& em) {
        fmc::CCamera* cam = camera->get<fmc::CCamera>();

        cam->updateRenderTexture();
        fmc::CTransform* ct = camera->get<fmc::CTransform>();
        bounds.setSize(fm::math::vec3(cam->viewPort.w,
                                      cam->viewPort.h,
                                      cam->getFarPlane() - cam->getNearPlane()));
        bounds.setCenter(fm::math::vec3(ct->position.x, ct->position.y, -1) +
                         bounds.getSize() / 2.0f);
        bounds.setScale(fm::math::vec3(1, 1, 1));
        fm::math::mat m;
        view(m, ct->position, {cam->viewPort.w, cam->viewPort.h}, ct->rotation);
        cam->viewMatrix = m;
}

void RenderingSystem::update(float dt, EntityManager& em, EventManager& event) {
        // fm::Renderer::getInstance().bindFrameBuffer();
        fmc::CCamera* cam = camera->get<fmc::CCamera>();

        if(!cam->getRenderTexture()->isCreated()) {
                std::cout << "No render texture created" << std::endl;
                return;
        }
        lightRenderTexture->bind();
        graphics.clear(true, true);

        cam->getRenderTexture()->bind();
        graphics.setViewPort(cam->viewPort);
        graphics.clear(true, true);

        cam->shader_data.FM_PV = cam->projection * cam->viewMatrix;
        cam->shader_data.FM_P = cam->projection;
        cam->shader_data.FM_V = cam->viewMatrix;
#if OPENGL_ES_VERSION > 2
        updateUniformBufferCamera(cam);
#endif
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

        graphics.bindFrameBuffer(0);
        graphics.setViewPort(cam->viewPort);
        graphics.clear(true, true);

        finalShader->Use();
        finalShader->Use()->setValue(
                "screenSize", fm::math::vec2(cam->viewPort.w, cam->viewPort.h));

        finalShader->setValue("viewPos",
                                 camera->get<fmc::CTransform>()->position);

        fm::Renderer::getInstance().SetSources(lightRenderTexture->getColorBuffer(),
                                               2);
        fm::Renderer::getInstance().blit(finalShader);
}

void RenderingSystem::draw(fmc::CCamera* cam) {
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
                        if(mesh) {
                                fm::Shader* shader = material->shader;
                                shader->Use()
                                ->setValue("FM_PV", cam->shader_data.FM_PV)
                                ->setValue("BloomEffect", material->bloom);

                                fm::math::mat model = fm::math::mat();
                                setModel(model, transform, worldPos);

                                shader->setValue("FM_PVM", cam->shader_data.FM_PV * model);
                                shader->setValue("FM_M", model)
                                ->setValue("mainColor", material->color);
                                if(material->textureReady) {
                                        glActiveTexture(GL_TEXTURE0);
                                        material->getTexture().bind();
                                }
                                
                                for(auto const& value : material->getValues()) {
                                    shader->setValue(value.first, value.second);
                                }
                                
                                graphics.setIndexBuffer(mesh->model->vertexBuffer);
                                graphics.draw(0,
                                              mesh->model->meshContainer->listIndices.size(),
                                              mesh->model->meshContainer->listIndices.data());
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
                                shader->Use();
                                shader->setValue("outline", node.text->outline)
                                ->setValue("outline_min", node.text->outline_min)
                                ->setValue("outline_max", node.text->outline_max)
                                ->setValue("outline_color",
                                              fm::math::vec3(node.text->outline_color.r,
                                                             node.text->outline_color.g,
                                                             node.text->outline_color.b))
                                ->setValue("projection", textdef.projection)
                                ->setValue("textColor", material->color)
                                ->setValue("soft_edges", node.text->soft_edges)
                                ->setValue("soft_edge_values",
                                              node.text->soft_edge_values);

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
                        cam->getRenderTexture()->getColorBuffer(), hasLight);
        } else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
                fm::Renderer::getInstance().lightComputation(
                        cam->getRenderTexture()->getColorBuffer(), false);
        }
}

void RenderingSystem::fillQueue(EntityManager& em) {
        queue.init();
        // int i = 0;
        for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial>()) {
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
                if (node.mesh) {
                        node.mesh->bounds.setCenter(
                                fm::math::vec3(node.transform->position) +
                                fm::math::vec3(node.transform->scale) / 2.0f);
                        node.mesh->bounds.setScale(fm::math::vec3(node.transform->scale));
                        if (!node.mesh->bounds.intersects(bounds))
                                continue;
                        valid = true;
                        node.state = fm::RENDER_QUEUE::OPAQUE;
                }
                if (node.dlight || node.plight) {
                        node.state = fm::RENDER_QUEUE::LIGHT;
                        valid = true;
                }
                if (node.text) {
                        node.state = fm::RENDER_QUEUE::OVERLAY;
                        valid = true;
                }
                node.queue = 0;
                if (!valid) {
                        continue;
                }
                queue.addElement(node);
        }
}

void RenderingSystem::over() {
}

void RenderingSystem::setModel(fm::math::mat& model,
                               fmc::CTransform* transform,
                               const fm::math::Vector2f& worldPos) {
        model = fm::math::translate(
                model, fm::math::vec3(worldPos.x, worldPos.y, -transform->layer));
        model = fm::math::translate(
                model,
                fm::math::vec3(
                        0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
        model = fm::math::rotate(
                model, transform->rotation, fm::math::vec3(0.0f, 0.0f, 1.0f));
        model = fm::math::translate(
                model,
                fm::math::vec3(
                        -0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
        model = fm::math::scale(
                model, fm::math::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::drawText(int posX,
                               int posY,
                               RFont* font,
                               const fmc::CText* ctext) {
        if (font == nullptr || ctext == nullptr) {
                // std::cout << "Font not found" << std::endl;
                return;
        }
        PointText coords[6 * ctext->text.size()];

        float x = posX;
        float y = posY;

        // shader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->tex);
        //glBindVertexArray(textdef.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);
glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        // Iterate through all characters
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

                coords[n++] = (PointText){
                        x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y
                };
                coords[n++] = (PointText){x2, -y2, ch.t.x, ch.t.y};

                coords[n++] =
                        (PointText){x2, -y2 - h, ch.t.x, ch.b_wh.y / font->atlas_height};
                coords[n++] = (PointText){
                        x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y
                };
                coords[n++] = (PointText){
                        x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height
                };

                coords[n++] = (PointText){x2 + w,
                                          -y2 - h,
                                          ch.t.x + ch.b_wh.x / font->atlas_width,
                                          ch.t.y + ch.b_wh.y / font->atlas_height};
        }
        //std::cout << n << std::endl;
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
        //glDrawArrays(GL_TRIANGLES, 0, n);
        graphics.draw(0, 0, n);
        //glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}





} // namespace fms
