#pragma once


namespace fm
{
	enum class TextureFormat
	{
		RED,
		RGB,
		RGBA,
		DEPTH_16,
		DEPTH_24,
		DEPTH_32,
		DEPTH_32F,
		DEPTH_STENCIL,
		STENCIL
	};

	enum class TextureType
	{
		FLOAT,
		HALF_FLOAT,
		UNSIGNED_BYTE,
		UNSIGNED_24_8
	};

	enum class TextureKind
	{
		TEXTURE2D,
		TEXTURE2D_MULTISAMPLED
	};

	enum class TextureFilter
	{
		NEAREST,
		LINEAR
	};

	enum class TextureWrapping
	{
		REPEAT,
		CLAMP_BORDER,
		MIRRORED_REPEAT,
		CLAMP_EDGE
	};
}