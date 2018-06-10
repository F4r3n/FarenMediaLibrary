#include "Rendering/RenderTexture.h"
#include <iostream>

using namespace fm;

RenderTexture::RenderTexture(unsigned int width, unsigned int height,
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth, int multiSampling) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->numberColors = numberColorAttchment;
    for(unsigned int i = 0; i < numberColorAttchment; ++i) {
        _formats.push_back(formats[i]);
        _types.push_back(types[i]);
    }
    this->multiSampling = multiSampling;
}

RenderTexture::RenderTexture(const RenderTexture &renderTexture, int multiSampling) {
    this->width = renderTexture.width;
    this->height = renderTexture.height;
    this->depth = renderTexture.depth;
    this->numberColors = renderTexture.numberColors;
    if(multiSampling == -1)
    this->multiSampling = renderTexture.multiSampling;
    for(unsigned int i = 0; i < renderTexture.numberColors; ++i) {
        _formats.push_back(renderTexture._formats[i]);
        _types.push_back(renderTexture._types[i]);
    }
}
RenderTexture::RenderTexture(unsigned int width, unsigned int height, unsigned int numberColorAttchment) {
    this->width = width;
    this->height = height;
    this->numberColors = numberColorAttchment;
    isReady = initFrameBuffer(nullptr, nullptr);
}

void RenderTexture::create() {

    isReady = initFrameBuffer(_formats.data(), _types.data());
}

RenderTexture::~RenderTexture() {
}

void RenderTexture::release() {
    //std::cout << "Release render texture" << std::endl;
    for(unsigned int i = 0; i < textureColorbuffer.size(); i++)
    {
        textureColorbuffer[i].release();
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //rboDepth.release();

    glDeleteRenderbuffers(1, &framebuffer);

    isReady = false;
    
    
}
bool RenderTexture::isCreated()
{
    return isReady;
}

bool RenderTexture::active()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    return true;
}

bool RenderTexture::initFrameBuffer(Format *formats, Type *types)
{
    if( width == 0 || height == 0) return false;
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    int error = glGetError();
    if(error != 0) {
        std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
        exit(-1);
    }
    // Create a color attachment texture [FragColor and BrightColor]
    if(formats != nullptr && types != nullptr)
    {
        
        for(int i = 0; i < numberColors; i++)
        {
            Texture t;
            t.filter = Filter::NEAREST;
            std::cout << "Create texture " <<i << " " << numberColors << " " << multiSampling<< std::endl;

            t.generate(width, height, formats[i], types[i], this->multiSampling);

            int error = glGetError();
            if(error != 0) {
                std::cerr << "ERROR OPENGL " << error << " " << __LINE__<< " " << __FILE__ << std::endl;
                exit(-1);
            }
            if(multiSampling > 0)
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
            textureColorbuffer.push_back(t);
        }
    }

    if(depth != 0)
    {
        //rboDepth.generate(width, height, Format::DEPTH_STENCIL, Type::UNSIGNED_24_8);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepth.getID(), 0);

            glGenRenderbuffers(1, &rboDepth);
            glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
            if(multiSampling > 0)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSampling, GL_DEPTH24_STENCIL8, width, height);
            }else
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

            }
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    }

    std::vector<GLuint> attachments;
    for(unsigned int i = 0; i < numberColors; i++)
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
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

}
