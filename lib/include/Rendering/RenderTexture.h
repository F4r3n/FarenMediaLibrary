#pragma once

#include <Core/Config.h>
#include "Rendering/Texture.h"

namespace fm {
    class RenderTexture {
    public:
        RenderTexture() {}
        RenderTexture(unsigned int width, unsigned int height, 
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth, int multiSampling = 0);
        RenderTexture(unsigned int width, unsigned int height, unsigned int numberColorAttchment);
        RenderTexture(const RenderTexture &renderTexture, int multiSampling = -1);
        ~RenderTexture();
        
        void release();
        bool isCreated();
        void bind();
        inline unsigned int getWidth() {return width;}
        inline unsigned int getHeight() {return height;}
        Texture* getColorBuffer() {return textureColorbuffer.data();}
        void create();
        bool active();
        bool IsMultiSampled() const {return multiSampling > 0;}
        unsigned int GetId() const {return framebuffer;}
    private:
        bool isReady = false;
        unsigned int width;
        unsigned int height;
        unsigned short depth;
        unsigned short numberColors = 0;
        int multiSampling = 0;
        std::vector<Format> _formats;
        std::vector<Type> _types;
        bool initFrameBuffer(Format *formats, Type *types);
        //main Framebuffer + position fragcolor bright color
        GLuint framebuffer;
        std::vector<Texture> textureColorbuffer;

        //Texture rboDepth;
        unsigned int rboDepth;
    };
}
