#include "OGLUniformbuffer.hpp"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/OGLShader.hpp"
#include <Core/Debug.h>
#include "GL/glew.h"
#include <cassert>
using namespace fm;

OGLUniformbuffer::OGLUniformbuffer() {}

OGLUniformbuffer::~OGLUniformbuffer() {}


void OGLUniformbuffer::Generate(size_t size, int indexBuffer, int bufferType)
{
	_bufferType = bufferType; //GL_UNIFORM_BUFFER || GL_SHADER_STORAGE_BUFFER
    _bindingPoint = indexBuffer;
    glGenBuffers(1, &_ubo);
    glBindBuffer(_bufferType, _ubo);
    glBufferData(_bufferType, size, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(_bufferType, 0);
    glBindBufferRange(_bufferType, _bindingPoint, _ubo, 0,size);

    fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);
}

void OGLUniformbuffer::SetBindingPoint(uint32_t inBindingPoint)
{
	_bindingPoint = inBindingPoint;
	glBindBufferBase(_bufferType, inBindingPoint, _ubo);
}


void OGLUniformbuffer::Bind() const
{
	assert(_ubo > 0);
    glBindBuffer(_bufferType, _ubo);
}

void OGLUniformbuffer::SetData(void *data, size_t size) const
{
	assert(_ubo > 0);
	if (_bufferType == GL_UNIFORM_BUFFER)
	{
		glBindBuffer(_bufferType, _ubo);
		glBufferSubData(_bufferType, 0, size, data);
		glBindBuffer(_bufferType, 0);
	}
	else
	{
		glBindBuffer(_bufferType, _ubo);
		GLvoid* p = glMapBuffer(_bufferType, GL_WRITE_ONLY);
		memcpy(p, data, size);
		glUnmapBuffer(_bufferType);
	}

}

void OGLUniformbuffer::LinkWithShader(Shader *shader, int index, const std::string &name) const
{
    dynamic_cast<OGLShader*>(shader)->SetUniformBuffer(name, _bindingPoint);
}

void OGLUniformbuffer::Free()
{
	assert(_ubo > 0);
    glDeleteBuffers(1, &_ubo);
}
