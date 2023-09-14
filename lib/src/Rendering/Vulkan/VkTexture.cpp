#include "Rendering/Vulkan/VkTexture.h"
#include "Core/FilePath.h"
#include "Core/Debug.h"
#include "Rendering/Image.h"

using namespace fm;

VkTexture::VkTexture(Vulkan* inVulkan, std::function<void(std::function<void(VkCommandBuffer cmd)>)>&& inSubmit)
{
	_vulkan = inVulkan;
	_submitBuffer = inSubmit;
}

bool VkTexture::UploadImage(const fm::FilePath& inPath)
{
	Image image;
	
	if (!image.loadImage(inPath.GetPathString())) {
		fm::Debug::get().LogError("Failed to load texture file " + inPath.GetPathString());
		return false;
	}

	fm::AllocatedBuffer stagingBuffer = _vulkan->CreateBuffer(image.GetDataSize(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
	_vulkan->MapBuffer(stagingBuffer, (void*)image.getImagePtr(), image.GetDataSize(), 0);


	VkExtent3D imageExtent;
	imageExtent.width = static_cast<uint32_t>(image.getSize().x);
	imageExtent.height = static_cast<uint32_t>(image.getSize().y);
	imageExtent.depth = 1;

	VkFormat image_format = VK_FORMAT_R8G8B8A8_SRGB;

	VkImageCreateInfo dimg_info = vk_init::CreateImageInfo(image_format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);

	fm::AllocatedImage newImage;

	VmaAllocationCreateInfo dimg_allocinfo = {};
	dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	vmaCreateImage(_vulkan->GetAllocator(), &dimg_info, &dimg_allocinfo, &newImage._image, &newImage._allocation, nullptr);

	image.clear();

	_submitBuffer([&newImage, &imageExtent, &stagingBuffer](VkCommandBuffer cmd) {
		VkImageSubresourceRange range;
		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = 1;
		range.baseArrayLayer = 0;
		range.layerCount = 1;

		VkImageMemoryBarrier imageBarrier_toTransfer = {};
		imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

		imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageBarrier_toTransfer.image = newImage._image;
		imageBarrier_toTransfer.subresourceRange = range;

		imageBarrier_toTransfer.srcAccessMask = 0;
		imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		//barrier the image into the transfer-receive layout
		vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);

		VkBufferImageCopy copyRegion = {};
		copyRegion.bufferOffset = 0;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;

		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.layerCount = 1;
		copyRegion.imageExtent = imageExtent;

		//copy the buffer into the image
		vkCmdCopyBufferToImage(cmd, stagingBuffer._buffer, newImage._image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);

		VkImageMemoryBarrier imageBarrier_toReadable = imageBarrier_toTransfer;

		imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		//barrier the image into the shader readable layout
		vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toReadable);
	});

	_vulkan->DestroyBuffer(stagingBuffer);
	_allocatedImage = newImage;

	return true;
}

void VkTexture::Destroy()
{
	_vulkan->DestroyImage(_allocatedImage);
}
