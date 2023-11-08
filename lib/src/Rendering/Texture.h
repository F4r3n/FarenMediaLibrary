#pragma once
#include "Core/FilePath.h"
#include "nlohmann/json_fwd.hpp"
#include "Resource/Resource.h"
namespace fm
{
	class Texture : public Resource
	{
	public:
		Texture() = default;
		Texture(const fm::FilePath& inPath);

		virtual void Load(const nlohmann::json& inJSON);
		virtual void Save(nlohmann::json& outJSON) const;
		virtual RESOURCE_TYPE GetType() const { return RESOURCE_TYPE::TEXTURE; }
	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;
	};
}