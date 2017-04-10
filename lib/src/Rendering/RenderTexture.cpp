#include "Rendering/RenderTexture.h"
#include <iostream>

using namespace fm;

RenderTexture::RenderTexture(unsigned int width, unsigned int height, 
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->numberColors = numberColorAttchment;
    isReady = initFrameBuffer(formats, types);
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, unsigned int numberColorAttchment) {
    this->width = width;
    this->height = height;
    this->numberColors = numberColorAttchment;
    isReady = initFrameBuffer(nullptr, nullptr);
}

RenderTexture::~RenderTexture() {
}

void RenderTexture::release() {
    //std::cout << "Release render texture" << std::endl;
    for(int i = 0; i < textureColorbuffer.size(); i++) textureColorbuffer[i].release();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    rboDepth.release();
    glDeleteRenderbuffers(1, &framebuffer);

    isReady = false;
    
    
}
bool RenderTexture::isCreated() {
    return isReady;
}

bool RenderTexture::active() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    return true;
}

bool RenderTexture::initFrameBuffer(Format *formats, Type *types) {
    if( width == 0 || height == 0) return false;
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // Create a color attachment texture [FragColor and BrightColor]
    if(formats != nullptr && types != nullptr) {
        
        for(int i = 0; i < numberColors; i++) {
            Texture t;
            t.filter = Filter::NEAREST;
            t.generate(width, height, formats[i], types[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, t.getID(), 0);
            textureColorbuffer.push_back(t);
        }
    }

    if(depth != 0) {
        rboDepth.generate(width, height, Format::DEPTH_STENCIL, Type::UNSIGNED_24_8);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepth.getID(), 0);
    }

    std::vector<GLuint> attachments;
    for(int i = 0; i < numberColors; i++) {
        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(attachments.size(), attachments.data());
    GLenum statusFrameBuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( statusFrameBuffer != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << std::endl;
        if(statusFrameBuffer == GL_FRAMEBUFFER_UNSUPPORTED) {
            std::cerr << "Error unsupported format" << std::endl;
        }
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void RenderTexture::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

}