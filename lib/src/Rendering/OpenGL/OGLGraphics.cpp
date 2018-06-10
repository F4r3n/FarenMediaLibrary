#include <Rendering/Graphics.hpp>
#include <Core/Config.h>
#include <Rendering/OpenGL/OGLGraphicsDef.hpp>
#include "Rendering/Model.hpp"
using namespace fm;
using namespace rendering;
Graphics::Graphics() {
}
Graphics::~Graphics() {
}

void Graphics::clear(bool colorBuffer,
                     bool depthBuffer,
                     bool replaceColor) const {
    if(replaceColor) {
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    }
    GLbitfield mask = 0;
    if(colorBuffer)
        mask |= GL_COLOR_BUFFER_BIT;
    if(depthBuffer)
        mask |= GL_DEPTH_BUFFER_BIT;
    glClear(mask);
}

void Graphics::setViewPort(const fm::math::vec4i& rect) const {
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

void Graphics::setViewPort(const fm::Rect<float>& rect) const {
    glViewport(rect.x, rect.y, rect.w, rect.h);
}

void Graphics::enable(RENDERING_TYPE r) {
    glEnable(renderingType[r]);
}
void Graphics::disable(RENDERING_TYPE r) {
    glDisable(renderingType[r]);
}

void Graphics::draw(int primitiveType,
                    unsigned int vertexCount,
                    unsigned int* indices) {
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, indices);
}

void Graphics::draw(int primitiveType,
                    unsigned int vertexStart,
                    unsigned int vertexCount) {
    glDrawArrays(GL_TRIANGLES, vertexStart, vertexCount);
    glBindTexture(GL_TEXTURE_2D, 0);
}
// TODO VAO needed with opengl CORE and not ES
void Graphics::setVertexBuffer(VertexBuffer* vertexBuffer) {
    vertexBuffer->prepareData();
}

void Graphics::bindFrameBuffer(unsigned int id) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::bindTexture2D(int number, int idTexture, int type) {
    glActiveTexture(GL_TEXTURE0 + number);
    glBindTexture(type, idTexture);
}

void Graphics::draw(Model* model) {
    setVertexBuffer(model->vertexBuffer);

    draw(0,
         model->meshContainer->listIndices.size(),
         model->meshContainer->listIndices.data());
}
