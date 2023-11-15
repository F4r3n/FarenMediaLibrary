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
	struct OGLCameraCache
	{
		std::unordered_map<uint32_t, std::shared_ptr<fm::OGLCamera>> _cameras;
		std::shared_ptr<fm::OGLCamera> FindOrCreateCamera(fmc::CCamera* inCamera);
		void Release(uint32_t inID);
	private:
	};
}
