#include "VkMaterial.h"
#include "VkPipelineBuilder.h"
#include "Rendering/material.hpp"
using namespace fm;

VkMaterial::VkMaterial(std::shared_ptr<fm::Material> inMaterial,
	VkDevice inDevice,
	VkRenderPass inRenderPass,
	VkExtent2D inExtent,
	const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts)
{
	_material = inMaterial;
	_pipeline = std::make_unique<fm::VkPipelineBuilder>(inDevice, inRenderPass, inExtent, inDescriptorLayouts,
		_material->GetShader().get());
}


void VkMaterial::Destroy()
{
	_pipeline->DeInit();
}

void VkMaterial::BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType)
{
	vkCmdBindPipeline(cmd, inType, _pipeline->GetPipeline());
}

VkPipelineLayout VkMaterial::GetPipelineLayout() const
{
	return _pipeline->GetPipelineLayout();
}

