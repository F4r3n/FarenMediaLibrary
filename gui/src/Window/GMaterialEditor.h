#pragma once

#include "Window/GWindow.h"
#include <memory>
#include <vector>
#include <string>
#include "Rendering/Shader.h"
#include "Rendering/material.hpp"
namespace fm
{
	class GameObject;
	class Material;
}

namespace gui
{
	class GMaterialEditor : public GWindow
	{
	public:

		struct MaterialValueEditor
		{
			uint32_t			offset = 0;
			uint32_t			size = 0;
			std::string			name;
			fm::MaterialValue	value;
		};

		GMaterialEditor();
		~GMaterialEditor();
		void CustomDraw();

		void DrawMaterialInspector(std::shared_ptr<fm::Material> inMaterial);
		static std::vector<std::string> GetValueTypeNames();
		static fm::FilePath CreateNewMaterial(const fm::FilePath& inPath);
	private:
		void _Save(std::shared_ptr<fm::Material> inMaterial);
		void _Update(float dt, Context &inContext);
		void _CheckMaterial(std::shared_ptr<fm::Material> inMaterial);
		void _Init(std::shared_ptr<fm::Material> inCurrentMaterial);
		void _BlockIterator(const fm::SubShader::Reflection& reflection, const std::string& name, uint32_t& offset, const fm::SubShader::Variable& inVariable);

		std::weak_ptr<fm::Material>	_material;

		std::string		_currentShaderPath;
		bool			_textureCheckBox = false;
		fm::SHADER_KIND	_currentKind = fm::SHADER_KIND::PLAIN;
		fm::MaterialKind		_currentMaterialKind = fm::MaterialKind::STANDARD;

		std::unordered_map<std::string, MaterialValueEditor> _properties;
	};
}

