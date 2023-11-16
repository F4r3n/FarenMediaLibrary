#pragma once
#include "Core/FilePath.h"
#include "nlohmann/json_fwd.hpp"
#include "Resource/Resource.h"
#include "Object.hpp"
namespace fm
{
	class Texture : public Resource, public Object<Texture>
	{
	public:
		Texture() = default;
		Texture(const fm::FilePath& inPath);

		virtual void Load(const nlohmann::json& inJSON);
		virtual void Save(nlohmann::json& outJSON) const;
		virtual RESOURCE_TYPE GetType() const { return RESOURCE_TYPE::TEXTURE; }
		static fm::RESOURCE_TYPE getType() { return fm::RESOURCE_TYPE::TEXTURE; }

	private:
		size_t				_mipmapLevel = 0;
		size_t				_canalNumber = 4;
	};
}