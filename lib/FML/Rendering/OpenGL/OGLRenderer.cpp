#include "OGLRenderer.h"
#include "Resource/ResourcesManager.h"
#include <iostream>
#include "Rendering/OpenGL/OGLTexture.hpp"
#include "Rendering/Shader.h"
#include "OGLFrameBuffer.hpp"
#include "Rendering/Model.hpp"
#include "Rendering/OpenGL/OGLShader.hpp"
#include <cassert>
using namespace fm;

OGLRenderer OGLRenderer::_instance;

OGLRenderer::OGLRenderer() {
}

void OGLRenderer::SetQuadScreen(fm::OGLModel* inModel) {
	_quad = inModel;
}

void OGLRenderer::lightComputation(fm::Graphics& graphics,
                                const OGLTexture&colorBuffer,
                                bool compute) 
{
	assert(false);
    //std::shared_ptr<fm::Shader> light = ResourcesManager::get().getResource<fm::Shader>("no_light");
	//
    //light->Use();
    //int error = glGetError();
    //if(error != 0) 
	//{
    //    std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ <<std::endl;
    //}
    //graphics.BindTexture2D(0, colorBuffer.getID(), (int)colorBuffer.GetKind());
    //graphics.Draw(_quad);
}

void OGLRenderer::postProcess(fm::Graphics& graphics, const OGLTexture& inTexture1)
{
	graphics.Disable(DEPTH_TEST);
    graphics.BindTexture2D(0, inTexture1.getID(), (int)inTexture1.GetKind());
    //graphics.bindTexture2D(1, colorBuffer[1].getID(), colorBuffer[1].GetKind());

    graphics.Draw(_quad);
}

void OGLRenderer::blit(fm::Graphics& graphics, OGLTexture& texture, OGLShader* shader) const
{
    shader->Use();
    graphics.BindTexture2D(0, texture.getID(), (int)texture.GetKind());

    graphics.Draw(_quad);
}


void OGLRenderer::blit(fm::Graphics& graphics,
                    const OGLFrameBuffer& source,
					const OGLFrameBuffer& dest,
					OGLShader* shader) const
{
    dest.bind();
    shader->Use();
    graphics.BindTexture2D(0, source.GetColorBufferTexture(0)->getID(), (int)source.GetColorBufferTexture(0)->GetKind());
	
    graphics.Draw(_quad);
}


void OGLRenderer::blit(fm::Graphics &graphics, const OGLFrameBuffer& source, BUFFER_BIT bufferBit) const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, (GLint)source.GetWidth(), (GLint)source.GetHeight(), 0, 0,
		(GLint)source.GetWidth(),
		(GLint)source.GetHeight(),
		bufferBit, GL_NEAREST);
}


void OGLRenderer::blit(fm::Graphics &graphics, const OGLFrameBuffer& source, const OGLFrameBuffer& dest, BUFFER_BIT bufferBit) const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source.GetId());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest.GetId());
	glBlitFramebuffer(0, 0, (GLint)source.GetWidth(), (GLint)source.GetHeight(), 0, 0,
		(GLint)dest.GetWidth(),
		(GLint)dest.GetHeight(),
		bufferBit, GL_NEAREST);
}


void OGLRenderer::SetSources(fm::Graphics& graphics, const std::vector<std::shared_ptr<fm::OGLTexture>> &textures, size_t numberIDs)
{
    for(size_t i = 0; (i < numberIDs && i < textures.size()); ++i) 
	{
        graphics.BindTexture2D(i, textures[i]->getID(), (int)textures[i]->GetKind());
    }
}


void OGLRenderer::blit(fm::Graphics& graphics,
                    int ID,
	const OGLFrameBuffer& dest,
					OGLShader* shader) const
{
    dest.bind();
    shader->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);

    graphics.Draw(_quad);
}


void OGLRenderer::blit(fm::Graphics& graphics,
                    const OGLFrameBuffer& dest,
					OGLShader* shader)
{

    shader->Use();
    dest.bind();
    graphics.Draw(_quad);
}


void OGLRenderer::blit(fm::Graphics& graphics, OGLShader* shader)
{
    shader->Use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    graphics.Draw(_quad);
}


void OGLRenderer::clear(fm::Graphics& graphics)
{
    graphics.Clear(fm::BUFFER_BIT::COLOR_BUFFER_BIT | fm::BUFFER_BIT::DEPTH_BUFFER_BIT);
}


OGLRenderer::~OGLRenderer()
{
}
