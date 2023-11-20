#include "OGLFrameBuffer.hpp"
#include <iostream>
#include <cassert>
#include "Core/Debug.h"
#include "GL/glew.h"
#include "OGLTextureDef.hpp"
#include "OGLTexture.hpp"
#include "Rendering/TextureDef.hpp"
#include "Rendering/RenderTexture.hpp"
#include "OGLTextureDef.hpp"
using namespace fm;

OGLFrameBuffer::OGLFrameBuffer(size_t width, size_t height,
	const std::vector<TextureFormat>& formats, const std::vector<TextureType>& types, size_t depth, size_t multiSampling)
	: FrameBuffer(width, height, formats, types, depth, multiSampling)
{

}

OGLFrameBuffer::OGLFrameBuffer(const RenderTexture&renderTexture, size_t multiSampling)
	:FrameBuffer(renderTexture, multiSampling)
{

}


void OGLFrameBuffer::create() {

    _created = _InitFrameBuffer();
}

OGLFrameBuffer::~OGLFrameBuffer()
{
	_Release();
}

void OGLFrameBuffer::_Release() {

	for(unsigned int i = 0; i < _textureColorbuffer.size(); i++)
    {
        _textureColorbuffer[i]->release();
    }
	

	if (_rboDepth > 0)
	{
		glDeleteRenderbuffers(1, &_rboDepth);
	}
	
	if (_framebuffer > 0)
	{
		glDeleteFramebuffers(1, &_framebuffer);
	}
}



bool OGLFrameBuffer::_InitFrameBuffer()
{
    if( _width == 0 || _height == 0) return false;

    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Create a color attachment texture [FragColor and BrightColor]
    if(!_formats.empty())
    {
        
        for(size_t i = 0; i < _formats.size(); i++)
        {
			std::shared_ptr<OGLTexture> t = std::make_shared<fm::OGLTexture>();
            t->filter = OGLTextureFilter::NEAREST;
            t->generate(_width, _height, ConvertTextureFormatToOGL(_formats[i]), ConvertTextureTypeToOGL(_types[i]), _multiSampling);

            if(_multiSampling > 0)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D_MULTISAMPLE, t->getID(), 0);

            }else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (GLenum)i, GL_TEXTURE_2D, t->getID(), 0);
            }

            _textureColorbuffer.emplace_back(t);
        }
    }

    if(_depth != 0)
    {
        //rboDepth.generate(width, height, Format::DEPTH_STENCIL, Type::UNSIGNED_24_8);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepth.getID(), 0);

            glGenRenderbuffers(1, &_rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
            if(_multiSampling > 0)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, (GLsizei)_multiSampling, GL_DEPTH24_STENCIL8, (GLsizei)_width, (GLsizei)_height);
            }else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)_width, (GLsizei)_height);

            }
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);

    }

    std::vector<GLuint> attachments;
    for(unsigned int i = 0; i < _numberColors; ++i)
    {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers((GLsizei)attachments.size(), attachments.data());
    GLenum statusFrameBuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( statusFrameBuffer != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << std::endl;
        if(statusFrameBuffer == GL_FRAMEBUFFER_UNSUPPORTED)
        {
            std::cerr << "Error unsupported format" << std::endl;
        }
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

std::shared_ptr<fm::OGLTexture> OGLFrameBuffer::GetColorBufferTexture(size_t id) const
{
	assert(id < _numberColors && id >= 0);
	return _textureColorbuffer[id];
}


void OGLFrameBuffer::bind(bool isRead) const
{
	if (isRead)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	}
}
