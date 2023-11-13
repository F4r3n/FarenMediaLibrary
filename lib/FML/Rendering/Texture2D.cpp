#include "Texture2D.hpp"
#include <cassert>
#include "OpenGL/OGLTexture.hpp"
#include "Rendering/Image.h"

using namespace fm;

Texture2D::Texture2D(std::shared_ptr<fm::OGLTexture> inTexture)
{
	_oglTexture = inTexture;
}

Texture2D::~Texture2D()
{

}

unsigned int Texture2D::GetID() const
{
	if (_oglTexture != nullptr)
	{
		return _oglTexture->getID();
	}
	return 0;
}

size_t	Texture2D::GetWidth() const
{
	if (_oglTexture != nullptr)
	{
		return _oglTexture->getWidth();
	}
	return 0;
}

size_t	Texture2D::GetHeight() const
{
	if (_oglTexture != nullptr)
	{
		return _oglTexture->getHeight();
	}
	return 0;
}

std::shared_ptr<Texture2D> Texture2D::CreateTexture2D(GRAPHIC_API inAPI, const fm::FilePath& inPath)
{
	switch (inAPI)
	{
	case OPENGL:
	{
		fm::Image image(inPath);
		if (image.LoadImage())
		{
			std::shared_ptr<fm::OGLTexture> texture = std::make_shared<fm::OGLTexture>(image);

			return std::make_shared<Texture2D>(texture);
		}
		return nullptr;
	}
		break;
	default:
		assert(false);
		break;
	}
	return nullptr;
}
