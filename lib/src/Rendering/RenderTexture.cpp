#include "Rendering/RenderTexture.h"
#include <iostream>

using namespace fm;

RenderTexture::RenderTexture(unsigned int width, unsigned int height,
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth, int multiSampling) {
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

RenderTexture::RenderTexture(const RenderTexture &renderTexture, int multiSampling) {
    _width = renderTexture._width;
    _height = renderTexture._height;
    _depth = renderTexture._depth;
    _numberColors = renderTexture._numberColors;
    if(multiSampling == -1)
    _multiSampling = renderTexture._multiSampling;
    for(unsigned int i = 0; i < renderTexture._numberColors; ++i) 
	{
        _formats.push_back(renderTexture._formats[i]);
        _types.push_back(renderTexture._types[i]);
    }
}


const RenderTexture& RenderTexture::operator=(const RenderTexture &inRenderTexture)
{
	Clone(inRenderTexture);
	return *this;
}


void RenderTexture::Clone(const RenderTexture &inRenderTexture)
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

RenderTexture::~RenderTexture() {
}

void RenderTexture::release() {
    //std::cout << "Release render texture" << std::endl;
    for(unsigned int i = 0; i < _textureColorbuffer.size(); i++)
    {
        _textureColorbuffer[i].release();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //rboDepth.release();

    glDeleteRenderbuffers(1, &_framebuffer);

    _isReady = false;
}

bool RenderTexture::isCreated()
{
    return _isReady;
}


bool RenderTexture::_InitFrameBuffer(Format *formats, Type *types)
{
    if( _width == 0 || _height == 0) return false;
    
    glGenFramebuffers(1, &_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    // Create a color attachment texture [FragColor and BrightColor]
    if(formats != nullptr && types != nullptr)
    {
        
        for(int i = 0; i < _numberColors; i++)
        {
            Texture t;
            t.filter = Filter::NEAREST;

            t.generate(_width, _height, formats[i], types[i], _multiSampling);

            int error = glGetError();
            if(error != 0) {
                std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
                exit(-1);
            }
            if(_multiSampling > 0)
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, t.getID(), 0);

            }else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t.getID(), 0);
            }

            error = glGetError();
            if(error != 0) {
                std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
                exit(-1);
            }
            _textureColorbuffer.push_back(t);
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
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, _multiSampling, GL_DEPTH24_STENCIL8, _width, _height);
            }else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);

            }
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);

    }

    std::vector<GLuint> attachments;
    for(unsigned int i = 0; i < _numberColors; i++)
    {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(attachments.size(), attachments.data());
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

void RenderTexture::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

}
