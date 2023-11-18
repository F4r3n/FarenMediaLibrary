#pragma once
#include <Core/Math/Vector4.h>
#include <Core/Rect.h>
#include <unordered_map>
#include "GL/glew.h"
#include "Rendering/OpenGL/OGLVertexBuffer.hpp"
namespace fm {

	class Model;
	class OGLModel;
	namespace rendering
	{
		class VertexBuffer;
	}

	enum RENDERING_TYPE : unsigned int {
		BLEND = GL_BLEND,
		DEPTH_TEST = GL_DEPTH_TEST,
		CULL_FACE = GL_CULL_FACE,

    };

	
	enum BUFFER_BIT : uint32_t
	{
		COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
		NONE_BUFFER_BIT = 0
	};
	constexpr enum BUFFER_BIT operator |(const enum BUFFER_BIT selfValue, const enum BUFFER_BIT inValue)
	{
		return (enum BUFFER_BIT)(uint32_t(selfValue) | uint32_t(inValue));
	}
    class Model;
	typedef std::unordered_map<fm::RENDERING_TYPE, bool> RenderingSettings;
    class Graphics
	{
      public :
        Graphics();
        ~Graphics();
        void Clear(BUFFER_BIT = COLOR_BUFFER_BIT) const;
        void SetViewPort(const fm::Rect<size_t> &rect) const;
        bool Enable(RENDERING_TYPE renderingTYPE) const;
        void Disable(RENDERING_TYPE renderingTYPE) const;
        void Draw(int primitiveType, size_t vertexCount, size_t* indices) const;
        void Draw(int primitiveType, size_t vertexStart, size_t vertexCount) const;
        void Draw(OGLModel* model) const;
        void BindVertexBuffer(rendering::OGLVertextBuffer *vertexBuffer) const;
        void BindFrameBuffer(unsigned int id) const;
        void BindTexture2D(size_t number, int idTexture, int type) const;
		void ActivateTexture2D(size_t number) const;

		void RestoreSettings(const RenderingSettings &inSettings);
		const RenderingSettings& GetRenderingSettings();
	private:
		mutable RenderingSettings _renderingSettings;
    };
    
}
