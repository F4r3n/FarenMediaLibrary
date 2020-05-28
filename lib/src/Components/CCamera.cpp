#include "Components/CCamera.h"
#include "Rendering/RenderTexture.h"
#include <EntityManager.h>
#include "Window.h"
#include "Resource/ResourcesManager.h"
#include "Rendering/uniformbuffer.hpp"
#include "Rendering/Shader.h"
#include <Core/Debug.h>
using namespace fmc;
using namespace fm;



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

CCamera::CCamera(size_t width, size_t height, fmc::RENDER_MODE mode, bool ortho, bool isAuto, int multiSampled)
	:
_nearPlane(0.1f),
_farPlane(1000.0f),
_fovy(60.0f),
_isInit(false),
_width(width),
_height(height),
_isOrto(ortho),
_multiSampled(multiSampled),
_isAuto(isAuto),
_target(nullptr)
{
	_name = "Camera";
	shader_data.render_mode = mode;
}

CCamera::~CCamera()
{
	if (_rendererConfiguration.isInit)
	{
		_rendererConfiguration.uboLight->Free();
	}
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

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
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

}

fm::math::mat CCamera::GetOrthographicProjectionForText() const
{
	return fm::math::ortho(0.0f, (float)_width, 0.0f, (float)_height, -_farPlane, _farPlane);
}


void CCamera::UpdateProjectionMatrix()
{
	if (_isOrto)
	{
		_projection = fm::math::ortho(0.0f, (float)_width, (float)_height, 0.0f, _nearPlane, _farPlane);
		_viewPort.w = (int)_width;
		_viewPort.h = (int)_height;
		_viewPort.x = 0;
		_viewPort.y = 0;

	}
	else
	{
		_projection = fm::math::perspective(fm::math::radians(_fovy), (float)_width / (float)_height, _nearPlane, _farPlane);
		_viewPort.w = (int)_width;
		_viewPort.h = (int)_height;
		_viewPort.x = 0;
		_viewPort.y = 0;

	}
}


void CCamera::Init()
{

	UpdateProjectionMatrix();

	_InitRenderTexture();
	_renderTexture.create();
	_isInit = true;

	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

}



void CCamera::SetNewProjection(int width, int height)
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


}

void CCamera::UpdateRenderTexture()
{

	if (_viewPort.w != _renderTexture.getWidth() || _viewPort.h != _renderTexture.getHeight())
	{
		_InitRenderTexture();
	}
    
}

void CCamera::SetNewViewPort(int x, int y, int width, int height)
{
    _isOrto = true;
    _projection = fm::math::ortho((float)x, (float)x + (float)width, (float)y + (float)height, (float)y, _nearPlane, _farPlane);
    _viewPort.w = width;
    _viewPort.h = height;
    _viewPort.x = x;
    _viewPort.y = y;

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


void CCamera::InitRenderConfig(const fm::Transform &inTransform, size_t sizeBytesLight)
{
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

	if ( !_rendererConfiguration.isInit)
	{
		_rendererConfiguration.uboLight = std::make_unique<fm::UniformBuffer>(fm::UniformBuffer());
		//_rendererConfiguration.uboLight->Generate(sizeof(PointLight)*NUMBER_POINTLIGHT_MAX, 2);
		_rendererConfiguration.uboLight->Generate(sizeBytesLight, 2);
		UpdateViewMatrix(inTransform);

		fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };
		fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
		_rendererConfiguration.lightRenderTexture = fm::RenderTexture(
			_renderTexture.getWidth(),
			_renderTexture.getHeight(),
			2,
			formats,
			types,
			0);

		_rendererConfiguration.postProcessRenderTexture = fm::RenderTexture(
			_renderTexture.getWidth(),
			_renderTexture.getHeight(),
			1,
			formats,
			types,
			0);


		_rendererConfiguration.lightRenderTexture.create();
		_rendererConfiguration.postProcessRenderTexture.create();

		_rendererConfiguration.isInit = true;
	}
	fm::Debug::logErrorExit(glGetError(), __FILE__, __LINE__);

}

void CCamera::InitUniformBuffer()
{
	glGenBuffers(1, &_rendererConfiguration.generatedBlockBinding);
	glBindBuffer(GL_UNIFORM_BUFFER, _rendererConfiguration.generatedBlockBinding);
	glBufferData(GL_UNIFORM_BUFFER,
				sizeof(shader_data),
				&shader_data,
				GL_DYNAMIC_COPY);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (auto shader : fm::ResourcesManager::get().getAll<fm::Shader>())
	{
		fm::Shader* s = dynamic_cast<fm::Shader*>(shader.second);
		s->Use();

		glBindBufferBase(GL_UNIFORM_BUFFER,
						_rendererConfiguration.bindingPointIndex,
						_rendererConfiguration.generatedBlockBinding);

		s->SetUniformBuffer("shader_data", _rendererConfiguration.bindingPointIndex);
	}
}

void CCamera::UpdateUniformBufferCamera()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _rendererConfiguration.generatedBlockBinding);
	GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &shader_data, sizeof(shader_data));
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void CCamera::UpdateRenderConfigBounds(const fm::Transform &inTransform)
{
	_rendererConfiguration.bounds.setSize(fm::math::vec3(static_cast<float>(_viewPort.w),
														 static_cast<float>(_viewPort.h),
														 static_cast<float>(_farPlane - _nearPlane)));
	_rendererConfiguration.bounds.setCenter(fm::math::vec3(inTransform.position.x, inTransform.position.y, -1) + _rendererConfiguration.bounds.getSize() / 2.0f);
	_rendererConfiguration.bounds.setScale(fm::math::vec3(1, 1, 1));
}

std::shared_ptr<fm::RenderTexture> CCamera::SetTarget(const fm::RenderTexture& inRenderTexture)
{
	_target = std::make_shared<fm::RenderTexture>(inRenderTexture);
	return _target;
}


std::shared_ptr<fm::RenderTexture> CCamera::SetTarget(fm::RenderTexture *inRenderTexture)
{
	if (inRenderTexture == nullptr)
	{
		_target = std::make_shared<fm::RenderTexture>(fm::RenderTexture(getInternalRenderTexture(), 0));
		return _target;
	}
	else
	{
		return SetTarget(*inRenderTexture);
	}
}

