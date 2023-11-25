#include "OGLCamera.hpp"
#include "OGLCameraCache.hpp"
#include <cassert>
#include "Components/CCamera.h"
using namespace fm;

std::shared_ptr<fm::OGLCamera> OGLCameraCache::FindOrCreateCamera(fmc::CCamera* inCamera)
{
	if (auto it = _cameras.find(inCamera->GetObjectID()); it != _cameras.end())
	{
		it->second->CheckStamp(inCamera);
		return it->second;
	}

	std::shared_ptr<fm::OGLCamera> cam;
	if (inCamera != nullptr)
	{
		cam = std::make_shared<fm::OGLCamera>(inCamera);
		cam->PrepareBuffer(sizeof(fmc::CCamera::shader_data));
		cam->CreateRenderTexture();
		_cameras.emplace(inCamera->GetObjectID(), cam);

		inCamera->SetDestroyCallback(std::bind(&OGLCameraCache::Release, this, std::placeholders::_1));
	}
	else
	{
		assert(false);
	}
	return cam;
}

OGLCameraCache::~OGLCameraCache()
{

}

void OGLCameraCache::Release(uint32_t inID)
{
	if (auto it = _cameras.find(inID); it != _cameras.end())
	{
		_cameras.erase(it);
	}
}


