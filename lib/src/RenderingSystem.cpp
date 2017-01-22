#include "RenderingSystem.h"
#include "Components/CTransform.h"
#include "ResourcesManager.h"
#include <iostream>
using namespace fms;
#include "Renderer.h"
#include "Shader.h"
#include "Components/CMaterial.h"
#include "Components/CPointLight.h"
#include "Components/CText.h"
#include "Components/CMesh.h"
#include <chrono>
#include <Components/CDirectionalLight.h>
#include "Window.h"

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
    
    textdef.projection = glm::ortho(0.0f, (float)fm::Window::width, 0.0f, (float)fm::Window::height);

    glGenVertexArrays(1, &textdef.VAO);
    glGenBuffers(1, &textdef.VBO);
    glBindVertexArray(textdef.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void RenderingSystem::initUniformBufferCamera(fmc::CCamera *camera) {
    glGenBuffers(1, &gbo);
    glBindBuffer(GL_UNIFORM_BUFFER, gbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(camera->shader_data), &camera->shader_data, GL_DYNAMIC_COPY);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

 for( auto shader : fm::ResourcesManager::get().getAllShaders()) {
        shader.second->Use();
        unsigned int block_index = glGetUniformBlockIndex(shader.second->Program, "shader_data");
        GLuint binding_point_index = 2;
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point_index, gbo);
        glUniformBlockBinding(shader.second->Program, block_index, binding_point_index);
    }
}

RenderingSystem::~RenderingSystem() {
}

