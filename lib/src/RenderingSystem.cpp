#include "RenderingSystem.h"
#include "CTransform.h"
#include "ResourcesManager.h"
#include <iostream>
using namespace fms;
#include "Renderer.h"
#include "Shader.h"
#include "CMaterial.h"
#include "CPointLight.h"
#include "CText.h"
#include <chrono>
RenderingSystem::RenderingSystem(int width, int height)
    : width(width)
    , height(height) {
    addComponent<fmc::CMesh>();
    addComponent<fmc::CTransform>();
    addComponent<fmc::CMaterial>();

    finalShader = fm::ResourcesManager::get().getShader("simple");
    lightShader = fm::ResourcesManager::get().getShader("light");
}

RenderingSystem::~RenderingSystem() {
}

void RenderingSystem::init(EntityManager& em, EventManager& event) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::cout << width << " " << height << std::endl;
    finalShader->Use()->setVector2f("screenSize", glm::vec2(width, height));
}

void RenderingSystem::addCamera(Entity* camera) {
    fmc::CCamera* cam = camera->get<fmc::CCamera>();
    fmc::CTransform* ct = camera->get<fmc::CTransform>();
    cam->viewMatrix = glm::mat4();
    view(cam->viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
    cameras.push_back(camera);
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
    for(auto camera : cameras) {
        fmc::CCamera* cam = camera->get<fmc::CCamera>();
        fmc::CTransform* ct = camera->get<fmc::CTransform>();
        cam->viewMatrix = glm::mat4();
        view(cam->viewMatrix, ct->position, { cam->viewPort.width, cam->viewPort.height }, ct->rotation);
        // cameras.push_back(camera);
    }
}

void RenderingSystem::update(float dt, EntityManager& em, EventManager& event) {
    fm::Renderer::getInstance().bindFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto camera : cameras) {

        fmc::CCamera* cam = camera->get<fmc::CCamera>();
        glm::mat4 FM_PV = cam->projection * cam->viewMatrix;

        int lightNumber = 0;
        for(auto e : em.iterate<fmc::CTransform, fmc::CMaterial>()) {
            fmc::CTransform* transform = e->get<fmc::CTransform>();
            fmc::CMaterial* material = e->get<fmc::CMaterial>();

            if(e->has<fmc::CMesh>()) {
                fmc::CMesh* cmesh = e->get<fmc::CMesh>();

                std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader(material->shaderName);
                shader->Use()->setMatrix("FM_PV", FM_PV)->setInt("BloomEffect", material->bloom);

                glm::mat4 model = glm::mat4();
                setModel(model, transform);

                shader->setMatrix("model", model)->setColor("mainColor", material->color);

                if(material->textureReady) {
                    glActiveTexture(GL_TEXTURE0);
                    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    material->getTexture().bind();
                }

                draw(cmesh);
            }

            if(e->has<fmc::CPointLight>()) {
                std::string ln = "light[" + std::to_string(lightNumber) + "]";

                lightShader->Use()
                    ->setVector3f(ln + ".position",
                                  glm::vec3(transform->position.x, transform->position.y, transform->layer))
                    ->setColor(ln + ".color", e->get<fmc::CPointLight>()->color)
                    ->setInt(ln + ".ready", 1);
                lightNumber++;
            }

            if(e->has<fmc::CText>()) {
                fmc::CText *text = e->get<fmc::CText>();
                std::shared_ptr<fm::Shader> shader = fm::ResourcesManager::get().getShader(material->shaderName);
                shader->Use();
                
                shader->setMatrix("projection", text->projection);
                shader->setColor("textColor", material->color);
                drawText(transform->position.x, transform->position.y, 
                fm::ResourcesManager::get().getResource<RFont>(text->fontName).get(), text);
                std::cout << "Text " << material->shaderName << std::endl;

            }
        }
    }

    fm::Renderer::getInstance().lightComputation();
}

void RenderingSystem::over() {

    // fm::Renderer::getInstance().blur();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    finalShader->Use();
    finalShader->setVector2f("viewPos", cameras[0]->get<fmc::CTransform>()->position);
    fm::Renderer::getInstance().postProcess(true);
    // fm::Renderer::getInstance().bindFrameBuffer();

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

void RenderingSystem::setModel(glm::mat4& model, fmc::CTransform* transform) {
    model = glm::translate(model, glm::vec3(transform->position.x, transform->position.y, -transform->layer));
    model = glm::translate(model, glm::vec3(0.5f * transform->scale.x, 0.5f * transform->scale.y, 0.0f));
    model = glm::rotate(model, transform->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * transform->scale.x, -0.5f * transform->scale.y, 0.0f));
    model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, 1.0f));
}

void RenderingSystem::drawText(int posX, int posY, RFont* font, const fmc::CText* ctext) {
     float x = posX;
    float y = posY;
    // shader.Use();

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(ctext->VAO);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Iterate through all characters
    std::string::const_iterator c;
    for(c = ctext->text.begin(); c != ctext->text.end(); c++) {
        Character ch = font->Characters[*c];
        GLfloat xpos = x + ch.Bearing.x * ctext->scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * ctext->scale;

        GLfloat w = ch.Size.x * ctext->scale;
        GLfloat h = ch.Size.y * ctext->scale;
        std::cout << xpos << " " << ypos << " " << w << " " << h << std::endl;
       
        // Update VBO for each character
        GLfloat vertices[6][4] = { { xpos, ypos + h, 0.0, 0.0 },
                                   { xpos, ypos, 0.0, 1.0 },
                                   { xpos + w, ypos, 1.0, 1.0 },

                                   { xpos, ypos + h, 0.0, 0.0 },
                                   { xpos + w, ypos, 1.0, 1.0 },
                                   { xpos + w, ypos + h, 1.0, 0.0 } };
        // std::cout << ch.TextureID << std::endl;
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, ctext->VBO);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * ctext->scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th
                                        // pixels by 64 to get amount of pixels))
        //std::cout << *c << std::endl;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderingSystem::draw(const fmc::CMesh* cmesh) {

    glBindVertexArray(cmesh->VAO);
    glDrawElements(GL_TRIANGLES, cmesh->size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}