#include "OGLCamera.hpp"
#include <GL/glew.h>
using namespace fm;

OGLCamera::OGLCamera(uint32_t inID) : _ID(inID)
{
}

OGLCamera::~OGLCamera()
{
	_shaderDataBuffer.Free();
}


void OGLCamera::PrepareBuffer(size_t size)
{
	_shaderDataBuffer.Generate(size, 0, GL_UNIFORM_BUFFER); //The binding point is always 0
}

void OGLCamera::SetBuffer(void* inData, size_t inSize)
{
	_shaderDataBuffer.SetData(inData, inSize);
}
void OGLCamera::BindBuffer()
{
	_shaderDataBuffer.Bind();
}