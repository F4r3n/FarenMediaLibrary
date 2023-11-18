#include "VkCamera.hpp"
#include "Components/CCamera.h"
using namespace fm;

VkCamera::VkCamera(fmc::CCamera* inCamera)
{
	_camera = inCamera;
}

VkCamera::~VkCamera()
{

}

VkExtent2D VkCamera::GetExtent() const
{
	VkExtent2D extent;
	extent.width = static_cast<uint32_t>(_camera->GetViewport().w);
	extent.height = static_cast<uint32_t>(_camera->GetViewport().h);
	return extent;
}


void VkCamera::MapCameraBuffer(Vulkan* inVulkan, fm::AllocatedBuffer& ioBuffer)
{
	
	fm::math::mat projection = _camera->GetProjectionMatrix();
	fm::math::mat view = _camera->GetViewMatrix();

	projection[1][1] *= -1;

	fmc::Shader_data camData;
	camData.FM_P = projection;
	camData.FM_V = view;
	camData.FM_PV = projection * view;
	inVulkan->MapBuffer(ioBuffer, &camData, sizeof(fmc::Shader_data), 0);
}
