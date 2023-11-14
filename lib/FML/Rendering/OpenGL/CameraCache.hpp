#pragma once
#include <memory>
#include <unordered_map>

namespace fmc
{
	class CCamera;
}

namespace fm
{
	class OGLCamera;
	struct CameraCache
	{
		std::unordered_map<uint32_t, std::shared_ptr<fm::OGLCamera>> _cameras;
		std::shared_ptr<fm::OGLCamera> FindOrCreateCamera(fmc::CCamera* inCamera);
	};
}
