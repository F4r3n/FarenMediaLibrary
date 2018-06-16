#pragma once
#include <Core/Math/Vector4.h>
#include <Core/Rect.h>
#include <Rendering/VertexBuffer.hpp>
namespace fm {
    enum RENDERING_TYPE : unsigned int{
        BLEND,
        DEPTH_TEST
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
