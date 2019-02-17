#pragma once
#include <Core/Math/Vector4.h>
#include <Core/Rect.h>
#include <Rendering/VertexBuffer.hpp>
namespace fm {
	enum RENDERING_TYPE : unsigned int {
		BLEND = GL_BLEND,
		DEPTH_TEST = GL_DEPTH_TEST
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
    
    class Graphics {
      public :
        Graphics();
        ~Graphics();
        void Clear(BUFFER_BIT = COLOR_BUFFER_BIT) const;
        void SetViewPort(const fm::math::vec4i &rect) const;
        void SetViewPort(const fm::Rect<float> &rect) const;
        void Enable(RENDERING_TYPE renderingTYPE) const;
        void Disable(RENDERING_TYPE renderingTYPE) const;
        void Draw(int primitiveType, unsigned int vertexCount, unsigned int* indices) const;
        void Draw(int primitiveType, unsigned int vertexStart, unsigned int vertexCount) const;
        void Draw(Model* model) const;
        void BindVertexBuffer(rendering::VertexBuffer *vertexBuffer) const;
        void BindFrameBuffer(unsigned int id) const;
        void BindTexture2D(int number, int idTexture, int type) const;
    };
    
}
