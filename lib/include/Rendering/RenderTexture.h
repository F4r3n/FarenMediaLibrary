#pragma once

#include <Core/Config.h>
#include "Rendering/Texture.h"

namespace fm 
{

    class RenderTexture
	{
    public:
        RenderTexture() {}
        RenderTexture(unsigned int width, unsigned int height, 
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth, int multiSampling = 0);
        RenderTexture(const RenderTexture &renderTexture, int multiSampling = -1);
        ~RenderTexture();
        
        void release();
        bool isCreated();
        void bind();
        inline unsigned int getWidth() {return _width;}
        inline unsigned int getHeight() {return _height;}
        Texture* getColorBuffer() {return _textureColorbuffer.data();}
        void create();
        bool IsMultiSampled() const {return _multiSampling > 0;}
        unsigned int GetId() const {return _framebuffer;}

		void Clone(const RenderTexture &inRenderTexture);
		const RenderTexture& operator=(const RenderTexture &inRenderTexture);
    private:
        bool _isReady = false;
        unsigned int _width = 0;
        unsigned int _height = 0;
        unsigned short _depth = 0;
        unsigned short _numberColors = 0;
        int _multiSampling = 0;
        std::vector<Format> _formats;
        std::vector<Type> _types;
        bool _InitFrameBuffer(Format *formats, Type *types);
        //main Framebuffer + position fragcolor bright color
        GLuint _framebuffer;
        std::vector<Texture> _textureColorbuffer;

        //Texture rboDepth;
        unsigned int _rboDepth;
    };
}
