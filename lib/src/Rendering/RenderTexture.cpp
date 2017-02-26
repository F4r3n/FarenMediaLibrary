#include "Rendering/RenderTexture.h"
#include <iostream>

using namespace fm;

RenderTexture::RenderTexture(unsigned int width, unsigned int height, unsigned short depth) {
    this->width = width;
    this->height = height;
    this->depth = depth;
    
    isReady = initFrameBuffer();
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    
    isReady = initFrameBuffer();
}

RenderTexture::~RenderTexture() {
}

void RenderTexture::release() {
    std::cout << "Release render texture" << std::endl;
    glDeleteTextures(3, textureColorbuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteRenderbuffers(1, &rboDepth);
    glDeleteRenderbuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &lightShadowFBO);

    glDeleteTextures(2, textureLightBuffer);
    isReady = false;
    
    for(int i = 0; i < 3; i++) textureColorbuffer[i] = 0;
}
bool RenderTexture::isCreated() {
    return isReady;
}

bool RenderTexture::active() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    return true;
}

bool RenderTexture::initFrameBuffer() {
    if( width == 0 || height == 0) return false;
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // Create a color attachment texture [FragColor and BrightColor]
    glGenTextures(3, textureColorbuffer);
    for(int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorbuffer[i], 0);
    }

    // Position texture
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, GL_TEXTURE_2D, textureColorbuffer[2], 0);

    if(depth != 0) {
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    }

    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // FBO to compute lights
    glGenFramebuffers(1, &lightShadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, lightShadowFBO);
    glGenTextures(2, textureLightBuffer);

    for(GLuint i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, textureLightBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureLightBuffer[i], 0);
    }
    GLuint attachments2[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return true;
}