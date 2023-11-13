#pragma once
#include "GraphicsAPI.h"
#include <memory>
namespace fm
{
	class FilePath;
	class OGLTexture;
	class Texture2D
	{
	public:
		static std::shared_ptr<Texture2D> CreateTexture2D(GRAPHIC_API inAPI, const fm::FilePath& inPath);
		~Texture2D();

		unsigned int GetID() const;
		size_t		 GetWidth() const;
		size_t		 GetHeight() const;

		Texture2D(std::shared_ptr<fm::OGLTexture> inTexture);

	private:

		std::shared_ptr<fm::OGLTexture> _oglTexture;
	};
}