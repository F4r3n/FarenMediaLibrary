#include "OGLMaterial.hpp"
#include "Rendering/material.hpp"
#include "Rendering/Shader.h"
#include "Rendering/OpenGL/OGLShader.hpp"
#include "Resource/ResourcesManager.h"
#include "OGLTextureCache.hpp"
#include "OGLTexture.hpp"
using namespace fm;

OGLMaterial::OGLMaterial(const OGLMaterialCreateInfo& inInfo, fm::OGLTextureCache& inTextures)
{
	_material = inInfo.material;
	_shader = inInfo.shader;
	auto info = _material->GetMaterialBufferInfo_Buffer(GRAPHIC_API::OPENGL);
	_materialStamp = _material->GetStamp();
	if (info.buffer != nullptr)
	{
		_materialBuffer.Generate(info.bufferSize, info.info.bindingPoint, GL_UNIFORM_BUFFER);
		_materialBuffer.SetData(info.buffer, info.bufferSize);
	}

	_texturesInfo = _material->GetMaterialBufferInfo_Textures(GRAPHIC_API::OPENGL);
	for (const auto& texture : _texturesInfo)
	{
		if (auto t = texture.texture.lock())
		{
			inTextures.FindOrCreateTexture(t); //upload textures;
		}
		else//blank
		{

		}
	}

}


void OGLMaterial::Bind(const fm::MaterialValues& inMaterialProperties, fm::OGLTextureCache& inTextures)
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
		for (const auto& texture : _texturesInfo)
		{
			if (auto t = texture.texture.lock())
			{
				inTextures.FindOrCreateTexture(t)->bind(texture.info.bindingPoint); //upload textures;
			}
			else//blank
			{
				//inTextures.GetBlank()->bind(texture.info.bindingPoint);
			}
		}
		if (_materialStamp != _material->GetStamp())
		{
			_materialStamp = _material->GetStamp();
			_materialBuffer.SetData(_material->GetBufferPtr(), _material->GetBufferSize());
			_texturesInfo = _material->GetMaterialBufferInfo_Textures(GRAPHIC_API::OPENGL);

			for (const auto& texture : _texturesInfo)
			{
				if (auto t = texture.texture.lock())
				{
					inTextures.FindOrCreateTexture(t);
				}
			}
		}
	}
}

void OGLMaterial::Destroy()
{
	_materialBuffer.Free();
}


uint32_t OGLMaterial::GetID() const
{
	return _material->GetID();
}
