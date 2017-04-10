#include "Rendering/RenderingSystem.h"
#include "Components/CTransform.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
using namespace fms;
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Components/CMaterial.h"
#include "Components/CPointLight.h"
#include "Components/CText.h"
#include "Components/CMesh.h"
#include <chrono>
#include <Components/CDirectionalLight.h>
#include "Window.h"

#include "Profiler/Profile.hpp"
#include "Profiler/Profiler.hpp"
#include "Profiler/ProfilerMacro.h"

#include "Core/Math/Matrix.h"
#include "Core/Math/Vector3.h"

struct PointText {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};
RenderingSystem::RenderingSystem(int width, int height)
    : width(width)
    , height(height) {

    finalShader = fm::ResourcesManager::get().getShader("simple");
    lightShader = fm::ResourcesManager::get().getShader("light");

    textdef.projection = fm::math::ortho(0.0f, (float)fm::Window::width, 0.0f, (float)fm::Window::height);

    glGenVertexArrays(1, &textdef.VAO);
    glGenBuffers(1, &textdef.VBO);
    glBindVertexArray(textdef.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    fm::Renderer::getInstance().createQuadScreen();
}

void RenderingSystem::initUniformBufferCamera(fmc::CCamera* camera) {
    glGenBuffers(1, &generatedBlockBinding);
    glBindBuffer(GL_UNIFORM_BUFFER, generatedBlockBinding);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(camera->shader_data), &camera->shader_data, GL_DYNAMIC_COPY);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(auto shader : fm::ResourcesManager::get().getAllShaders()) {
        shader.second->Use();
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointIndex, generatedBlockBinding);
        glUniformBlockBinding(
            shader.second->Program, glGetUniformBlockIndex(shader.second->Program, "shader_data"), bindingPointIndex);
    }
}

RenderingSystem::~RenderingSystem() {
}

void RenderingSystem::updateUniformBufferCamera(fmc::CCamera* camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, generatedBlockBinding);
    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &camera->shader_data, sizeof(camera->shader_data));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void RenderingSystem::init(EntityManager& em, EventManager& event) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << width << " " << height << std::endl;
}

