#pragma once

#include "GL/glew.h"
namespace fm
{
	enum class TextureFormat;
	enum class TextureType;
	enum class OGLTextureFormat
	{
		NONE,
		RED = GL_RED,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH_16 = GL_DEPTH_COMPONENT16,
		DEPTH_24 = GL_DEPTH_COMPONENT24,
		DEPTH_32 = GL_DEPTH_COMPONENT32,
		DEPTH_32F = GL_DEPTH_COMPONENT32F,
		DEPTH_STENCIL = GL_DEPTH_STENCIL,
		STENCIL = GL_STENCIL_INDEX
	};

	enum class OGLTextureType
	{
		NONE,
		FLOAT = GL_FLOAT,
		HALF_FLOAT = GL_HALF_FLOAT,
		UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
		UNSIGNED_24_8 = GL_UNSIGNED_INT_24_8
	};

	enum class OGLTextureKind
	{
		TEXTURE2D = GL_TEXTURE_2D,
		TEXTURE2D_MULTISAMPLED = GL_TEXTURE_2D_MULTISAMPLE
	};

	enum class OGLTextureFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum class OGLTextureWrapping
	{
		REPEAT = GL_REPEAT,
		CLAMP_BORDER = GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_EDGE = GL_CLAMP_TO_EDGE
	};

}

namespace fm
{
	fm::OGLTextureFormat	ConvertTextureFormatToOGL(fm::TextureFormat inFormat);
	fm::OGLTextureType		ConvertTextureTypeToOGL(fm::TextureType inFormat);
}
