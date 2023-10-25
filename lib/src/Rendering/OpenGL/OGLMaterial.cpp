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
	for (auto const& [name, value] : _material->GetProperties())
	{
		_shader->setValue(name, value);
	}

	for (auto const& [name, value] : inMaterialProperties)
	{
		_shader->setValue(name, value);
	}
}

uint32_t OGLMaterial::GetID() const
{
	return _material->GetID();
}