void RenderingSystem::setCamera(Entity* camera) {
    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    cam->viewMatrix = fm::math::mat();
    view(cam->viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
    this->camera = camera;

    initUniformBufferCamera(cam);
    fm::Format formats[] = {fm::Format::RGBA, fm::Format::RGBA};
    fm::Type types[] = {fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE};
    lightRenderTexture = std::make_shared<fm::RenderTexture>(cam->getRenderTexture()->getWidth(),
                                                            cam->getRenderTexture()->getHeight(), 
                                                            2 , 
                                                            formats,
                                                            types,
                                                            0);
    
}


void
RenderingSystem::view(fm::math::mat& viewMatrix, const fm::math::Vector2f& position, const fm::math::Vector2f& size, float rotation) {
    viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(position.x, position.y, 0));

    viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    viewMatrix = fm::math::rotate(viewMatrix, rotation, fm::math::vec3(0, 0, 1));
    viewMatrix = fm::math::translate(viewMatrix, fm::math::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
}

void RenderingSystem::pre_update(EntityManager& em) {
    fmc::CCamera* cam = camera->get<fmc::CCamera>();

    cam->updateRenderTexture();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    //cam->viewMatrix = glm::mat4();
    fm::math::mat m;
    view(m, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
    cam->viewMatrix = m;
}

void RenderingSystem::update(float dt, EntityManager& em, EventManager& event) {
    // fm::Renderer::getInstance().bindFrameBuffer();
    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    
    if(!cam->getRenderTexture()->isCreated()) {
        std::cout << "No render texture created" << std::endl;
        return;
    }

    cam->getRenderTexture()->active();
    glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->shader_data.FM_PV = cam->projection * cam->viewMatrix;
    cam->shader_data.FM_P = cam->projection;
    cam->shader_data.FM_V = cam->viewMatrix;
    updateUniformBufferCamera(cam);
    
    //PROFILER_START(Rendering)
    //PROFILER_START(RenderingSort)

    queue.init();
    for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial>()) {
        fm::RenderNode node = { e->get<fmc::CTransform>(),        e->get<fmc::CMaterial>(),   e->get<fmc::CMesh>(),
                                e->get<fmc::CDirectionalLight>(), e->get<fmc::CPointLight>(), e->get<fmc::CText>(),
                                fm::RENDER_QUEUE::BACKGROUND };
        if(node.dlight || node.plight) {
            node.state = fm::RENDER_QUEUE::LIGHT;
        }
        if(node.text) {
            node.state = fm::RENDER_QUEUE::OVERLAY;
        }
        node.queue = 0;
        queue.addElement(node);
    }
    int lightNumber = 0;
    queue.start();
    //PROFILER_STOP(RenderingSort)
    //PROFILER_START(Draw)

    while(!queue.empty()) {
        fm::RenderNode node = queue.getFrontElement();
        fmc::CTransform* transform = node.transform;
        fmc::CMaterial* material = node.mat;
        fmc::CMesh* mesh = node.mesh;
        fm::math::Vector2f worldPos = transform->getWorldPos(em);

        int q = node.queue;
        fm::RENDER_QUEUE state = node.state;
        
        
        if(state < fm::RENDER_QUEUE::LIGHT) {
            if(mesh) {

                std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader(material->shaderName);

                shader->Use()->setMatrix("FM_PV", cam->shader_data.FM_PV)->setInt("BloomEffect", material->bloom);

                fm::math::mat model = fm::math::mat();
                setModel(model, transform, worldPos);
      
                shader->setMatrix("FM_PVM", cam->shader_data.FM_PV * model);
                shader->setMatrix("FM_M", model)->setColor("mainColor", material->color);

                if(material->textureReady) {
                    glActiveTexture(GL_TEXTURE0);
                    material->getTexture().bind();
                }

                draw(mesh);
            }
        }
        if(state >= fm::RENDER_QUEUE::LIGHT && state < fm::RENDER_QUEUE::AFTER_LIGHT) {
            if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                if(node.plight) {
                    std::string ln = "light[" + std::to_string(lightNumber) + "]";

                    lightShader->Use()
                        ->setVector3f(ln + ".position", fm::math::vec3(worldPos.x, worldPos.y, transform->layer))
                        ->setColor(ln + ".color", node.plight->color)
                        ->setInt(ln + ".ready", 1);
                    lightNumber++;
                }

                if(node.dlight) {

                    lightShader->Use()->setColor("dlight.color", node.dlight->color);
                }
            }
        }

//After all lights, we compute the frame buffer
        if(state >= fm::RENDER_QUEUE::AFTER_LIGHT && queuePreviousValue < fm::RENDER_QUEUE::AFTER_LIGHT) {
            if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
                lightRenderTexture->bind();
                glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

                fm::Renderer::getInstance().lightComputation(cam->getRenderTexture()->getColorBuffer());
            }
        }

        if((state >= fm::RENDER_QUEUE::TRANSPARENT && queuePreviousValue < fm::RENDER_QUEUE::TRANSPARENT)
|| (state >= fm::RENDER_QUEUE::OVERLAY && queuePreviousValue < fm::RENDER_QUEUE::OVERLAY)){
            blendingMode = true;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            // TODO draw
        }

        if(state >= fm::RENDER_QUEUE::OVERLAY) {
            if(node.text) {

                std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader("text");
                shader->Use();
                shader->setInt("outline", node.text->outline)
                    ->setVector2f("outline_min", node.text->outline_min)
                    ->setVector2f("outline_max", node.text->outline_max)
                    ->setVector3f(
                          "outline_color",
                          fm::math::vec3(node.text->outline_color.r, node.text->outline_color.g, node.text->outline_color.b))
                    ->setMatrix("projection", textdef.projection)
                    ->setColor("textColor", material->color)
                    ->setInt("soft_edges", node.text->soft_edges)
                    ->setVector2f("soft_edge_values", node.text->soft_edge_values);

                drawText(worldPos.x,
                         worldPos.y,
                         fm::ResourcesManager::get().getResource<RFont>(node.text->fontName).get(),
                         node.text);
            }
        }
        queuePreviousValue = state;
        queue.next();
    }
    //PROFILER_STOP(Draw)
    //PROFILER_STOP(Rendering)
    //PROFILER_DISPLAY(Rendering)
    //PROFILER_DISPLAY(RenderingSort)
    //PROFILER_DISPLAY(Draw)
    
    glDisable(GL_BLEND);
    blendingMode = false;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

    finalShader->Use();
    finalShader->Use()->setVector2f("screenSize", fm::math::vec2(cam->viewPort.width, cam->viewPort.height));

    finalShader->setVector2f("viewPos", camera->get<fmc::CTransform>()->position);
    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
        fm::Renderer::getInstance().SetSources(lightRenderTexture->getColorBuffer(), 2);
        //fm::RenderTexture rt = fm::RenderTexture(cam->viewPort.width, cam->viewPort.height, 1);
        
        fm::Renderer::getInstance().blit(finalShader);
    } else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
        fm::Renderer::getInstance().SetSources(lightRenderTexture->getColorBuffer(), 2);
        fm::Renderer::getInstance().blit(finalShader);
    }
}

void RenderingSystem::over() {
}



void RenderingSystem::setModel(fm::math::mat& model, fmc::CTransform* transform, const fm::math::Vector2f& worldPos) {
    model = fm::math::translate(model, fm::math::vec3(worldPos.x, worldPos.y, -transform->layer));
    model = fm::math::translate(model, fm::math::vec3(0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
    model = fm::math::rotate(model, transform->rotation, fm::math::vec3(0.0f, 0.0f, 1.0f));
    model = fm::math::translate(model, fm::math::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
    model = fm::math::scale(model, fm::math::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::drawText(int posX, int posY, RFont* font, const fmc::CText* ctext) {
    PointText coords[6 * ctext->text.size()];

    float x = posX;
    float y = posY;

    // shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->tex);
    glBindVertexArray(textdef.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);

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

        coords[n++] = (PointText){ x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y };
        coords[n++] = (PointText){ x2, -y2, ch.t.x, ch.t.y };

        coords[n++] = (PointText){ x2, -y2 - h, ch.t.x, ch.b_wh.y / font->atlas_height };
        coords[n++] = (PointText){ x2 + w, -y2, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y };
        coords[n++] = (PointText){ x2, -y2 - h, ch.t.x, ch.t.y + ch.b_wh.y / font->atlas_height };

        coords[n++] = (PointText){
            x2 + w, -y2 - h, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y + ch.b_wh.y / font->atlas_height
        };
    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, n);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::draw(const fmc::CMesh* cmesh) {

    glBindVertexArray(cmesh->VAO);
    glDrawElements(GL_TRIANGLES, cmesh->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}