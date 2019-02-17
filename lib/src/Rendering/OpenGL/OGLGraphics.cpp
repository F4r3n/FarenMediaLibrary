#include <Rendering/Graphics.hpp>
#include <Core/Config.h>
#include <Rendering/OpenGL/OGLGraphicsDef.hpp>
#include "Rendering/Model.hpp"
using namespace fm;
using namespace rendering;
Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Clear(BUFFER_BIT inOption) const
{
    glClear(inOption);
}

void Graphics::SetViewPort(const fm::math::vec4i& rect) const
{
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

void Graphics::SetViewPort(const fm::Rect<float>& rect) const
{
    glViewport(rect.x, rect.y, rect.w, rect.h);
}

void Graphics::Enable(RENDERING_TYPE r) const
{
    glEnable(r);
}
void Graphics::Disable(RENDERING_TYPE r) const
{
    glDisable(r);
}

void Graphics::Draw(int primitiveType,
                    unsigned int vertexCount,
                    unsigned int* indices) const
{
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, indices);
}

void Graphics::Draw(int primitiveType,
                    unsigned int vertexStart,
                    unsigned int vertexCount) const
{
    glDrawArrays(GL_TRIANGLES, vertexStart, vertexCount);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Graphics::Draw(Model* model) const
{
    model->PrepareBuffer();
    for(size_t i = 0; i < model->GetNumberMeshes(); ++i)
    {
        fm::rendering::MeshContainer* mesh = model->GetMeshContainer(i);
        Draw(0, mesh->listIndices.size(), mesh->listIndices.data());
    }

}

// TODO VAO needed with opengl CORE and not ES
void Graphics::BindVertexBuffer(VertexBuffer* vertexBuffer) const
{
    vertexBuffer->prepareData();
}

void Graphics::BindFrameBuffer(unsigned int id) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::BindTexture2D(int number, int idTexture, int type) const
{
    glActiveTexture(GL_TEXTURE0 + number);
    glBindTexture(type, idTexture);
}


