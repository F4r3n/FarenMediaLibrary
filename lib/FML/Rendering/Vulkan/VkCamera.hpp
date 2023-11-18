#pragma once
#include "Vulkan.hpp"

class Vulkan;

namespace fmc
{
	class CCamera;
}

namespace fm
{
	class VkCamera
	{
	public:
		VkCamera(fmc::CCamera* inCamera);
		~VkCamera();
		void MapCameraBuffer(Vulkan* inVulkan, fm::AllocatedBuffer& ioBuffer);
		VkExtent2D GetExtent() const;
	private:
		fmc::CCamera*	_camera = nullptr;
		VkFramebuffer	_frameBuffer = nullptr;
	};
}