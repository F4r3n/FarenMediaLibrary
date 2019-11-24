#include "Components/CCamera.h"
#include "Rendering/RenderTexture.h"
#include <EntityManager.h>
#include "Window.h"
using namespace fmc;
using namespace fm;

bool CCamera::IsOrthographic()
{
    return _isOrto;
}

float CCamera::GetFarPlane()
{
    return _farPlane;
}

float CCamera::GetNearPlane()
{
    return _nearPlane;
}

CCamera::CCamera()
{
    _name = "Camera";
	_width = fm::Window::kWidth;
	_height = fm::Window::kHeight;
	_fovy = 60.0f;
	_nearPlane = 0.1f;
	_farPlane = 1000.0f;
	_isOrto = false;
	_isAuto = true;
	_multiSampled = 0;
	shader_data.render_mode = fmc::RENDER_MODE::FORWARD;

	_isInit = false;
}

CCamera::CCamera(int width, int height, fmc::RENDER_MODE mode, bool ortho, bool isAuto, int multiSampled)
{
	_name = "Camera";
	_nearPlane = 0.1f;
	_farPlane = 1000.0f;
	_fovy = 60.0f;
	_isInit = false;

	_width = width;
	_height = height;
	shader_data.render_mode = mode;
	_isOrto = ortho;
	_multiSampled = multiSampled;
	_isAuto = isAuto;

}

CCamera::~CCamera()
{
	if (_renderTexture.isCreated())
	{
		_renderTexture.release();
	}

	if (_rendererConfiguration.isInit)
	{
		if (_rendererConfiguration.lightRenderTexture.isCreated())
		{
			_rendererConfiguration.lightRenderTexture.release();
		}
		if (_rendererConfiguration.postProcessRenderTexture.isCreated())
		{
			_rendererConfiguration.postProcessRenderTexture.release();
		}
	}
}

bool CCamera::Serialize(nlohmann::json &ioJson) const
{
    ioJson["width"] = _viewPort.w;
    ioJson["height"] = _viewPort.h;
    ioJson["farPlane"] = _farPlane;
    ioJson["nearPlane"] = _nearPlane;
    ioJson["isOrtho"] = _isOrto;
    ioJson["fovy"] = _fovy;
    ioJson["Render_Mode"] = shader_data.render_mode;
    ioJson["multiSampled"] = _multiSampled;
	ioJson["isAuto"] = _isAuto;

    return true;
}

bool CCamera::Read(const nlohmann::json &inJSON)
{
    _width					= inJSON["width"];
    _height					= inJSON["height"];
    _farPlane				= inJSON["farPlane"];
    _nearPlane				= inJSON["nearPlane"];
    _isOrto					= inJSON["isOrtho"];
    shader_data.render_mode = inJSON["Render_Mode"];
    _fovy					= inJSON["fovy"];
    _multiSampled			= inJSON["multiSampled"];
	_isAuto					= inJSON["isAuto"];

    return true;
}

void CCamera::Destroy()
{
    EntityManager::get().removeComponent<CCamera>(BaseComponent::_IDEntity);
}

void CCamera::_InitRenderTexture()
{
	if (shader_data.render_mode == fmc::RENDER_MODE::DEFERRED)
	{
		fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA, fm::Format::RGB };
#if OPENGL_ES_VERSION > 2
		fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::HALF_FLOAT };
#else
		fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
#endif
		_renderTexture = fm::RenderTexture(_width, _height, 3, formats, types, 24, _multiSampled);
	}
	else if (shader_data.render_mode == fmc::RENDER_MODE::FORWARD)
	{
		fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

		fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
		_renderTexture = fm::RenderTexture(_width, _height, 2, formats, types, 24, _multiSampled);
	}
}

void CCamera::UpdateProjectionMatrix()
{
	if (_isOrto)
	{
		_projection = fm::math::ortho(0.0f, (float)_width, (float)_height, 0.0f, _nearPlane, _farPlane);
		_viewPort.w = static_cast<float>(_width);
		_viewPort.h = static_cast<float>(_height);
		_viewPort.x = 0;
		_viewPort.y = 0;

	}
	else
	{
		_projection = fm::math::perspective(fm::math::radians(_fovy), (float)_width / (float)_height, _nearPlane, _farPlane);
		_viewPort.w = static_cast<float>(_width);
		_viewPort.h = static_cast<float>(_height);
		_viewPort.x = 0;
		_viewPort.y = 0;

	}
}


void CCamera::Init()
{

	UpdateProjectionMatrix();

	if (_renderTexture.isCreated())
	{
		_renderTexture.release();
	}

	_InitRenderTexture();
	_renderTexture.create();
	_isInit = true;
}



void CCamera::SetNewProjection(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;
    _isOrto = true;
    _farPlane = 100.0f;

    _projection = fm::math::ortho(0.0f, (float)width, (float)height, 0.0f, _nearPlane, _farPlane);
    _viewPort.w = width;
    _viewPort.h = height;
    _viewPort.x = 0;
    _viewPort.y = 0;
    if(_renderTexture.isCreated())
    {
        _renderTexture.release();
    }

}

void CCamera::UpdateRenderTexture()
{

	if (_viewPort.w != _renderTexture.getWidth() || _viewPort.h != _renderTexture.getHeight())
	{
		_renderTexture.release();
		_InitRenderTexture();
	}
    
}

void CCamera::SetNewViewPort(int x, int y, unsigned int width, unsigned int height)
{
    _isOrto = true;
    _projection = fm::math::ortho((float)x, (float)x + (float)width, (float)y + (float)height, (float)y, _nearPlane, _farPlane);
    _viewPort.w = static_cast<float>(width);
    _viewPort.h = static_cast<float>(height);
    _viewPort.x = static_cast<float>(x);
    _viewPort.y = static_cast<float>(y);
    if( _renderTexture.isCreated())
    {
        _renderTexture.release();
    }
	_InitRenderTexture();

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

void CCamera::UpdateShaderData()
{
	shader_data.FM_PV = _projection * _viewMatrix;
	shader_data.FM_P = _projection;
	shader_data.FM_V = _viewMatrix;
}


