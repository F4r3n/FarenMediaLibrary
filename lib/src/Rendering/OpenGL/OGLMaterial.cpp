#include "OGLMaterial.hpp"
#include "Rendering/material.hpp"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/OGLShader.hpp"
#include "Resource/ResourcesManager.h"
using namespace fm;

OGLMaterial::OGLMaterial(const OGLMaterialCreateInfo& inInfo)
{
	_material = inInfo.material;
	_shader = inInfo.shader;
	auto info = _material->GetMaterialBufferInfo(GRAPHIC_API::OPENGL);
	_materialStamp = _material->GetStamp();
	if (info.buffer != nullptr)
	{
		_materialBuffer.Generate(info.bufferSize, info.bindingPoint, GL_UNIFORM_BUFFER);
		_materialBuffer.SetData(info.buffer, info.bufferSize);
	}

}


void OGLMaterial::Bind(const fm::MaterialProperties& inMaterialProperties)
{
	_shader->Use();
	for (auto const& [name, value] : _material->GetUniforms())
	{
		_shader->setValue(name, value);
	}

	for (auto const& [name, value] : inMaterialProperties)
	{
		_shader->setValue(name, value);
	}

	if (_materialBuffer.IsValid())
	{
		_materialBuffer.Bind();
		if (_materialStamp != _material->GetStamp())
		{
			_materialStamp = _material->GetStamp();
			_materialBuffer.SetData(_material->GetBufferPtr(), _material->GetBufferSize());
		}
	}

}

uint32_t OGLMaterial::GetID() const
{
	return _material->GetID();
}