void RenderingSystem::updateUniformBufferCamera(fmc::CCamera *camera) {
    glBindBuffer(GL_UNIFORM_BUFFER, gbo);
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
    cam->viewMatrix = glm::mat4();
    view(cam->viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
    this->camera = camera;

    initUniformBufferCamera(cam);

}

void
RenderingSystem::view(glm::mat4& viewMatrix, const fm::Vector2f& position, const fm::Vector2f& size, float rotation) {
    viewMatrix = glm::translate(viewMatrix, glm::vec3(position.x, position.y, 0));

    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    viewMatrix = glm::rotate(viewMatrix, rotation, glm::vec3(0, 0, 1));
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
}

void RenderingSystem::pre_update(EntityManager& em) {
    start = std::chrono::system_clock::now();
    
    fmc::CCamera* cam = camera->get<fmc::CCamera>();

    cam->updateRenderTexture();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    cam->viewMatrix = glm::mat4();
    view(cam->viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
        // cameras.push_back(camera);

}

void RenderingSystem::update(float dt, EntityManager& em, EventManager& event) {
    //fm::Renderer::getInstance().bindFrameBuffer();
    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    if(!cam->getRenderTexture().isCreated()) return;
    
    
    cam->getRenderTexture().active();
    glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam->shader_data.FM_PV = cam->projection * cam->viewMatrix;
    cam->shader_data.FM_P = cam->projection;
    cam->shader_data.FM_V = cam->viewMatrix;
    updateUniformBufferCamera(cam);
    
    int lightNumber = 0;
    for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial>()) {
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        fmc::CMaterial* material = e->get<fmc::CMaterial>();
        fm::Vector2f worldPos = transform->getWorldPos(em);
        

        if(e->has<fmc::CMesh>()) {
            fmc::CMesh* cmesh = e->get<fmc::CMesh>();

            std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader(material->shaderName);
            //std::cout << material->shaderName << std::endl;
            shader->Use()->setMatrix("FM_PV", cam->shader_data.FM_PV)->setInt("BloomEffect", material->bloom);
            glm::mat4 model = glm::mat4();
            setModel(model, transform, worldPos);
            shader->setMatrix("FM_PVM", cam->shader_data.FM_PV*model);
            shader->setMatrix("FM_M", model)->setColor("mainColor", material->color);
            
            if(material->textureReady) {
                glActiveTexture(GL_TEXTURE0);
                // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                material->getTexture().bind();
            }

            draw(cmesh);
        }

        
    
        if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
            if(e->has<fmc::CPointLight>()) {
                    std::string ln = "light[" + std::to_string(lightNumber) + "]";
    
                    lightShader->Use()
                        ->setVector3f(ln + ".position", glm::vec3(worldPos.x, worldPos.y, transform->layer))
                        ->setColor(ln + ".color", e->get<fmc::CPointLight>()->color)
                        ->setInt(ln + ".ready", 1);
                    lightNumber++;
                }
    
                if(e->has<fmc::CDirectionalLight>()) {
    
                    lightShader->Use()->setColor("dlight.color", e->get<fmc::CDirectionalLight>()->color);
                }
            
        }
    }
    
    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
        glBindFramebuffer(GL_FRAMEBUFFER, cam->getRenderTexture().getLightBuffer());
        glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

        fm::Renderer::getInstance().lightComputation(cam->getRenderTexture().getColorBuffer(), 
                                                     cam->getRenderTexture().getLightBuffer());
    }
    
                                                 
    


    for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial, fmc::CText>()) {
        fmc::CTransform* transform = e->get<fmc::CTransform>();
        fmc::CMaterial* material = e->get<fmc::CMaterial>();

        fmc::CText* text = e->get<fmc::CText>();
        fm::Vector2f worldPos = transform->getWorldPos(em);
        std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader("text");
        
        shader->Use();
        shader->setInt(   "outline",          text->outline)
            ->setVector2f("outline_min",      text->outline_min)
            ->setVector2f("outline_max",      text->outline_max)
            ->setVector3f("outline_color",    glm::vec3(text->outline_color.r, text->outline_color.g, text->outline_color.b))
            ->setMatrix(  "projection",       textdef.projection)
            ->setColor(   "textColor",        material->color)
            ->setInt(     "soft_edges",       text->soft_edges)
            ->setVector2f("soft_edge_values", text->soft_edge_values);

        drawText(worldPos.x, worldPos.y, fm::ResourcesManager::get().getResource<RFont>(text->fontName).get(), text);
    }

    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(cam->viewPort.x, cam->viewPort.y, cam->viewPort.width, cam->viewPort.height);

    //std::cout << "yop" << std::endl;
    finalShader->Use();
    finalShader->Use()->setVector2f("screenSize", glm::vec2(cam->viewPort.width, cam->viewPort.height));
    
    finalShader->setVector2f("viewPos", camera->get<fmc::CTransform>()->position);
    if(cam->shader_data.render_mode == fmc::RENDER_MODE::DEFERRED) {
        fm::Renderer::getInstance().postProcess(cam->getRenderTexture().getLightTextures());
    } else if(cam->shader_data.render_mode == fmc::RENDER_MODE::FORWARD) {
        fm::Renderer::getInstance().postProcess(cam->getRenderTexture().getColorBuffer());
    }
    
}

void RenderingSystem::over() {

    end = std::chrono::system_clock::now();
    elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    timer += elapsed_seconds.count();
    // std::cout << "Time rendering " << elapsed_seconds.count() << " " <<timer/(double)frame << std::endl;
    if(frame == 100) {
        std::cout << timer << " " << frame << " " << timer / (double)frame << std::endl;
        frame = 0;
        timer = 0;
    }
    frame++;
}

void RenderingSystem::setModel(glm::mat4& model, fmc::CTransform* transform, const fm::Vector2f& worldPos) {
    model = glm::translate(model, glm::vec3(worldPos.x, worldPos.y, -transform->layer));
    model = glm::translate(model, glm::vec3(0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
    model = glm::rotate(model, transform->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
    model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::drawText(int posX, int posY, RFont* font, const fmc::CText* ctext) {
    PointText coords[6*ctext->text.size()];
    
    
    float x = posX;
    float y = posY;

    // shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->tex);
    glBindVertexArray(textdef.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, textdef.VBO);
   

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        coords[n++] =
            (PointText){ x2 + w, -y2 - h, ch.t.x + ch.b_wh.x / font->atlas_width, ch.t.y + ch.b_wh.y / font->atlas_height };
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