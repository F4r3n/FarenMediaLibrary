#include "Components/CCamera.h"
#include "Rendering/RenderTexture.hpp"
#include <EntityManager.h>
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/OpenGL/OGLUniformbuffer.hpp"
#include "Rendering/Shader.h"
#include <Core/Debug.h>
#include "Rendering/OpenGL/OGLShader.hpp"
#include "Rendering/OpenGL/OGLTexture.hpp"

#include "Rendering/commandBuffer.hpp"
#include <nlohmann/json.hpp>
#include "Rendering/TextureDef.hpp"
using namespace fmc;
using namespace fm;



CCamera::CCamera()
{
    _name = "Camera";
	_fovy = 60.0f;
	_nearPlane = 0.1f;
	_farPlane = 1000.0f;
	_isOrto = false;
	_isAuto = true;
	_multiSampled = 0;

	_isInit = false;
}

CCamera::CCamera(size_t width, size_t height, fmc::RENDER_MODE mode, bool ortho, bool isAuto, int multiSampled)
	:
_isOrto(ortho),
_nearPlane(0.1f),
_farPlane(1000.0f),
_fovy(60.0f),
_multiSampled(multiSampled),
_isInit(false),
_isAuto(isAuto),
_target(nullptr)
{
	_viewPort.w = width;
	_viewPort.h = height;
	_name = "Camera";
}

CCamera::~CCamera()
{
	if (_destroyCallback != nullptr)
		_destroyCallback();
}

bool CCamera::Serialize(nlohmann::json &ioJson) const
{
    ioJson["viewPort"] = _viewPort;
    ioJson["farPlane"] = _farPlane;
    ioJson["nearPlane"] = _nearPlane;
    ioJson["isOrtho"] = _isOrto;
    ioJson["fovy"] = _fovy;
    ioJson["multiSampled"] = _multiSampled;
	ioJson["isAuto"] = _isAuto;

    return true;
}

bool CCamera::Read(const nlohmann::json &inJSON)
{
	if (inJSON.contains("width"))
	{
		_viewPort.w = inJSON["width"];
	}
	if (inJSON.contains("height"))
	{
		_viewPort.h = inJSON["height"];
	}
	if (inJSON.contains("viewPort"))
	{
		_viewPort = inJSON["viewPort"];
	}

    _farPlane				= inJSON["farPlane"];
    _nearPlane				= inJSON["nearPlane"];
    _isOrto					= inJSON["isOrtho"];
    _fovy					= inJSON["fovy"];
    _multiSampled			= inJSON["multiSampled"];
	_isAuto					= inJSON["isAuto"];

    return true;
}

fm::math::mat CCamera::GetOrthographicProjectionForText() const
{
	return fm::math::ortho(0.0f, (float)_viewPort.w, 0.0f, (float)_viewPort.h, -_farPlane, _farPlane);
}


void CCamera::UpdateProjectionMatrix()
{
	if (_isOrto)
	{
		_projection = fm::math::ortho(0.0f, (float)_viewPort.w, (float)_viewPort.h, 0.0f, _nearPlane, _farPlane);
	}
	else
	{
		_projection = fm::math::perspective(fm::math::radians(_fovy), (float)_viewPort.w / (float)_viewPort.h, _nearPlane, _farPlane);
	}
}


void CCamera::SetNewProjection(int width, int height)
{
    _isOrto = true;
    _farPlane = 100.0f;

    _projection = fm::math::ortho(0.0f, (float)width, (float)height, 0.0f, _nearPlane, _farPlane);
    _viewPort.w = width;
    _viewPort.h = height;
    _viewPort.x = 0;
    _viewPort.y = 0;
}


void CCamera::_InitRenderTexture()
{
	std::vector<fm::TextureFormat> formats{ fm::TextureFormat::RGBA, fm::TextureFormat::RGBA };
	std::vector<fm::TextureType> types{ fm::TextureType::UNSIGNED_BYTE, fm::TextureType::UNSIGNED_BYTE };
	_renderTexture = std::make_shared<fm::RenderTexture>(_viewPort.w, _viewPort.h, formats, types, 24, _multiSampled);
	_Touch();
}

void CCamera::SetNewViewPort(size_t x, size_t y, size_t width, size_t height)
{
    _viewPort.w = width;
    _viewPort.h = height;
    _viewPort.x = x;
    _viewPort.y = y;
}

bool CCamera::HasCommandBuffer(fm::RENDER_QUEUE inQueue) const
{
	return _commandBuffers.find(inQueue) != _commandBuffers.cend();
}


void CCamera::AddCommandBuffer(fm::RENDER_QUEUE inQueue, const fm::CommandBuffer &inCommandBuffer)
{
	_commandBuffers[inQueue].push(inCommandBuffer);
}


void CCamera::UpdateViewMatrix(const fm::Transform &inTransform)
{
	if (_isOrto)
	{
		fm::math::mat m;
		m.identity();
		_viewMatrix = fm::math::translate(m, fm::math::vec3(-inTransform.position.x, -inTransform.position.y, -inTransform.position.z));
		_viewMatrix = fm::math::translate(_viewMatrix, fm::math::vec3(0.5f * _viewPort.w, 0.5f * _viewPort.h, 0.0f));
		_viewMatrix = fm::math::rotate(_viewMatrix, inTransform.rotation.GetEulerAngles().x, fm::math::vec3(0, 0, 1));
		_viewMatrix = fm::math::translate(_viewMatrix, fm::math::vec3(-0.5f * _viewPort.w, -0.5f * _viewPort.h, 0.0f));
	}
	else
	{
		_viewMatrix.identity();
		_viewMatrix = fm::math::rotate(_viewMatrix, inTransform.rotation.GetRotationMatrix());
		_viewMatrix = fm::math::translate(_viewMatrix, fm::math::vec3(-inTransform.position.x, -inTransform.position.y, inTransform.position.z));
	}
}




void CCamera::SetTarget(std::shared_ptr<fm::FrameBuffer> inFrameBuffer)
{
	_Touch();
	_target = inFrameBuffer;
}


void CCamera::ExecuteStartRendering()
{
	if (_onStartRendering != nullptr)
	{
		_onStartRendering();
	}
}

void CCamera::ExecutePostRendering()
{
	if (_onPostRendering != nullptr)
	{
		_onPostRendering();
	}
}
