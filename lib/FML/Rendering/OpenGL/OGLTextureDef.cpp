#include "OGLTextureDef.hpp"
#include "Rendering/TextureDef.hpp"

fm::OGLTextureFormat fm::ConvertTextureFormatToOGL(TextureFormat inFormat)
{
	switch (inFormat)
	{
	case TextureFormat::RED:
		return OGLTextureFormat::RED;
		case TextureFormat::RGB:
			return OGLTextureFormat::RGB;
		case TextureFormat::RGBA:
			return OGLTextureFormat::RGBA;
		case TextureFormat::DEPTH_16:
			return OGLTextureFormat::DEPTH_16;
		case TextureFormat::DEPTH_24:
			return OGLTextureFormat::DEPTH_24;
		case TextureFormat::DEPTH_32:
			return OGLTextureFormat::DEPTH_32;
		case TextureFormat::DEPTH_32F:
			return OGLTextureFormat::DEPTH_32F;
		case TextureFormat::DEPTH_STENCIL:
			return OGLTextureFormat::DEPTH_STENCIL;
		case TextureFormat::STENCIL:
			return OGLTextureFormat::STENCIL;
	}

	return OGLTextureFormat::NONE;
}


fm::OGLTextureType	fm::ConvertTextureTypeToOGL(TextureType inType)
{
	switch (inType)
	{
	case fm::TextureType::FLOAT:
		return OGLTextureType::FLOAT;
	case fm::TextureType::HALF_FLOAT:
		return OGLTextureType::HALF_FLOAT;
	case fm::TextureType::UNSIGNED_BYTE:
		return OGLTextureType::UNSIGNED_BYTE;
	case fm::TextureType::UNSIGNED_24_8:
		return OGLTextureType::UNSIGNED_24_8;
	}

	return OGLTextureType::NONE;

}
