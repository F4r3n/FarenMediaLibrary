#include "Rendering/Renderer.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
#include "Rendering/Texture.h"
using namespace fm;

Renderer Renderer::_instance;

Renderer::Renderer() {
}

void Renderer::createQuadScreen() {
    if(quad == nullptr) {
        quad = ResourcesManager::get().getResource<fm::Model>("QuadFS");
    }
}

void Renderer::lightComputation(fm::Graphics& graphics,
                                Texture* colorBuffer,
                                bool compute) {
    fm::Shader* light;
    if(compute)
        light = ResourcesManager::get().getResource<fm::Shader>("light");
    else {
        light = ResourcesManager::get().getResource<fm::Shader>("no_light");
    }
    light->Use();
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ <<std::endl;
        exit(-1);
    }
    graphics.bindTexture2D(0, colorBuffer[0].getID(), colorBuffer[0].GetKind());

    if(compute) {
        graphics.bindTexture2D(1, colorBuffer[2].getID(), colorBuffer[2].GetKind());
    }

    graphics.draw(quad);
}

void Renderer::postProcess(fm::Graphics& graphics, Texture* colorBuffer) {
    graphics.bindTexture2D(0, colorBuffer[0].getID(), colorBuffer[0].GetKind());
    graphics.bindTexture2D(1, colorBuffer[1].getID(), colorBuffer[1].GetKind());

    graphics.draw(quad);
}

void Renderer::blit(fm::Graphics& graphics,
                    Texture& texture,
                    Shader* shader) const {
    shader->Use();
    graphics.bindTexture2D(0, texture.getID(), texture.GetKind());

    graphics.draw(quad);
}
void Renderer::blit(fm::Graphics& graphics,
                    RenderTexture& source,
                    RenderTexture& dest,
                    Shader* shader) const {
    dest.bind();
    shader->Use();
    graphics.bindTexture2D(0, source.getColorBuffer()[0].getID(), source.getColorBuffer()[0].GetKind());

    graphics.draw(quad);
}

void Renderer::SetSources(fm::Graphics& graphics,
                          Texture* textures,
                          int numberIDs) {
    for(int i = 0; i < numberIDs; ++i) {
        graphics.bindTexture2D(i, textures[i].getID(), textures[i].GetKind());
    }
}

void Renderer::blit(fm::Graphics& graphics,
                    int ID,
                    RenderTexture& dest,
                    Shader* shader) const {
    dest.bind();
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);

    graphics.draw(quad);
}

void Renderer::blit(fm::Graphics& graphics,
                    RenderTexture& dest,
                    Shader* shader) {

    shader->Use();
    dest.bind();
    graphics.draw(quad);
}
void Renderer::blit(fm::Graphics& graphics, Shader* shader) {
    shader->Use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    graphics.draw(quad);
}

void Renderer::clear(fm::Graphics& graphics) {
    graphics.clear(true, true, true);
}

Renderer::~Renderer() {
}
