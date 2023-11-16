#include "OGLCamera.hpp"
#include <GL/glew.h>
#include "Components/CCamera.h"
#include "Rendering/TextureDef.hpp"
#include "OGLFrameBuffer.hpp"
#include "Rendering/Graphics.hpp"
using namespace fm;

OGLCamera::OGLCamera(fmc::CCamera* inCamera)
{
	_camera = inCamera;
	_currentStamp = _camera->GetStamp();
	std::vector<fm::TextureFormat> formats{ fm::TextureFormat::RGBA, fm::TextureFormat::RGBA };
	std::vector<fm::TextureType> types{ fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };

	_CreateInternalRenderTexture();

	_postProcessRenderTexture = std::make_shared<fm::OGLFrameBuffer>(
		_camera->GetWidth(), _camera->GetHeight(),
		formats,
		types, 1);
}

void OGLCamera::_CreateInternalRenderTexture()
{
	std::vector<fm::TextureFormat> formats{ fm::TextureFormat::RGBA, fm::TextureFormat::RGBA };
	std::vector<fm::TextureType> types{ fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };

	_renderTexture = std::make_shared<fm::OGLFrameBuffer>(
		_camera->GetWidth(), _camera->GetHeight(),
		formats, types, 24, _camera->GetMultiSampled());
}


void OGLCamera::CheckStamp()
{
	if (_currentStamp != _camera->GetStamp())
	{
		if (_camera->GetViewport().w != _renderTexture->GetWidth()
			|| _camera->GetViewport().h != _renderTexture->GetHeight())
		{
			std::vector<fm::TextureFormat> formats = { fm::TextureFormat::RGBA, fm::TextureFormat::RGBA };
			std::vector<fm::TextureType> types = { fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };

			_renderTexture = std::make_shared<fm::OGLFrameBuffer>(
				_camera->GetWidth(), _camera->GetHeight(),
				formats, types, 24, _camera->GetMultiSampled());
		}
	}
	if (_target == nullptr && _camera->GetTarget() != nullptr)
	{
		_target = std::dynamic_pointer_cast<fm::OGLFrameBuffer>(_camera->GetTarget());
		if (!_target->IsCreated())
		{
			_target->create();
		}
	}
	_currentStamp = _camera->GetStamp();
}

void OGLCamera::BindTarget(const fm::Graphics& inGraphics) const
{
	if (_target != nullptr)
	{
		_target->bind();
	}
	else
	{
		inGraphics.BindFrameBuffer(0);
	}
}

void OGLCamera::BindPostProcess() const
{
	_postProcessRenderTexture->bind();
}

void OGLCamera::BindInternal() const
{
	_renderTexture->bind();
}

std::shared_ptr<fm::OGLFrameBuffer> OGLCamera::GetInternal() const
{
	return _renderTexture;
}
std::shared_ptr<fm::OGLFrameBuffer> OGLCamera::GetTarget() const
{
	return _target;
}
std::shared_ptr<fm::OGLFrameBuffer> OGLCamera::GetPostProcess() const
{
	return _postProcessRenderTexture;
}

OGLCamera::~OGLCamera()
{
	_shaderDataBuffer.Free();
}


void OGLCamera::CreateRenderTexture()
{
	if (!_renderTexture->IsCreated())
	{
		_renderTexture->create();
	}

	if (!_postProcessRenderTexture->IsCreated())
	{
		_postProcessRenderTexture->create();
	}
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

uint32_t OGLCamera::GetID() const
{
	return _camera->GetObjectID();
}
