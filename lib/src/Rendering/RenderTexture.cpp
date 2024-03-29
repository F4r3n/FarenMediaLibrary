#include "Rendering/RenderTexture.h"
#include <iostream>
#include <cassert>
#include "Core/Debug.h"
#include "GL/glew.h"
#include "OpenGL/OGLTextureDef.hpp"
#include "OpenGL/OGLTexture.hpp"

using namespace fm;

RenderTexture::RenderTexture(size_t width, size_t height, size_t numberColorAttchment, Format *formats, Type *types, size_t depth, int multiSampling) {
    _width = width;
    _height = height;
    _depth = depth;
    _numberColors = numberColorAttchment;
    for(unsigned int i = 0; i < numberColorAttchment; ++i) {
        _formats.push_back(formats[i]);
        _types.push_back(types[i]);
    }
    _multiSampling = multiSampling;
}

RenderTexture::RenderTexture(const RenderTexture &renderTexture, [[maybe_unused]] int multiSampling) {
    _width = renderTexture._width;
    _height = renderTexture._height;
    _depth = renderTexture._depth;
    _numberColors = renderTexture._numberColors;
    _multiSampling = renderTexture._multiSampling;
    for(unsigned int i = 0; i < renderTexture._numberColors; ++i) 
	{
        _formats.push_back(renderTexture._formats[i]);
        _types.push_back(renderTexture._types[i]);
    }
}



const RenderTexture& RenderTexture::operator=(RenderTexture &&inRenderTexture)
{
	_Clone(inRenderTexture);

	inRenderTexture._formats.clear();
	inRenderTexture._types.clear();
	inRenderTexture._textureColorbuffer.clear();
	inRenderTexture._rboDepth = 0;
	inRenderTexture._framebuffer = 0;
	return *this;
}


void RenderTexture::_Clone(const RenderTexture &inRenderTexture)
{
	_width = inRenderTexture._width;
	_height = inRenderTexture._height;
	_depth = inRenderTexture._depth;
	_numberColors = inRenderTexture._numberColors;
	_multiSampling = inRenderTexture._multiSampling;

	_formats.clear();
	_types.clear();
	for (unsigned int i = 0; i < inRenderTexture._numberColors; ++i)
	{
		_formats.push_back(inRenderTexture._formats[i]);
		_types.push_back(inRenderTexture._types[i]);
	}

	_textureColorbuffer.clear();
	for (auto && texture : inRenderTexture._textureColorbuffer)
	{
		_textureColorbuffer.emplace_back(texture);
	}

	_isReady = inRenderTexture._isReady;
	_framebuffer = inRenderTexture._framebuffer;
	_rboDepth = inRenderTexture._rboDepth;
}



void RenderTexture::create() {

    _isReady = _InitFrameBuffer(_formats.data(), _types.data());
}

RenderTexture::~RenderTexture()
{
	_Release();
}

void RenderTexture::_Release() {

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

bool RenderTexture::isCreated() const
{
    return _isReady;
}


bool RenderTexture::_InitFrameBuffer(Format *formats, Type *types)
{
    if( _width == 0 || _height == 0) return false;

    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

    // Create a color attachment texture [FragColor and BrightColor]
    if(formats != nullptr && types != nullptr)
    {
        
        for(int i = 0; i < _numberColors; i++)
        {
			std::shared_ptr<OGLTexture> t = std::make_shared<fm::OGLTexture>();
            t->filter = Filter::NEAREST;
            t->generate(_width, _height, formats[i], types[i], _multiSampling);

            if(_multiSampling > 0)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, t->getID(), 0);

            }else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t->getID(), 0);
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
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, _multiSampling, GL_DEPTH24_STENCIL8, (GLsizei)_width, (GLsizei)_height);
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

std::shared_ptr<fm::OGLTexture> RenderTexture::GetColorBufferTexture(size_t id) const
{
	assert(id < _numberColors && id >= 0);
	return _textureColorbuffer[id];
}


void RenderTexture::bind(bool isRead) const
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
