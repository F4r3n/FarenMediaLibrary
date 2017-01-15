#pragma once
//TODO forward mode
#include <GL/glew.h>

namespace fm {
    class RenderTexture {
    public:
        RenderTexture() {}
        RenderTexture(unsigned int width, unsigned int height, unsigned short depth);
        RenderTexture(unsigned int width, unsigned int height);
        ~RenderTexture();
        
        void release();
        bool isCreated();
        
        unsigned int getWidth() {return width;}
        unsigned int getHeight() {return height;}
        GLuint* getColorBuffer() {return textureColorbuffer;}
        GLuint  getLightBuffer() {return lightShadowFBO;}
        GLuint* getPingPongFBO() {return pingpongFBO;}
        GLuint* getPingPongBuffer() {return pingpongColorbuffers;}
        bool active();
    private:
        bool isReady = false;
        unsigned int width;
        unsigned int height;
        unsigned short depth;
        
        bool initFrameBuffer();
        GLuint framebuffer;
        GLuint textureColorbuffer[3];
    
        GLuint pingpongFBO[2];
        GLuint pingpongColorbuffers[2];
        
        GLuint lightShadowFBO;
    
        GLuint rboDepth;
    };
}