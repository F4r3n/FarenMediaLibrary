#pragma once
#include <unordered_map>
#include <memory>
namespace fmc
{
	class CCamera;
}

namespace fm
{
	class VkCamera;
	struct VkCameraCache
	{
		std::shared_ptr<VkCamera> FindOrCreate(fmc::CCamera* inCamera);
		std::unordered_map<uint32_t, std::shared_ptr<VkCamera>> _cameras;
	};
}
