#pragma once

#include <Core/Config.h>
#include "Rendering/OpenGL/OGLTexture.hpp"

namespace fm 
{

    class RenderTexture
	{
    public:
        RenderTexture() {}
        RenderTexture(size_t width, size_t height, 
unsigned int numberColorAttchment, Format *formats, Type *types, unsigned short depth, int multiSampling = 0);
        RenderTexture(const RenderTexture &renderTexture, int multiSampling = -1);
        ~RenderTexture();
        
        bool isCreated() const;
        void bind(bool isRead = false);
        inline size_t getWidth() const {return _width;}
        inline size_t getHeight() const {return _height;}
        
		const fm::OGLTexture& GetColorBufferTexture(size_t id) const;
		const std::vector<OGLTexture>& GetColorBuffer() const { return _textureColorbuffer; }

        void create();
        bool IsMultiSampled() const {return _multiSampling > 0;}
        unsigned int GetId() const {return _framebuffer;}

		const RenderTexture& operator=(RenderTexture &&inRenderTexture);
		const RenderTexture& operator=(const RenderTexture &inRenderTexture)= delete;

    private:
		void _Clone(const RenderTexture& inRenderTexture);
		void _Release();

        bool _isReady = false;
        size_t _width = 0;
        size_t _height = 0;
        unsigned short _depth = 0;
        unsigned short _numberColors = 0;
        int _multiSampling = 0;
        std::vector<Format> _formats;
        std::vector<Type> _types;
        bool _InitFrameBuffer(Format *formats, Type *types);
        //main Framebuffer + position fragcolor bright color
        std::vector<OGLTexture> _textureColorbuffer;

        //Texture rboDepth;
		GLuint _framebuffer = 0;
		GLuint _rboDepth = 0;
    };
}
