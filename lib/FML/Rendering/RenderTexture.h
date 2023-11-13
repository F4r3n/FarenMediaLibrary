#pragma once
#include <vector>
namespace fm 
{
	enum class Format;
	enum class Type;
	class OGLTexture;

    class RenderTexture
	{
    public:
        RenderTexture() {}
        RenderTexture(size_t width, size_t height, size_t numberColorAttchment, Format *formats, Type *types, size_t depth, int multiSampling = 0);
        RenderTexture(const RenderTexture &renderTexture, int multiSampling = -1);
        ~RenderTexture();
        
        bool isCreated() const;
        void bind(bool isRead = false) const;
        inline size_t getWidth() const {return _width;}
        inline size_t getHeight() const {return _height;}
        
		std::shared_ptr<fm::OGLTexture> GetColorBufferTexture(size_t id) const;
		const std::vector<std::shared_ptr<fm::OGLTexture>>& GetColorBuffer() const { return _textureColorbuffer; }

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
		size_t _depth = 0;
		size_t _numberColors = 0;
        int _multiSampling = 0;
        std::vector<Format> _formats;
        std::vector<Type> _types;
        bool _InitFrameBuffer(Format *formats, Type *types);
        //main Framebuffer + position fragcolor bright color
        std::vector<std::shared_ptr<OGLTexture>> _textureColorbuffer;

        //Texture rboDepth;
		unsigned int _framebuffer = 0;
		unsigned int _rboDepth = 0;
    };
}
