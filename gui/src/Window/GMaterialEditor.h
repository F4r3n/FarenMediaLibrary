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
	struct MaterialValueInfo;
	class Texture2D;
}

namespace gui
{
	class GMaterialEditor : public GWindow
	{
	public:

		struct MaterialValueEditor
		{
			fm::MaterialValueInfo info;
			fm::MaterialValue	  value;
		};

		GMaterialEditor();
		~GMaterialEditor();
		void CustomDraw();

		void DrawMaterialInspector(std::shared_ptr<fm::Material> inMaterial);
		static std::vector<std::string> GetValueTypeNames();
		static fm::FilePath CreateNewMaterial(const fm::FilePath& inPath);
	private:
		void _TextureViewer(std::shared_ptr<fm::Material> inMaterial, fm::STANDARD_MATERIAL_PROPERTIES_TEXTURE inKind);
		void _Save(std::shared_ptr<fm::Material> inMaterial);
		void _Update(float dt, Context &inContext);
		void _CheckMaterial(std::shared_ptr<fm::Material> inMaterial);
		void _Init(std::shared_ptr<fm::Material> inCurrentMaterial);

		std::weak_ptr<fm::Material>	_material;

		std::string		_currentShaderPath;
		fm::SHADER_KIND	_currentKind = fm::SHADER_KIND::PLAIN;
		fm::MaterialKind		_currentMaterialKind = fm::MaterialKind::STANDARD;

		std::unordered_map<std::string, MaterialValueEditor> _properties;
		std::unordered_map<uint32_t, std::shared_ptr<fm::Texture2D>> _textures;

	};
}

