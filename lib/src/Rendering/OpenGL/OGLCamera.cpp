#include "OGLCamera.hpp"

using namespace fm;

OGLCamera::OGLCamera(uint32_t inID) : _ID(inID)
{
}

void OGLCamera::PrepareBuffer(size_t size)
{
	_shaderDataBuffer.Generate(size, 0); //The binding point is always 0
}

void OGLCamera::SetBuffer(void* inData, size_t inSize)
{
	_shaderDataBuffer.SetData(inData, inSize);
}
void OGLCamera::BindBuffer()
{
	_shaderDataBuffer.Bind();
}