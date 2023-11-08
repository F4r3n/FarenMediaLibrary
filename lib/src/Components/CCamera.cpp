#include "Components/CCamera.h"
#include "Rendering/RenderTexture.h"
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

using namespace fmc;
using namespace fm;



CCamera::CCamera()
{
    _name = "Camera";
	_width = 0;
	_height = 0;
	_fovy = 60.0f;
	_nearPlane = 0.1f;
	_farPlane = 1000.0f;
	_isOrto = false;
	_isAuto = true;
	_multiSampled = 0;

	_isInit = false;
	_ID++;
	_currentID = _ID;
}

CCamera::CCamera(size_t width, size_t height, fmc::RENDER_MODE mode, bool ortho, bool isAuto, int multiSampled)
	:
_isOrto(ortho),
_nearPlane(0.1f),
_farPlane(1000.0f),
_fovy(60.0f),
_width(width),
_height(height),
_multiSampled(multiSampled),
_isInit(false),
_isAuto(isAuto),
_target(nullptr)
{
	_name = "Camera";
	_ID++;
	_currentID = _ID;
}

CCamera::~CCamera()
{
	if (_rendererConfiguration.isInit)
	{
		_rendererConfiguration.uboLight->Free();
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
    _fovy					= inJSON["fovy"];
    _multiSampled			= inJSON["multiSampled"];
	_isAuto					= inJSON["isAuto"];

    return true;
}


void CCamera::_InitRenderTexture()
{

	fm::Format formats[] = { fm::Format::RGBA, fm::Format::RGBA };

	fm::Type types[] = { fm::Type::UNSIGNED_BYTE, fm::Type::UNSIGNED_BYTE };
	_renderTexture = fm::RenderTexture(_width, _height, 2, formats, types, 24, _multiSampled);
	
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
	if (!_isInit)
	{
		UpdateProjectionMatrix();

		_InitRenderTexture();
		_renderTexture.create();
		_isInit = true;
	}

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
		_rendererConfiguration.uboLight = std::make_unique<fm::OGLUniformbuffer>(fm::OGLUniformbuffer());
		//_rendererConfiguration.uboLight->Generate(sizeof(PointLight)*NUMBER_POINTLIGHT_MAX, 2);
		_rendererConfiguration.uboLight->Generate(sizeBytesLight, 2, GL_UNIFORM_BUFFER);
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
}

void CCamera::InitUniformBuffer()
{
	//glGenBuffers(1, &_rendererConfiguration.generatedBlockBinding);
	//glBindBuffer(GL_UNIFORM_BUFFER, _rendererConfiguration.generatedBlockBinding);
	//glBufferData(GL_UNIFORM_BUFFER,
	//			sizeof(shader_data),
	//			&shader_data,
	//			GL_DYNAMIC_COPY);
	//
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//
	//for (auto shader : fm::ResourcesManager::get().getAll<fm::Shader>())
	//{
	//	std::shared_ptr<fm::Shader> s = std::dynamic_pointer_cast<fm::Shader>(shader.second);
	//	s->Use();
	//
	//	glBindBufferBase(GL_UNIFORM_BUFFER,
	//					_rendererConfiguration.bindingPointIndex,
	//					_rendererConfiguration.generatedBlockBinding);
	//
	//	std::shared_ptr<OGLShader> glshader = std::dynamic_pointer_cast<OGLShader>(s);
	//	glshader->SetUniformBuffer("shader_data", _rendererConfiguration.bindingPointIndex);
	//}
}

void CCamera::UpdateUniformBufferCamera()
{
	//glBindBuffer(GL_UNIFORM_BUFFER, _rendererConfiguration.generatedBlockBinding);
	//GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	//memcpy(p, &shader_data, sizeof(shader_data));
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
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


void CCamera::ExecuteStartRendering() {
	if (_onStartRendering != nullptr)
	{
		_onStartRendering();
	}
}

void CCamera::ExecutePostRendering() {
	if (_onPostRendering != nullptr)
	{
		_onPostRendering();
	}
}
