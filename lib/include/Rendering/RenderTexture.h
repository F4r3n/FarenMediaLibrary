#pragma once
//TODO forward mode
#include <GL/glew.h>
#include "Rendering/Texture.h"
namespace fm {
    class RenderTexture {
    public:
        RenderTexture() {}
        RenderTexture(unsigned int width, unsigned int height, unsigned short depth);
        RenderTexture(unsigned int width, unsigned int height);
        ~RenderTexture();
        
        void release();
        bool isCreated();
        
        inline unsigned int getWidth() {return width;}
        inline unsigned int getHeight() {return height;}
        Texture* getColorBuffer() {return textureColorbuffer;}
        GLuint  getLightBuffer() {return lightShadowFBO;}
        Texture* getLightTextures() {return textureLightBuffer;}

        bool active();
        
    private:
        bool isReady = false;
        unsigned int width;
        unsigned int height;
        unsigned short depth;
        
        bool initFrameBuffer();
        //main Framebuffer + position fragcolor bright color
        GLuint framebuffer;
        Texture textureColorbuffer[3];
        
        GLuint lightShadowFBO;
        Texture textureLightBuffer[2];
        GLuint rboDepth;
    };
}