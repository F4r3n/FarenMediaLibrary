#pragma once
#include "Resource/Resource.h"
#include <vector>
namespace fm
{
	enum class TextureFormat;
	enum class TextureType;

	class RenderTexture : public Resource
	{
	public:
		RenderTexture(const fm::FilePath& inPath);
		RenderTexture(size_t width, size_t height, const std::vector<TextureFormat>& formats,
			const std::vector<TextureType>& types, size_t depth, size_t multiSampling = 0);

		virtual void Load(const nlohmann::json& inJSON) override;
		virtual void Save(nlohmann::json& outJSON) const override;
		virtual RESOURCE_TYPE GetType() const override{ return RESOURCE_TYPE::RENDER_TEXTURE; }

		size_t GetWidth() const{ return _width; }
		size_t GetHeight() const { return _height;}
		size_t GetDepth() const { return _depth; }
		size_t GetNbColors() const { return _numberColors; }
		size_t GetMultiSampling() const { return _multiSampling; }

		const std::vector<TextureFormat>& GetTexturesFormat() const { return _formats; }
		const std::vector<TextureType>& GetTexturesType() const { return _types; }

	private:
		size_t	_width = 0;
		size_t	_height = 0;
		size_t	_depth = 0;
		size_t	_numberColors = 0;
		size_t	_multiSampling = 0;
		std::vector<TextureFormat>	_formats;
		std::vector<TextureType>	_types;
	public:
		uint32_t GetID() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;
	};

	std::shared_ptr<fm::RenderTexture> CreateRenderTexture(size_t width, size_t height, size_t multismapled);

}