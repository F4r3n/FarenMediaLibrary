#include "OGLCamera.hpp"
#include "CameraCache.hpp"
#include <cassert>
#include "Components/CCamera.h"
using namespace fm;

std::shared_ptr<fm::OGLCamera> CameraCache::FindOrCreateCamera(fmc::CCamera* inCamera)
{
	if (auto it = _cameras.find(inCamera->GetInstance()); it != _cameras.end())
	{
		return it->second;
	}

	std::shared_ptr<fm::OGLCamera> cam;
	if (inCamera != nullptr)
	{
		cam = std::make_shared<fm::OGLCamera>(inCamera);
		cam->PrepareBuffer(sizeof(fmc::CCamera::shader_data));
		cam->CreateRenderTexture();
		_cameras.emplace(inCamera->GetInstance(), cam);
	}
	else
	{
		assert(false);
	}
	return cam;
}

