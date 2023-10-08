#include "OGLUniformbuffer.hpp"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/OGLShader.hpp"
#include <Core/Debug.h>
#include "GL/glew.h"

using namespace fm;

OGLUniformbuffer::OGLUniformbuffer() {}

OGLUniformbuffer::~OGLUniformbuffer() {}


void OGLUniformbuffer::Generate(size_t size, int indexBuffer)
{
    _bindingPoint = indexBuffer;
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _ubo, 0,size);

    fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);
}

void OGLUniformbuffer::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
}

void OGLUniformbuffer::SetData(void *data, size_t size) const
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OGLUniformbuffer::LinkWithShader(Shader *shader, int index, const std::string &name) const
{
    dynamic_cast<OGLShader*>(shader)->SetUniformBuffer(name, _bindingPoint);
}

void OGLUniformbuffer::Free()
{
    glDeleteBuffers(1, &_ubo);
}