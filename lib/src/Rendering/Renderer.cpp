#include "Rendering/Renderer.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTexture.h"

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
                                const Texture &colorBuffer,
                                bool compute) 
{
    fm::Shader* light;
    light = ResourcesManager::get().getResource<fm::Shader>("no_light");

    light->Use();
    int error = glGetError();
    if(error != 0) 
	{
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ <<std::endl;
    }
    graphics.BindTexture2D(0, colorBuffer.getID(), colorBuffer.GetKind());
    graphics.Draw(quad);
}

void Renderer::postProcess(fm::Graphics& graphics, const Texture& inTexture1) 
{
	graphics.Disable(DEPTH_TEST);
    graphics.BindTexture2D(0, inTexture1.getID(), inTexture1.GetKind());
    //graphics.bindTexture2D(1, colorBuffer[1].getID(), colorBuffer[1].GetKind());

    graphics.Draw(quad);
}

void Renderer::blit(fm::Graphics& graphics,
                    Texture& texture,
                    Shader* shader) const {
    shader->Use();
    graphics.BindTexture2D(0, texture.getID(), texture.GetKind());

    graphics.Draw(quad);
}
void Renderer::blit(fm::Graphics& graphics,
                    RenderTexture& source,
                    RenderTexture& dest,
                    Shader* shader) const {
    dest.bind();
    shader->Use();
    graphics.BindTexture2D(0, source.GetColorBufferTexture(0).getID(), source.GetColorBufferTexture(0).GetKind());

    graphics.Draw(quad);
}


void Renderer::blit(fm::Graphics &graphics, RenderTexture& source, BUFFER_BIT bufferBit) const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, source.getWidth(), source.getHeight(), 0, 0,
		source.getWidth(),
		source.getHeight(),
		bufferBit, GL_NEAREST);
}


void Renderer::blit(fm::Graphics &graphics, RenderTexture& source, RenderTexture& dest, BUFFER_BIT bufferBit) const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.GetId());
	glBlitFramebuffer(0, 0, source.getWidth(), source.getHeight(), 0, 0,
		dest.getWidth(),
		dest.getHeight(),
		bufferBit, GL_NEAREST);
}


void Renderer::SetSources(fm::Graphics& graphics, const std::vector<fm::Texture> &textures, size_t numberIDs) 
{
    for(size_t i = 0; (i < numberIDs && i < textures.size()); ++i) 
	{
        graphics.BindTexture2D(i, textures[i].getID(), textures[i].GetKind());
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

    graphics.Draw(quad);
}

void Renderer::blit(fm::Graphics& graphics,
                    RenderTexture& dest,
                    Shader* shader) {

    shader->Use();
    dest.bind();
    graphics.Draw(quad);
}
void Renderer::blit(fm::Graphics& graphics, Shader* shader) {
    shader->Use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    graphics.Draw(quad);
}

void Renderer::clear(fm::Graphics& graphics) {
    graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
}

Renderer::~Renderer() {
}
