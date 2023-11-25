#pragma once
#include "OGLUniformbuffer.hpp"
#include <memory>
namespace fmc
{
	class CCamera;
}

namespace fm
{
	class Graphics;

	class OGLFrameBuffer;
	class OGLCamera
	{
	public:
		OGLCamera(fmc::CCamera* inCamera);
		~OGLCamera();
		uint32_t GetID() const;
		void PrepareBuffer(size_t size);
		void SetBuffer(void* inData, size_t inSize);
		void BindBuffer();
		void CreateRenderTexture();
		void CheckStamp(fmc::CCamera* inCamera);

		void BindTarget(const fm::Graphics& inGraphics) const;
		void BindPostProcess() const;
		void BindInternal() const;
		std::shared_ptr<fm::OGLFrameBuffer> GetInternal() const;
		std::shared_ptr<fm::OGLFrameBuffer> GetTarget() const;
		std::shared_ptr<fm::OGLFrameBuffer> GetPostProcess() const;
	private:


		void _CreateInternalRenderTexture(fmc::CCamera* inCamera);
		fm::OGLUniformbuffer _shaderDataBuffer;
		uint32_t _ID = 0;
		uint32_t _cameraID = 0;
		std::shared_ptr<fm::OGLFrameBuffer>	_postProcessRenderTexture;
		std::shared_ptr<fm::OGLFrameBuffer>	_renderTexture;

		std::shared_ptr<fm::OGLFrameBuffer>	_target = nullptr;
		//fmc::CCamera*			_camera = nullptr;

		size_t	_currentStamp = 0;
	};
}