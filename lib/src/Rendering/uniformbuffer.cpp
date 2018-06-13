#include "Rendering/uniformbuffer.hpp"
#include "Rendering/Shader.h"
#include <Core/Debug.h>
using namespace fm;

UniformBuffer::UniformBuffer() {}

UniformBuffer::~UniformBuffer() {}


void UniformBuffer::Generate(size_t size, int indexBuffer)
{
    _bindingPoint = indexBuffer;
    glGenBuffers(1, &_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _ubo, 0,size);

    fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);
}

void UniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
}

void UniformBuffer::SetData(void *data, size_t size)
{
    fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

    glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

}

void UniformBuffer::LinkWithShader(Shader *shader, int index, const std::string &name)
{
    shader->SetUniformBuffer(name, _bindingPoint);
}

void UniformBuffer::Free()
{
    glDeleteBuffers(1, &_ubo);
}
