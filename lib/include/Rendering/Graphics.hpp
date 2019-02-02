#pragma once
#include <Core/Math/Vector4.h>
#include <Core/Rect.h>
#include <Rendering/VertexBuffer.hpp>
namespace fm {
	enum RENDERING_TYPE : unsigned int {
		BLEND = GL_BLEND,
		DEPTH_TEST = GL_DEPTH_TEST
    };

	enum BUFFER_BIT
	{
		COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT,
		DEPTH_BUFFER_BIT = GL_DEPTH_BUFFER_BIT,
		STENCIL_BUFFER_BIT = GL_STENCIL_BUFFER_BIT,
		NONE_BUFFER_BIT = 0
	};
    class Model;
    
    class Graphics {
      public :
        Graphics();
        ~Graphics();
        void clear(bool colorBuffer = true, bool depthBuffer = false, bool replaceColor = false) const;
        void setViewPort(const fm::math::vec4i &rect) const;
        void setViewPort(const fm::Rect<float> &rect) const;
        void enable(RENDERING_TYPE renderingTYPE) const;
        void disable(RENDERING_TYPE renderingTYPE) const;
        void draw(int primitiveType, unsigned int vertexCount, unsigned int* indices) const;
        void draw(int primitiveType, unsigned int vertexStart, unsigned int vertexCount) const;
        void draw(Model* model) const;
        void setVertexBuffer(rendering::VertexBuffer *vertexBuffer) const;
        void bindFrameBuffer(unsigned int id) const;
        void bindTexture2D(int number, int idTexture, int type) const;
    };
    
}
