#include "VkCameraCache.hpp"
#include "Components/CCamera.h"
#include "VkCamera.hpp"
using namespace fm;

std::shared_ptr<VkCamera> VkCameraCache::FindOrCreate(fmc::CCamera* inCamera)
{
	if (auto it = _cameras.find(inCamera->GetObjectID()); it != _cameras.end())
	{
		return it->second;
	}

	std::shared_ptr<fm::VkCamera> cam;
	if (inCamera != nullptr)
	{
		cam = std::make_shared<fm::VkCamera>(inCamera);
		//cam->PrepareBuffer(sizeof(fmc::CCamera::shader_data));
		//cam->CreateRenderTexture();
		_cameras.emplace(inCamera->GetObjectID(), cam);

		//inCamera->SetDestroyCallback(std::bind(&OGLCameraCache::Release, this, std::placeholders::_1));
	}
	else
	{
		assert(false);
	}
	return cam;
}