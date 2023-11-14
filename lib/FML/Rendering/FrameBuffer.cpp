#include "FrameBuffer.hpp"
#include "RenderTexture.hpp"
using namespace fm;



FrameBuffer::~FrameBuffer()
{

}

FrameBuffer::FrameBuffer(size_t width, size_t height, const std::vector<TextureFormat>& formats, const std::vector<TextureType>& types, size_t depth, size_t multiSampling)
{
	_width = width;
	_height = height;
	_depth = depth;
	_formats = formats;
	_types = types;
	_numberColors = _formats.size();
	_multiSampling = multiSampling;
}

FrameBuffer::FrameBuffer(const RenderTexture& renderTexture, size_t multiSampling)
{
	_width = renderTexture.GetWidth();
	_height = renderTexture.GetHeight();
	_depth = renderTexture.GetDepth();
	_numberColors = renderTexture.GetNbColors();
	_multiSampling = renderTexture.GetMultiSampling();
	_formats = renderTexture.GetTexturesFormat();
	_types = renderTexture.GetTexturesType();
	_numberColors = _formats.size();

}

