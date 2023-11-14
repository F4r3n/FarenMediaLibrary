#pragma once

namespace fm
{
	enum class TextureFormat;
	enum class TextureType;
	class RenderTexture;
	class FrameBuffer
	{
	public:
		FrameBuffer(size_t width, size_t height, const std::vector<TextureFormat>& formats, const std::vector<TextureType>& types, size_t depth, size_t multiSampling);
		FrameBuffer(const RenderTexture& renderTexture, size_t multiSampling);
		virtual ~FrameBuffer();

		bool IsMultiSampled() const { return _multiSampling > 0; }
		size_t GetWidth() const { return _width; }
		size_t GetHeight() const { return _height; }
		bool IsCreated() const { return _created; }

	protected:
		bool	_created = false;
		size_t _width = 0;
		size_t _height = 0;
		size_t _depth = 0;
		size_t _numberColors = 0;
		size_t	_multiSampling = 0;
		std::vector<TextureFormat> _formats;
		std::vector<TextureType> _types;
	};

}