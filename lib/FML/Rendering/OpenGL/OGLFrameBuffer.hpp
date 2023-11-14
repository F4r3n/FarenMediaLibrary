#pragma once
#include <vector>
#include "Rendering/FrameBuffer.hpp"
namespace fm 
{
	enum class TextureFormat;
	enum class TextureType;
	enum class OGLTextureFormat;
	enum class OGLTextureType;
	class OGLTexture;
	class RenderTexture;

    class OGLFrameBuffer : public FrameBuffer
	{
    public:
		OGLFrameBuffer(size_t width, size_t height, const std::vector<TextureFormat>& formats, const std::vector<TextureType>& types, size_t depth, size_t multiSampling = 0);
		OGLFrameBuffer(const RenderTexture& renderTexture, size_t multiSampling = 0);
        ~OGLFrameBuffer();
        
        void bind(bool isRead = false) const;

        
		std::shared_ptr<fm::OGLTexture> GetColorBufferTexture(size_t id) const;
		const std::vector<std::shared_ptr<fm::OGLTexture>>& GetColorBuffer() const { return _textureColorbuffer; }

        void create();
        bool IsMultiSampled() const {return _multiSampling > 0;}
        unsigned int GetId() const {return _framebuffer;}

    private:
		void _Release();


        bool _InitFrameBuffer();
        //main Framebuffer + position fragcolor bright color
        std::vector<std::shared_ptr<OGLTexture>> _textureColorbuffer;

        //Texture rboDepth;
		unsigned int _framebuffer = 0;
		unsigned int _rboDepth = 0;
    };
}
