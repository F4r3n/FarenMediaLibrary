#pragma once

#include <Core/Config.h>
#include "Rendering/Texture.h"

namespace fm {
    class RenderTexture {
    public:
        RenderTexture() {}
        RenderTexture(unsigned int width, unsigned int height, 
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth);
        RenderTexture(unsigned int width, unsigned int height, unsigned int numberColorAttchment);
        ~RenderTexture();
        
        void release();
        bool isCreated();
        void bind();
        inline unsigned int getWidth() {return width;}
        inline unsigned int getHeight() {return height;}
        Texture* getColorBuffer() {return textureColorbuffer.data();}

        bool active();
        
    private:
        bool isReady = false;
        unsigned int width;
        unsigned int height;
        unsigned short depth;
        unsigned short numberColors = 0;
        bool initFrameBuffer(Format *formats, Type *types);
        //main Framebuffer + position fragcolor bright color
        GLuint framebuffer;
        std::vector<Texture> textureColorbuffer;

        Texture rboDepth;
    };
}