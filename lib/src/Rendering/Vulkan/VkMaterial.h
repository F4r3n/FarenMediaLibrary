#pragma once
#include <memory>
#include "Vulkan.h"
namespace fm
{
	class Material;
	class VkPipelineBuilder;
	class VkMaterial
	{
	public:
		VkMaterial(std::shared_ptr<fm::Material> inMaterial,
			VkDevice inDevice,
			VkRenderPass inRenderPass,
			VkExtent2D inExtent,
			const std::vector<VkDescriptorSetLayout>& inDescriptorLayouts);

		void Destroy();
		void BindPipeline(VkCommandBuffer cmd, VkPipelineBindPoint inType);
		VkPipelineLayout GetPipelineLayout() const;
	private:
		std::shared_ptr<Material> _material = nullptr;
		std::unique_ptr<VkPipelineBuilder> _pipeline = nullptr;
	};
}