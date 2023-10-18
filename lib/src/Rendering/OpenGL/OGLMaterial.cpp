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
}


void OGLMaterial::Bind(const fm::MaterialProperties& inMaterialProperties)
{
	_shader->Use();
	for (auto const& value : _material->GetProperties())
	{
		_shader->setValue(value.name, value.materialValue);
	}

	for (auto const& value : inMaterialProperties)
	{
		_shader->setValue(value.name, value.materialValue);
	}
}

uint32_t OGLMaterial::GetID() const
{
	return _material->GetID();
}
