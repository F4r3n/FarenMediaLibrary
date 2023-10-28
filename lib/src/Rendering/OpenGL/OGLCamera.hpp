#pragma once
#include "OGLUniformbuffer.hpp"


namespace fm
{
	class OGLCamera
	{
	public:
		OGLCamera(uint32_t inID);
		~OGLCamera();
		uint32_t GetID() const { return _ID; }
		void PrepareBuffer(size_t size);
		void SetBuffer(void* inData, size_t inSize);
		void BindBuffer();
	private:
		fm::OGLUniformbuffer _shaderDataBuffer;
		uint32_t _ID = 0;
	};
}