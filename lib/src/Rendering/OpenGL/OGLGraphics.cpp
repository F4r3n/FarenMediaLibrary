#include <Rendering/Graphics.hpp>
#include <Core/Config.h>
#include <Rendering/OpenGL/OGLGraphicsDef.hpp>
using namespace fm;
using namespace rendering;
Graphics::Graphics() {
}
Graphics::~Graphics() {
}

void Graphics::clear(bool colorBuffer, bool depthBuffer, bool replaceColor) const{
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

void Graphics::setViewPort(const fm::math::vec4i& rect) const{
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

void Graphics::setViewPort(const fm::Rect<float> &rect) const{
    glViewport(rect.x, rect.y, rect.w, rect.h);
}

void Graphics::enable(RENDERING_TYPE r) {
    glEnable(renderingType[r]);
}
void Graphics::disable(RENDERING_TYPE r) {
    glDisable(renderingType[r]);
}

void Graphics::draw(int primitiveType, unsigned int vertexCount, unsigned int* indices) {
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, indices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Graphics::draw(int primitiveType, unsigned int vertexStart, unsigned int vertexCount) {
    glDrawArrays(GL_TRIANGLES, vertexStart, vertexCount);
}


void Graphics::setIndexBuffer(VertexBuffer *vertexBuffer) {
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->index);
        // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // UV attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}

void Graphics::bindFrameBuffer(unsigned int id) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

